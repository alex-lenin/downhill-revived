/* The auto-updater (finding 054), run by the first dhd.exe (the one before the relaunch, boot.c).

   At start, unless dhd.ini says auto_update = off (DHD_AUTO_UPDATE):
   - it asks the update source (update_url, DHD_UPDATE_URL; empty: the GitHub releases API of
     skuza-oss/downhill-revived) for the newest release, waiting at most CHECK_SECONDS. The
     answer is either GitHub's release JSON, whose latest.json asset (or, without one, its
     DownhillRevived-*-win.zip asset and that asset's sha256 digest) names the zip, or a
     latest.json itself: {"version": "X.Y.Z", "url": the zip (may be relative to the
     manifest), "sha256": the zip's 64 hex digits}. port/tools/package_release.py writes it;
   - if that release is newer than DHD_VERSION (port/include/version.h): auto_update = ask (the
     default) shows "Downhill Revived X.Y.Z is available" with Update now / Later; on updates
     without asking. No network, no answer or a bad one: the game just starts.
   Updating: the zip is downloaded beside dhd.exe (dhd-update\), its SHA-256 checked, unpacked
   (Windows' own tar.exe), the other files of its folder copied over (never dhd.ini, never a
   folder such as saves\), then the running dhd.exe is renamed dhd.exe.old, the new one put in
   its place and started with the environment this process was started with; the next start
   deletes dhd.exe.old and dhd-update\. A failure puts the old dhd.exe back, says why and
   starts the game as it was.

   Online play needs the newest release: the server refuses older ones (client_outdated). The
   online menu then offers Update Now (update_request): the game ends with UPDATE_EXIT_CODE and
   this first process updates without asking (update_after_game), then starts the game again.

   The check is skipped when a tool drives the game (DHD_NAV, DHD_AUTOPAD_FRAMES, DHD_RACE_SYNC,
   DHD_RACE_PAD, DHD_NET_CREATE, DHD_NET_JOIN or DHD_HEADLESS set), so tests and gates never
   reach the network. DHD_UPDATE_NOW=1 checks and updates without asking (tests). */
#include "hle.h"
#include "../include/version.h"
#include "../net/json.h"
#include "splash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winhttp.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt")

#define OFFICIAL_UPDATE_URL "https://api.github.com/repos/skuza-oss/downhill-revived/releases/latest"
#define RELEASES_PAGE "https://github.com/skuza-oss/downhill-revived/releases/latest"
#define TITLE "Downhill Revived"
#define WIDE2(s) L##s
#define WIDE(s) WIDE2(s)
enum { CHECK_SECONDS = 6, UPDATE_EXIT_CODE = 0x55504454 /* "UPDT": the game asks for an update */ };

typedef struct {
    char version[32], url[1024], sha256[65], page[512];
} Release;

static char *start_environment;  /* the block this process started with (before dhd.ini) */

/* ---- small helpers ---- */

static void exe_path(char *out, size_t size)
{
    GetModuleFileNameA(NULL, out, (DWORD)size);
}

/* The folder dhd.exe is in, with its trailing backslash. */
static void exe_dir(char *out, size_t size)
{
    exe_path(out, size);
    char *slash = strrchr(out, '\\');
    if (slash) slash[1] = 0;
}

/* "X.Y.Z" (a leading v, a missing patch and a -suffix allowed) to numbers; 0 if it isn't one. */
static int parse_version(const char *v, int out[3])
{
    out[0] = out[1] = out[2] = 0;
    if (*v == 'v' || *v == 'V') v++;
    if (*v < '0' || *v > '9') return 0;
    int n = sscanf(v, "%d.%d.%d", &out[0], &out[1], &out[2]);
    return n >= 2 && out[0] >= 0 && out[1] >= 0 && out[2] >= 0;
}

/* >0 when a is newer than b; versions that can't be read are never newer. */
static int compare_versions(const char *a, const char *b)
{
    int x[3], y[3];
    if (!parse_version(a, x) || !parse_version(b, y)) return 0;
    for (int i = 0; i < 3; i++)
        if (x[i] != y[i]) return x[i] - y[i];
    return 0;
}

static int is_hex64(const char *s)
{
    if (strlen(s) != 64) return 0;
    for (; *s; s++)
        if (!((*s >= '0' && *s <= '9') || (*s >= 'a' && *s <= 'f'))) return 0;
    return 1;
}

static void lower(char *s)
{
    for (; *s; s++)
        if (*s >= 'A' && *s <= 'Z') *s += 32;
}

static int automated(void)
{
    static const char *const tools[] = {"DHD_NAV", "DHD_AUTOPAD_FRAMES", "DHD_RACE_SYNC", "DHD_RACE_PAD",
                                        "DHD_NET_CREATE", "DHD_NET_JOIN", "DHD_HEADLESS"};
    for (unsigned i = 0; i < sizeof tools / sizeof *tools; i++)
        if (getenv(tools[i])) return 1;
    return 0;
}

/* auto_update: 0 off, 1 ask, 2 on. */
static int mode(void)
{
    const char *m = getenv("DHD_AUTO_UPDATE");
    if (!m || !*m || !_stricmp(m, "ask")) return 1;
    if (!_stricmp(m, "off") || !_stricmp(m, "0") || !_stricmp(m, "no") || !_stricmp(m, "false")) return 0;
    if (!_stricmp(m, "on") || !_stricmp(m, "1") || !_stricmp(m, "yes") || !_stricmp(m, "true")) return 2;
    return 1;
}

static const char *source_url(void)
{
    const char *u = getenv("DHD_UPDATE_URL");
    return u && *u ? u : OFFICIAL_UPDATE_URL;
}

/* ---- HTTP (WinHTTP) ---- */

typedef struct {
    HINTERNET session, connection, request;
} Http;

static void http_close(Http *h)
{
    if (h->request) WinHttpCloseHandle(h->request);
    if (h->connection) WinHttpCloseHandle(h->connection);
    if (h->session) WinHttpCloseHandle(h->session);
    memset(h, 0, sizeof *h);
}

/* Sends a GET for url (http or https; redirects followed) and reads the status. Returns the
   HTTP status (200 on success), or 0, with why for anything but 200. */
static int http_open(Http *h, const char *url, int receive_ms, char *why, size_t why_size)
{
    static const wchar_t agent[] = L"DownhillRevived/" WIDE(DHD_VERSION);
    wchar_t wurl[1100], host[256], path[1024];
    URL_COMPONENTS uc;
    DWORD status = 0, size = sizeof status;
    memset(h, 0, sizeof *h);
    memset(&uc, 0, sizeof uc);
    uc.dwStructSize = sizeof uc;
    MultiByteToWideChar(CP_UTF8, 0, url, -1, wurl, 1100);
    uc.lpszHostName = host, uc.dwHostNameLength = 256;
    uc.lpszUrlPath = path, uc.dwUrlPathLength = 1024;
    if (!WinHttpCrackUrl(wurl, 0, 0, &uc)) {
        snprintf(why, why_size, "not a web address: %s", url);
        return 0;
    }
    h->session = WinHttpOpen(agent, WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!h->session)  /* Windows before 8.1 has no automatic proxy */
        h->session = WinHttpOpen(agent, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (h->session) WinHttpSetTimeouts(h->session, 4000, 4000, 4000, receive_ms);
    if (h->session) h->connection = WinHttpConnect(h->session, host, uc.nPort, 0);
    if (h->connection)
        h->request = WinHttpOpenRequest(h->connection, L"GET", path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
                                        uc.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (!h->request ||
        !WinHttpSendRequest(h->request, L"Accept: application/vnd.github+json, application/json, */*\r\n", (DWORD)-1L,
                            WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(h->request, NULL) ||
        !WinHttpQueryHeaders(h->request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                             &status, &size, WINHTTP_NO_HEADER_INDEX)) {
        snprintf(why, why_size, "no answer from %s (error %lu)", url, GetLastError());
        http_close(h);
        return 0;
    }
    if (status != 200) snprintf(why, why_size, "HTTP %lu from %s", status, url);
    return (int)status;
}

/* The whole body of url into out (NUL-terminated); its length, or -1 with why. */
static int http_get_text(const char *url, char *out, int cap, char *why, size_t why_size)
{
    Http h;
    int n = 0;
    DWORD got;
    if (http_open(&h, url, 5000, why, why_size) != 200) {
        http_close(&h);
        return -1;
    }
    while (n < cap - 1 && WinHttpReadData(h.request, out + n, (DWORD)(cap - 1 - n), &got) && got) n += (int)got;
    out[n] = 0;
    http_close(&h);
    return n;
}

/* ---- the newest release ---- */

/* url relative to base (a manifest's own address) when it names no scheme. */
static void resolve(const char *base, const char *url, char *out, size_t size)
{
    char dir[1024];
    if (strstr(url, "://") || !*url) {
        snprintf(out, size, "%s", url);
        return;
    }
    snprintf(dir, sizeof dir, "%s", base);
    char *q = strchr(dir, '?'), *scheme = strstr(dir, "://");
    if (q) *q = 0;
    char *root = scheme ? strchr(scheme + 3, '/') : NULL;
    if (*url == '/') {  /* from the host's root */
        if (root) *root = 0;
    } else if (root) {
        strrchr(root, '/')[1] = 0;
    } else
        strncat(dir, "/", sizeof dir - strlen(dir) - 1);
    snprintf(out, size, "%s%s", dir, url);
}

/* A latest.json: version, url (the zip), sha256. */
static int read_manifest(const char *json, const char *base, Release *r, char *why, size_t why_size)
{
    char url[1024] = "";
    if (!json_string(json, "version", r->version, sizeof r->version) || !json_string(json, "url", url, sizeof url) ||
        !json_string(json, "sha256", r->sha256, sizeof r->sha256)) {
        snprintf(why, why_size, "the update manifest %s lacks version, url or sha256", base);
        return 0;
    }
    resolve(base, url, r->url, sizeof r->url);
    lower(r->sha256);
    json_string(json, "page", r->page, sizeof r->page);
    return 1;
}

/* The newest release the update source names; 0 with why when it can't be known. */
static int latest_release(Release *r, char *why, size_t why_size)
{
    static char body[256 * 1024];
    const char *url = source_url();
    memset(r, 0, sizeof *r);
    if (http_get_text(url, body, sizeof body, why, why_size) < 0) return 0;
    int assets = json_count(body, "assets");
    if (assets < 0) {  /* a latest.json itself */
        if (!read_manifest(body, url, r, why, why_size)) return 0;
    } else {  /* GitHub's release JSON */
        char path[48], name[160], manifest[1024] = "", zip[1024] = "", digest[80] = "", tag[40] = "", page[512] = "";
        json_string(body, "tag_name", tag, sizeof tag);
        json_string(body, "html_url", page, sizeof page);
        for (int i = 0; i < assets; i++) {
            snprintf(path, sizeof path, "assets[%d].name", i);
            if (!json_string(body, path, name, sizeof name)) continue;
            snprintf(path, sizeof path, "assets[%d].browser_download_url", i);
            size_t n = strlen(name);
            if (!strcmp(name, "latest.json")) json_string(body, path, manifest, sizeof manifest);
            else if (!strncmp(name, "DownhillRevived-", 16) && n > 24 && !strcmp(name + n - 8, "-win.zip")) {
                json_string(body, path, zip, sizeof zip);
                snprintf(path, sizeof path, "assets[%d].digest", i);
                json_string(body, path, digest, sizeof digest);
            }
        }
        if (manifest[0]) {
            if (http_get_text(manifest, body, sizeof body, why, why_size) < 0 ||
                !read_manifest(body, manifest, r, why, why_size)) return 0;
        } else if (zip[0] && !strncmp(digest, "sha256:", 7)) {
            snprintf(r->version, sizeof r->version, "%s", tag[0] == 'v' || tag[0] == 'V' ? tag + 1 : tag);
            snprintf(r->url, sizeof r->url, "%s", zip);
            snprintf(r->sha256, sizeof r->sha256, "%s", digest + 7);
            lower(r->sha256);
        } else {
            snprintf(why, why_size, "the newest release at %s has no latest.json and no zip with a digest", url);
            return 0;
        }
        if (page[0]) snprintf(r->page, sizeof r->page, "%s", page);
    }
    int v[3];
    if (!parse_version(r->version, v) || !is_hex64(r->sha256) ||
        (strncmp(r->url, "https://", 8) && strncmp(r->url, "http://", 7))) {
        snprintf(why, why_size, "the update source %s gave an unusable answer (version \"%s\")", url, r->version);
        return 0;
    }
    if (!r->page[0]) snprintf(r->page, sizeof r->page, "%s", strcmp(url, OFFICIAL_UPDATE_URL) ? url : RELEASES_PAGE);
    return 1;
}

/* The check at start runs in a thread, so a slow network can't hold the game up. */
static Release found;
static char found_why[400];
static volatile LONG found_ok;

static DWORD WINAPI check_thread(void *arg)
{
    static Release r;
    static char why[400];
    (void)arg;
    int ok = latest_release(&r, why, sizeof why);
    if (ok) found = r;
    snprintf(found_why, sizeof found_why, "%s", why);
    InterlockedExchange(&found_ok, ok ? 1 : -1);
    return 0;
}

/* ---- the progress window ---- */

static HWND progress;

static LRESULT CALLBACK progress_proc(HWND w, UINT m, WPARAM wp, LPARAM lp)
{
    if (m == WM_CLOSE) return 0;  /* it closes when the update is done */
    return DefWindowProcA(w, m, wp, lp);
}

static void pump(void)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

static void show_progress(const char *text)
{
    if (!progress) {
        WNDCLASSA wc;
        memset(&wc, 0, sizeof wc);
        wc.lpfnWndProc = progress_proc;
        wc.hInstance = GetModuleHandleA(NULL);
        wc.hCursor = LoadCursor(NULL, IDC_WAIT);
        wc.hIcon = LoadIconA(wc.hInstance, MAKEINTRESOURCEA(1));
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wc.lpszClassName = "DhdUpdateProgress";
        RegisterClassA(&wc);
        int w = 460, h = 120;
        progress = CreateWindowExA(WS_EX_TOPMOST, wc.lpszClassName, TITLE, WS_POPUP | WS_CAPTION | WS_VISIBLE,
                                   (GetSystemMetrics(SM_CXSCREEN) - w) / 2, (GetSystemMetrics(SM_CYSCREEN) - h) / 2, w, h,
                                   NULL, NULL, wc.hInstance, NULL);
        HWND label = CreateWindowExA(0, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_LEFT, 16, 18, w - 40, 50, progress,
                                     (HMENU)1, wc.hInstance, NULL);
        SendMessageA(label, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
    }
    SetDlgItemTextA(progress, 1, text);
    pump();
}

static void hide_progress(void)
{
    if (progress) DestroyWindow(progress);
    progress = NULL;
    pump();
}

/* ---- the update itself ---- */

/* Deletes a folder and everything in it. */
static void remove_tree(const char *dir)
{
    char pattern[MAX_PATH], path[MAX_PATH];
    WIN32_FIND_DATAA fd;
    snprintf(pattern, sizeof pattern, "%s\\*", dir);
    HANDLE f = FindFirstFileA(pattern, &fd);
    if (f != INVALID_HANDLE_VALUE) {
        do {
            if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, "..")) continue;
            snprintf(path, sizeof path, "%s\\%s", dir, fd.cFileName);
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) remove_tree(path);
            else {
                SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL);
                DeleteFileA(path);
            }
        } while (FindNextFileA(f, &fd));
        FindClose(f);
    }
    RemoveDirectoryA(dir);
}

/* Downloads r's zip into path, hashing it on the way; 1 when its SHA-256 is r's. */
static int download(const Release *r, const char *path, char *why, size_t why_size)
{
    Http h;
    BCRYPT_ALG_HANDLE alg = NULL;
    BCRYPT_HASH_HANDLE hash = NULL;
    uint8_t digest[32];
    static uint8_t buf[1 << 16];
    char text[200], hex[65];
    DWORD got, total = 0, size = sizeof total, wrote;
    unsigned long long have = 0;
    int ok = 0;
    if (http_open(&h, r->url, 30000, why, why_size) != 200) {
        http_close(&h);
        return 0;
    }
    WinHttpQueryHeaders(h.request, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                        &total, &size, WINHTTP_NO_HEADER_INDEX);
    HANDLE file = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        snprintf(why, why_size, "can't write %s (error %lu)", path, GetLastError());
        http_close(&h);
        return 0;
    }
    if (BCryptOpenAlgorithmProvider(&alg, BCRYPT_SHA256_ALGORITHM, NULL, 0) == 0) BCryptCreateHash(alg, &hash, NULL, 0, NULL, 0, 0);
    for (;;) {
        if (!WinHttpReadData(h.request, buf, sizeof buf, &got)) {
            snprintf(why, why_size, "the download stopped (error %lu)", GetLastError());
            break;
        }
        if (!got) {
            ok = 1;
            break;
        }
        if (!WriteFile(file, buf, got, &wrote, NULL) || wrote != got) {
            snprintf(why, why_size, "can't write %s (error %lu)", path, GetLastError());
            break;
        }
        if (hash) BCryptHashData(hash, buf, got, 0);
        have += got;
        if (total) snprintf(text, sizeof text, "Downloading Downhill Revived %s...\n%llu of %lu KB", r->version, have / 1024,
                            (unsigned long)(total / 1024));
        else snprintf(text, sizeof text, "Downloading Downhill Revived %s...\n%llu KB", r->version, have / 1024);
        show_progress(text);
    }
    CloseHandle(file);
    http_close(&h);
    if (ok && (!hash || BCryptFinishHash(hash, digest, sizeof digest, 0))) {
        snprintf(why, why_size, "can't check the download");
        ok = 0;
    }
    if (hash) BCryptDestroyHash(hash);
    if (alg) BCryptCloseAlgorithmProvider(alg, 0);
    if (!ok) return 0;
    for (int i = 0; i < 32; i++) snprintf(hex + 2 * i, 3, "%02x", digest[i]);
    if (strcmp(hex, r->sha256)) {
        snprintf(why, why_size, "the download is damaged: its SHA-256 is %.16s..., the release says %.16s...", hex, r->sha256);
        return 0;
    }
    fprintf(stderr, "update: %s downloaded, %llu bytes, sha256 %s\n", r->url, have, hex);
    return 1;
}

/* Unpacks zip into dir with Windows' own tar.exe (Windows 10 1803 and later). */
static int unpack(const char *zip, const char *dir, char *why, size_t why_size)
{
    char tar[MAX_PATH], cmd[3 * MAX_PATH + 32];
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    DWORD code = 1;
    memset(&si, 0, sizeof si);
    si.cb = sizeof si;
    UINT n = GetSystemDirectoryA(tar, MAX_PATH);
    snprintf(tar + n, sizeof tar - n, "\\tar.exe");
    if (GetFileAttributesA(tar) == INVALID_FILE_ATTRIBUTES) {
        snprintf(why, why_size, "this Windows has no tar.exe to unpack the update");
        return 0;
    }
    CreateDirectoryA(dir, NULL);
    snprintf(cmd, sizeof cmd, "\"%s\" -xf \"%s\" -C \"%s\"", tar, zip, dir);
    if (!CreateProcessA(tar, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, dir, &si, &pi)) {
        snprintf(why, why_size, "can't start tar.exe (error %lu)", GetLastError());
        return 0;
    }
    if (WaitForSingleObject(pi.hProcess, 120000) != WAIT_OBJECT_0) TerminateProcess(pi.hProcess, 1);
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    if (code) snprintf(why, why_size, "the update couldn't be unpacked (tar exit %lu)", code);
    return code == 0;
}

/* The unpacked release's folder: the one holding dhd.exe (the zip's DownhillRevived-X.Y.Z\). */
static int release_folder(const char *dir, char *out, size_t size)
{
    char pattern[MAX_PATH], exe[MAX_PATH];
    WIN32_FIND_DATAA fd;
    int found_one = 0;
    snprintf(exe, sizeof exe, "%s\\dhd.exe", dir);
    if (GetFileAttributesA(exe) != INVALID_FILE_ATTRIBUTES) {
        snprintf(out, size, "%s", dir);
        return 1;
    }
    snprintf(pattern, sizeof pattern, "%s\\*", dir);
    HANDLE f = FindFirstFileA(pattern, &fd);
    if (f == INVALID_HANDLE_VALUE) return 0;
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == '.') continue;
        snprintf(exe, sizeof exe, "%s\\%s\\dhd.exe", dir, fd.cFileName);
        if (GetFileAttributesA(exe) != INVALID_FILE_ATTRIBUTES) {
            snprintf(out, size, "%s\\%s", dir, fd.cFileName);
            found_one = 1;
        }
    } while (!found_one && FindNextFileA(f, &fd));
    FindClose(f);
    return found_one;
}

/* The release's other files (README.txt, LICENSE.txt, ...) over the old ones: plain files only,
   never dhd.ini or a log; folders (saves\) are never touched. */
static void copy_side_files(const char *from, const char *to)
{
    char pattern[MAX_PATH], src[MAX_PATH], dst[MAX_PATH];
    WIN32_FIND_DATAA fd;
    snprintf(pattern, sizeof pattern, "%s\\*", from);
    HANDLE f = FindFirstFileA(pattern, &fd);
    if (f == INVALID_HANDLE_VALUE) return;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        if (!_stricmp(fd.cFileName, "dhd.exe") || !_stricmp(fd.cFileName, "dhd.ini") ||
            !_stricmp(fd.cFileName, "dhd.log") || !_stricmp(fd.cFileName, "iso-check.txt")) continue;
        snprintf(src, sizeof src, "%s\\%s", from, fd.cFileName);
        snprintf(dst, sizeof dst, "%s%s", to, fd.cFileName);
        if (!CopyFileA(src, dst, FALSE)) fprintf(stderr, "update: can't replace %s (error %lu)\n", dst, GetLastError());
    } while (FindNextFileA(f, &fd));
    FindClose(f);
}

/* Starts exe as this process was started: its command line, the environment from before dhd.ini
   was read, the same log handles. */
static int start_again(const char *exe)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof si);
    si.cb = sizeof si;
    fflush(stdout), fflush(stderr);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE std[3] = {si.hStdInput, si.hStdOutput, si.hStdError};
    for (int i = 0; i < 3; i++)
        if (std[i] && std[i] != INVALID_HANDLE_VALUE) SetHandleInformation(std[i], HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    if (!CreateProcessA(exe, GetCommandLineA(), NULL, NULL, TRUE, GetPriorityClass(GetCurrentProcess()), start_environment,
                        NULL, &si, &pi)) {
        fprintf(stderr, "update: can't start %s (error %lu)\n", exe, GetLastError());
        return 0;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 1;
}

/* Downloads, checks, unpacks and puts release r in place of this dhd.exe, then starts it and
   ends this process. Returns only on failure, with the old dhd.exe in place and why. */
static void install(const Release *r, char *why, size_t why_size)
{
    char dir[MAX_PATH], work[MAX_PATH], zip[MAX_PATH], unpacked[MAX_PATH], folder[MAX_PATH], exe[MAX_PATH],
        old[MAX_PATH + 8], fresh[MAX_PATH + 16], text[200];
    exe_dir(dir, sizeof dir);
    exe_path(exe, sizeof exe);
    snprintf(work, sizeof work, "%sdhd-update", dir);
    snprintf(zip, sizeof zip, "%s\\DownhillRevived-%s-win.zip", work, r->version);
    snprintf(unpacked, sizeof unpacked, "%s\\unpacked", work);
    snprintf(old, sizeof old, "%s.old", exe);
    remove_tree(work);
    if (!CreateDirectoryA(work, NULL)) {
        snprintf(why, why_size, "can't make %s (error %lu): is the game's folder read-only?", work, GetLastError());
        return;
    }
    snprintf(text, sizeof text, "Downloading Downhill Revived %s...", r->version);
    show_progress(text);
    if (!download(r, zip, why, why_size)) goto failed;
    show_progress("Unpacking the update...");
    if (!unpack(zip, unpacked, why, why_size)) goto failed;
    if (!release_folder(unpacked, folder, sizeof folder)) {
        snprintf(why, why_size, "the update holds no dhd.exe");
        goto failed;
    }
    snprintf(fresh, sizeof fresh, "%s\\dhd.exe", folder);
    show_progress("Installing the update...");
    /* the running exe can be renamed but not overwritten: it becomes dhd.exe.old */
    SetFileAttributesA(old, FILE_ATTRIBUTE_NORMAL);
    DeleteFileA(old);
    if (!MoveFileExA(exe, old, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        snprintf(why, why_size, "can't rename %s (error %lu)", exe, GetLastError());
        goto failed;
    }
    if (!MoveFileExA(fresh, exe, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH)) {
        snprintf(why, why_size, "can't put the new dhd.exe in place (error %lu)", GetLastError());
        MoveFileExA(old, exe, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
        goto failed;
    }
    copy_side_files(folder, dir);
    fprintf(stderr, "update: Downhill Revived %s installed over %s; starting it\n", r->version, DHD_VERSION);
    hide_progress();
    if (!start_again(exe)) {
        snprintf(why, why_size, "the new dhd.exe is in place but didn't start");
        return;
    }
    ExitProcess(0);
failed:
    hide_progress();
    remove_tree(work);
    fprintf(stderr, "update: failed: %s\n", why);
}

/* Update now / Later: a message box whose Yes and No buttons are renamed by a CBT hook on this
   thread when the box opens. */
static HHOOK button_hook;

static LRESULT CALLBACK rename_buttons(int code, WPARAM wp, LPARAM lp)
{
    if (code == HCBT_ACTIVATE) {
        HWND box = (HWND)wp;
        SetDlgItemTextA(box, IDYES, "Update now");
        SetDlgItemTextA(box, IDNO, "Later");
        UnhookWindowsHookEx(button_hook);
        button_hook = NULL;
        return 0;
    }
    return CallNextHookEx(button_hook, code, wp, lp);
}

static int ask(const Release *r)
{
    char text[600];
    snprintf(text, sizeof text,
             "Downhill Revived %s is available (this is %s).\n\n"
             "Update now downloads it and starts the game again. Your settings (dhd.ini) and saves "
             "stay as they are. Online races need the newest version.", r->version, DHD_VERSION);
    button_hook = SetWindowsHookExA(WH_CBT, rename_buttons, NULL, GetCurrentThreadId());
    int answer = MessageBoxA(NULL, text, TITLE, MB_YESNO | MB_ICONINFORMATION | MB_SETFOREGROUND | MB_TOPMOST);
    if (button_hook) UnhookWindowsHookEx(button_hook), button_hook = NULL;
    fprintf(stderr, "update: the player chose %s\n", answer == IDYES ? "Update now" : "Later");
    return answer == IDYES;
}

static void failed_box(const char *why, const Release *r)
{
    char text[1400];
    snprintf(text, sizeof text, "The update didn't work:\n%s\n\nThe game starts as it was (%s). The new version can "
             "be downloaded from\n%s", why, DHD_VERSION, r && r->page[0] ? r->page : RELEASES_PAGE);
    MessageBoxA(NULL, text, TITLE, MB_OK | MB_ICONWARNING | MB_SETFOREGROUND | MB_TOPMOST);
}

/* ---- the entry points (boot.c, online_menu.c) ---- */

void update_save_environment(void)
{
    char *env = GetEnvironmentStringsA(), *end = env;
    if (!env) return;
    while (*end) end += strlen(end) + 1;
    size_t n = (size_t)(end - env) + 1;
    if ((start_environment = malloc(n))) memcpy(start_environment, env, n);
    FreeEnvironmentStringsA(env);
}

/* What an earlier update left: dhd.exe.old (the exe it replaced) and its download folder. */
static void clean_up(void)
{
    char exe[MAX_PATH], old[MAX_PATH + 8], dir[MAX_PATH], work[MAX_PATH];
    exe_path(exe, sizeof exe);
    exe_dir(dir, sizeof dir);
    snprintf(old, sizeof old, "%s.old", exe);
    snprintf(work, sizeof work, "%sdhd-update", dir);
    if (GetFileAttributesA(old) != INVALID_FILE_ATTRIBUTES) {
        for (int i = 0; i < 30; i++) {  /* just after an update the old process may still be ending */
            SetFileAttributesA(old, FILE_ATTRIBUTE_NORMAL);
            if (DeleteFileA(old) || GetLastError() == ERROR_FILE_NOT_FOUND) break;
            Sleep(100);
        }
        fprintf(stderr, "update: %s %s\n", old, GetFileAttributesA(old) == INVALID_FILE_ATTRIBUTES ? "deleted" : "still there");
    }
    if (GetFileAttributesA(work) != INVALID_FILE_ATTRIBUTES) remove_tree(work);
}

/* The first process, after dhd.ini: clean up, then look for a newer release and offer it. */
void update_at_start(void)
{
    if (getenv("DHD_RELAUNCHED")) return;
    clean_up();
    const char *now_env = getenv("DHD_UPDATE_NOW");
    int now = now_env && atoi(now_env), m = now ? 2 : mode();
    if (!m || (!now && automated())) return;
    HANDLE t = CreateThread(NULL, 0, check_thread, NULL, 0, NULL);
    if (!t) return;
    DWORD waited = WaitForSingleObject(t, CHECK_SECONDS * 1000);
    CloseHandle(t);
    if (waited != WAIT_OBJECT_0 || found_ok != 1) {
        fprintf(stderr, "update: no check (%s); starting %s\n", waited != WAIT_OBJECT_0 ? "no answer in time" : found_why,
                DHD_VERSION);
        return;
    }
    if (compare_versions(found.version, DHD_VERSION) <= 0) {
        fprintf(stderr, "update: %s is the newest (the source has %s)\n", DHD_VERSION, found.version);
        return;
    }
    fprintf(stderr, "update: %s is available (this is %s)\n", found.version, DHD_VERSION);
    splash_close();  /* the question and the update's own progress instead */
    if (m == 1 && !ask(&found)) return;
    char why[600];
    install(&found, why, sizeof why);
    failed_box(why, &found);
}

/* The first process, when the game ended with code: if the game asked for an update (the
   online menu's Update Now), update without asking, then start the game again (the new one, or
   this one when there is none). Returns the exit code to end with. */
int update_after_game(int code)
{
    if ((unsigned)code != UPDATE_EXIT_CODE) return code;
    static Release r;
    char why[600] = "", exe[MAX_PATH];
    fprintf(stderr, "update: the game asked for an update\n");
    show_progress("Looking for the new version...");
    if (!latest_release(&r, why, sizeof why)) {
        hide_progress();
        failed_box(why, NULL);
    } else if (compare_versions(r.version, DHD_VERSION) <= 0) {
        char text[1200];
        hide_progress();
        snprintf(text, sizeof text, "No version newer than %s was found at\n%s\n\nIf online races still ask for an "
                 "update, the new version can be downloaded from\n%s", DHD_VERSION, source_url(), r.page);
        MessageBoxA(NULL, text, TITLE, MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND | MB_TOPMOST);
    } else {
        install(&r, why, sizeof why);
        failed_box(why, &r);
    }
    exe_path(exe, sizeof exe);
    return start_again(exe) ? 0 : 1;
}

/* The online menu's Update Now: the game ends and the first dhd.exe updates (update_after_game).
   Without that first process (the game wasn't relaunched) this one does it. */
void update_request(void)
{
    fprintf(stderr, "update: asked for from the online menu\n");
    fflush(stdout), fflush(stderr);
    if (getenv("DHD_RELAUNCHED")) ExitProcess(UPDATE_EXIT_CODE);
    exit(update_after_game(UPDATE_EXIT_CODE));
}
