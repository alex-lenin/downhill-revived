/* The Windows host of host_api.h (port/host): the module's OS services. The launcher, window,
   input, updater and the rest of the Windows shell stay in host.c. Fibers, guest RAM, the FP
   mode, the guest-safe callback contracts and the thread name live in the host64 glue
   (fiber_x64.c, guest_memory*.c, fp_mode.c, the generated imports_gen.c, host.c). The module
   sees only host_api.h. */
#include "host_api.h"
#include "../include/guest.h"
#include "../net/net.h"
#include "../runtime/sha256.h"

#include <fcntl.h>
#include <intrin.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <xmmintrin.h>

/* On 64-bit hosts guest_base (guest.h) is defined by port/host64/guest_memory.c, which maps
   guest memory. */

/* The instruction pointer and stack pointer of a thread's CONTEXT (the 32-bit and 64-bit records
   name them differently). */
#ifdef _WIN64
#define CTX_IP(c) ((uintptr_t)(c)->Rip)
#else
#define CTX_IP(c) ((uintptr_t)(c)->Eip)
#endif

double host_wall_seconds(void)
{
    static LARGE_INTEGER start, freq;
    LARGE_INTEGER t;
    if (!freq.QuadPart) {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
    }
    QueryPerformanceCounter(&t);
    return (double)(t.QuadPart - start.QuadPart) / (double)freq.QuadPart;
}

void host_sleep_ms(unsigned ms) { Sleep(ms); }

/* Until a QueryPerformanceCounter time: a high-resolution waitable timer (Windows 10 1803+; one
   per thread, the EE and the presenter both wait) sleeps to a millisecond before it (it woke up
   to 0.85 ms late, finding 052), then a pause-instruction spin. Without that timer, Sleep at 1 ms
   timer resolution to 2 ms before. */
void host_wait_wall(double seconds)
{
    static __declspec(thread) HANDLE timer;
    static __declspec(thread) int tried;
    static volatile LONG coarse;
    if (!tried) {
        tried = 1;
        timer = CreateWaitableTimerExW(NULL, NULL, 0x00000002 /* CREATE_WAITABLE_TIMER_HIGH_RESOLUTION */,
                                       TIMER_ALL_ACCESS);
        if (!timer && !InterlockedExchange(&coarse, 1)) {
            HMODULE winmm = LoadLibraryA("winmm.dll");
            FARPROC begin = winmm ? GetProcAddress(winmm, "timeBeginPeriod") : NULL;
            if (begin) ((UINT (WINAPI *)(UINT))(void *)begin)(1);
        }
    }
    double left = seconds - host_wall_seconds();
    if (timer && left > 0.001) {
        LARGE_INTEGER due;
        due.QuadPart = -(LONGLONG)((left - 0.001) * 1e7);  /* relative, 100 ns units */
        if (SetWaitableTimer(timer, &due, 0, NULL, NULL, FALSE))
            WaitForSingleObject(timer, INFINITE);
    } else if (!timer && left > 0.002)
        Sleep((DWORD)((left - 0.002) * 1000));
    while (host_wall_seconds() < seconds)
        YieldProcessor();
}

/* ---- environment, errors and the program ---- */
const char *host_getenv(const char *name) { return getenv(name); }

unsigned long host_last_error(void) { return GetLastError(); }

void host_error_box(const char *title, const char *message)
{
    MessageBoxA(NULL, message, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
}

unsigned host_exe_path(char *out, unsigned size) { return GetModuleFileNameA(NULL, out, size); }

void host_exit(int code) { ExitProcess((UINT)code); }

int host_display_hz(void)
{
    DEVMODEA mode;
    memset(&mode, 0, sizeof mode);
    mode.dmSize = sizeof mode;
    return EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &mode) ? (int)mode.dmDisplayFrequency : 0;
}

unsigned host_fp_csr(void) { return _mm_getcsr(); }
void host_fp_set_csr(unsigned csr) { _mm_setcsr(csr); }
uint64_t host_cycles(void) { return __rdtsc(); }

void host_putenv(const char *text) { _putenv(text); }
void host_putenv_s(const char *name, const char *value) { _putenv_s(name, value); }

void host_dpi_aware(void)
{
    HMODULE user32 = GetModuleHandleA("user32.dll");
    typedef BOOL(WINAPI * ContextFn)(HANDLE);
    typedef BOOL(WINAPI * AwareFn)(void);
    ContextFn context = user32 ? (ContextFn)(void *)GetProcAddress(user32, "SetProcessDpiAwarenessContext") : NULL;
    if (context && context((HANDLE)-4 /* DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 */)) return;
    AwareFn aware = user32 ? (AwareFn)(void *)GetProcAddress(user32, "SetProcessDPIAware") : NULL;
    if (aware) aware();
}

double host_monitor_aspect(void)
{
    MONITORINFO m = {sizeof m};
    POINT origin = {0, 0};
    if (!GetMonitorInfoA(MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY), &m)) return 16.0 / 9.0;
    double w = m.rcMonitor.right - m.rcMonitor.left, h = m.rcMonitor.bottom - m.rcMonitor.top;
    return w > 0 && h > 0 ? w / h : 16.0 / 9.0;
}

int host_mem_protect(uint32_t at, uint32_t size, int protection)
{
    DWORD old;
    return VirtualProtect(GUEST(at), size, protection == HOST_MEM_NOACCESS ? PAGE_NOACCESS : PAGE_READWRITE, &old) != 0;
}

/* The access-fault guard: one vectored handler, for the thread that installed it. */
static int (*guard_on_fault)(uint32_t address);
static DWORD guard_thread;

static LONG CALLBACK guard_fault(EXCEPTION_POINTERS *e)
{
    const EXCEPTION_RECORD *r = e->ExceptionRecord;
    if (r->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || r->NumberParameters < 2 ||
        GetCurrentThreadId() != guard_thread || !guard_on_fault(GADDR(r->ExceptionInformation[1])))
        return EXCEPTION_CONTINUE_SEARCH;
    return EXCEPTION_CONTINUE_EXECUTION;
}

int host_fault_guard(int (*on_fault)(uint32_t address))
{
    guard_on_fault = on_fault;
    guard_thread = GetCurrentThreadId();
    return AddVectoredExceptionHandler(1, guard_fault) != NULL;
}

int host_fault_guard_here(void) { return guard_thread && GetCurrentThreadId() == guard_thread; }

typedef struct { void (*fn)(void *); void *arg; } DetachedStart;

static DWORD WINAPI detached_run(void *p)
{
    DetachedStart start = *(DetachedStart *)p;
    free(p);
    start.fn(start.arg);
    return 0;
}

int host_thread_detach(void (*fn)(void *), void *arg)
{
    DetachedStart *start = malloc(sizeof *start);
    HANDLE t;
    if (!start) return 0;
    start->fn = fn;
    start->arg = arg;
    t = CreateThread(NULL, 0, detached_run, start, 0, NULL);
    if (!t) {
        free(start);
        return 0;
    }
    CloseHandle(t);
    return 1;
}

int host_path_info(const char *path, uint64_t *size, uint64_t *write_time)
{
    WIN32_FILE_ATTRIBUTE_DATA info;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &info)) return 0;
    *size = (uint64_t)info.nFileSizeHigh << 32 | info.nFileSizeLow;
    *write_time = (uint64_t)info.ftLastWriteTime.dwHighDateTime << 32 | info.ftLastWriteTime.dwLowDateTime;
    return 1;
}

/* dhd.exe's .text: the section of the image that holds the code, by its PE header. */
void host_code_range(uintptr_t *lo, uintptr_t *hi)
{
    uint8_t *base = (uint8_t *)GetModuleHandleA(NULL);
    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)base;
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(base + dos->e_lfanew);
    IMAGE_SECTION_HEADER *sec = IMAGE_FIRST_SECTION(nt);
    *lo = *hi = 0;
    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++)
        if (memcmp(sec[i].Name, ".text\0\0\0", 8) == 0) {
            *lo = (uintptr_t)base + sec[i].VirtualAddress;
            *hi = *lo + sec[i].Misc.VirtualSize;
        }
}

void host_memory_regions(void (*visit)(uintptr_t base, uintptr_t end, int host_memory))
{
    HMODULE self = GetModuleHandleA(NULL);
    MEMORY_BASIC_INFORMATION m;
    uintptr_t a = 0x10000;
    while (a < 0xFFFF0000u && VirtualQuery((void *)a, &m, sizeof m) == sizeof m) {
        uintptr_t base = (uintptr_t)m.BaseAddress, end = base + m.RegionSize;
        if (end <= base) break;
        visit(base, end, m.State != MEM_FREE && (m.Type != MEM_IMAGE || (HMODULE)m.AllocationBase != self));
        a = end;
    }
}

/* ---- files ----
   A file is a C library FILE behind a small handle (1..FILE_SLOTS, 0 is none). The disc image
   and the card's files are read as the C library reads them, but a read opens the file with
   delete sharing as well, the way the autopad script reader always has, so a driver can replace
   the script while it is being read. */
#define FILE_SLOTS 32
static FILE *file_slot[FILE_SLOTS + 1];

static FILE *file_of(HostFile h) { return h > 0 && h <= FILE_SLOTS ? file_slot[h] : NULL; }

HostFile host_file_open(const char *path, unsigned mode)
{
    FILE *f;
    int slot;
    if (mode & HOST_FILE_WRITE) {
        const char *kind = mode & HOST_FILE_TRUNCATE ? "w+b" : "r+b";
        f = fopen(mode & HOST_FILE_DISCARD ? "NUL" : path, kind);
    } else {
        HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        int fd;
        if (h == INVALID_HANDLE_VALUE) return HOST_FILE_NONE;
        fd = _open_osfhandle((intptr_t)h, _O_RDONLY | _O_BINARY);  /* the FILE closes the handle */
        if (fd < 0) {
            CloseHandle(h);
            return HOST_FILE_NONE;
        }
        f = _fdopen(fd, "rb");
        if (!f) {
            _close(fd);
            return HOST_FILE_NONE;
        }
    }
    if (!f) return HOST_FILE_NONE;
    for (slot = 1; slot <= FILE_SLOTS && file_slot[slot]; slot++)
        ;
    if (slot > FILE_SLOTS) {
        fclose(f);
        return HOST_FILE_NONE;
    }
    file_slot[slot] = f;
    return slot;
}

void host_file_close(HostFile h)
{
    FILE *f = file_of(h);
    if (f) {
        fclose(f);
        file_slot[h] = NULL;
    }
}

int64_t host_file_read(HostFile h, void *dst, size_t size)
{
    FILE *f = file_of(h);
    size_t n;
    if (!f) return -1;
    n = fread(dst, 1, size, f);
    return n < size && ferror(f) ? -1 : (int64_t)n;
}

size_t host_file_write(HostFile h, const void *src, size_t size)
{
    FILE *f = file_of(h);
    return f ? fwrite(src, 1, size, f) : 0;
}

int host_file_flush(HostFile h)
{
    FILE *f = file_of(h);
    return f ? fflush(f) : -1;
}

int host_file_seek(HostFile h, uint64_t offset)
{
    FILE *f = file_of(h);
    return f ? _fseeki64(f, (long long)offset, SEEK_SET) : -1;
}

uint64_t host_file_size(HostFile h)
{
    FILE *f = file_of(h);
    long long here, end;
    if (!f || (here = _ftelli64(f)) < 0) return 0;
    if (_fseeki64(f, 0, SEEK_END) != 0) return 0;
    end = _ftelli64(f);
    _fseeki64(f, here, SEEK_SET);
    return end < 0 ? 0 : (uint64_t)end;
}

/* ---- paths and folders ---- */
#define DIR_SLOTS 16
static HANDLE dir_slot[DIR_SLOTS + 1];

/* A FILETIME (UTC) as local time, as the card's directory entries show it. */
static void host_time_of(const FILETIME *ft, HostTime *out)
{
    FILETIME local;
    SYSTEMTIME st;
    memset(out, 0, sizeof *out);
    if (!FileTimeToLocalFileTime(ft, &local) || !FileTimeToSystemTime(&local, &st)) return;
    out->year = st.wYear, out->month = st.wMonth, out->day = st.wDay;
    out->hour = st.wHour, out->minute = st.wMinute, out->second = st.wSecond;
}

static void dir_entry_of(const WIN32_FIND_DATAA *fd, HostDirEntry *e)
{
    memset(e, 0, sizeof *e);
    snprintf(e->name, sizeof e->name, "%s", fd->cFileName);
    e->is_dir = (fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    e->size = fd->nFileSizeLow;
    host_time_of(&fd->ftCreationTime, &e->created);
    host_time_of(&fd->ftLastWriteTime, &e->modified);
}

HostDir host_dir_open(const char *path, HostDirEntry *first)
{
    char pattern[HOST_PATH_MAX];
    WIN32_FIND_DATAA fd;
    HANDLE h;
    int slot;
    snprintf(pattern, sizeof pattern, "%s\\*", path);
    h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return HOST_DIR_NONE;
    for (slot = 1; slot <= DIR_SLOTS && dir_slot[slot]; slot++)
        ;
    if (slot > DIR_SLOTS) {
        FindClose(h);
        return HOST_DIR_NONE;
    }
    dir_slot[slot] = h;
    dir_entry_of(&fd, first);
    return slot;
}

int host_dir_next(HostDir d, HostDirEntry *e)
{
    WIN32_FIND_DATAA fd;
    if (d <= 0 || d > DIR_SLOTS || !dir_slot[d] || !FindNextFileA(dir_slot[d], &fd)) return 0;
    dir_entry_of(&fd, e);
    return 1;
}

void host_dir_close(HostDir d)
{
    if (d > 0 && d <= DIR_SLOTS && dir_slot[d]) {
        FindClose(dir_slot[d]);
        dir_slot[d] = NULL;
    }
}

int host_path_is_dir(const char *path)
{
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}

int host_path_exists(const char *path) { return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES; }

int host_dir_create(const char *path) { return CreateDirectoryA(path, NULL) ? 1 : 0; }

/* ---- the heartbeat (autopad.c) ---- */
static HANDLE heartbeat_thread;  /* a duplicate of the thread that started the heartbeat */
static void (*heartbeat_beat)(void);

static DWORD WINAPI heartbeat_run(void *unused)
{
    (void)unused;
    for (;;) {
        Sleep(30000);
        heartbeat_beat();
    }
}

void host_heartbeat_start(void (*beat)(void))
{
    HANDLE self;
    heartbeat_beat = beat;
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &self, 0, FALSE,
                    DUPLICATE_SAME_ACCESS);
    heartbeat_thread = self;
    CloseHandle(CreateThread(NULL, 0, heartbeat_run, NULL, 0, NULL));
}

uintptr_t host_heartbeat_eip(void)
{
    CONTEXT c;
    uintptr_t eip = 0;
    memset(&c, 0, sizeof c);
    c.ContextFlags = CONTEXT_CONTROL;
    if (SuspendThread(heartbeat_thread) != (DWORD)-1) {
        if (GetThreadContext(heartbeat_thread, &c)) eip = CTX_IP(&c);
        ResumeThread(heartbeat_thread);
    }
    return eip;
}

void host_console_write(uint32_t address, uint32_t bytes) {
    fwrite(GUEST(address),1,bytes,stderr); fflush(stderr);
}

/* ---- async services: file SHA-256 and the lobby list (online_menu.c) ----
   The blocking part runs on a host thread; only results cross, as guest-address
   buffers the game thread copies at poll time. One job of each kind runs at a
   time. The Interlocked state is the whole protocol: the worker publishes its
   result before marking itself done, and the poller copies it before idling. */
static LONG sha_job;  /* 0 idle, 1 working, 2 done, 3 failed */
static char sha_path[1024];
static uint8_t sha_digest[32];

static DWORD WINAPI sha_run(void *unused)
{
    static uint8_t buf[1 << 20];
    Sha256 hash;
    FILE *f;
    int ok = 0;
    (void)unused;
    f = fopen(sha_path, "rb");
    if (f) {
        size_t n;
        sha256_init(&hash);
        while ((n = fread(buf, 1, sizeof buf, f)) > 0) sha256_update(&hash, buf, n);
        ok = !ferror(f);
        sha256_final(&hash, sha_digest);
        fclose(f);
    }
    InterlockedExchange(&sha_job, ok ? 2 : 3);
    return 0;
}

int host_service_file_sha256_start(uint32_t path)
{
    const char *at = GUEST(path);
    HANDLE t;
    if (InterlockedCompareExchange(&sha_job, 1, 0) != 0) return 0;
    if (!path || strlen(at) >= sizeof sha_path) {
        InterlockedExchange(&sha_job, 0);
        return 0;
    }
    snprintf(sha_path, sizeof sha_path, "%s", at);
    t = CreateThread(NULL, 0, sha_run, NULL, 0, NULL);
    if (!t) {
        InterlockedExchange(&sha_job, 0);
        return 0;
    }
    CloseHandle(t);
    return 1;
}

int host_service_file_sha256_poll(uint32_t digest)
{
    if (InterlockedCompareExchange(&sha_job, 0, 0) == 1) return 0;
    if (InterlockedCompareExchange(&sha_job, 0, 0) == 2) {
        memcpy(GUEST(digest), sha_digest, sizeof sha_digest);
        InterlockedExchange(&sha_job, 0);
        return 1;
    }
    InterlockedExchange(&sha_job, 0);
    return -1;
}

/* The lobby fetch: entries published here, copied to the guest buffer at poll time. */
#define ASYNC_LOBBY_MAX 64
static LONG lobby_job;  /* 0 idle, 1 working, 2 done, 3 failed */
static NetLobbyInfo lobby_list[ASYNC_LOBBY_MAX];
static int lobby_max, lobby_count;

static DWORD WINAPI lobby_run(void *unused)
{
    int n;
    (void)unused;
    n = net_list_lobbies(lobby_list, lobby_max);
    if (n < 0) {
        InterlockedExchange(&lobby_job, 3);
        return 0;
    }
    lobby_count = n;
    InterlockedExchange(&lobby_job, 2);
    return 0;
}

int host_service_net_list_lobbies_start(int max)
{
    HANDLE t;
    if (max <= 0 || max > ASYNC_LOBBY_MAX) return 0;
    if (InterlockedCompareExchange(&lobby_job, 1, 0) != 0) return 0;
    lobby_max = max;
    lobby_count = -1;
    t = CreateThread(NULL, 0, lobby_run, NULL, 0, NULL);
    if (!t) {
        InterlockedExchange(&lobby_job, 0);
        return 0;
    }
    CloseHandle(t);
    return 1;
}

int host_service_net_list_lobbies_poll(uint32_t out, int max)
{
    int n;
    if (InterlockedCompareExchange(&lobby_job, 0, 0) == 1) return -2;
    if (InterlockedCompareExchange(&lobby_job, 0, 0) != 2) {
        InterlockedExchange(&lobby_job, 0);
        return -1;
    }
    n = lobby_count < max ? lobby_count : max;
    if (n > 0) memcpy(GUEST(out), lobby_list, (size_t)n * sizeof lobby_list[0]);
    InterlockedExchange(&lobby_job, 0);
    return n;
}
