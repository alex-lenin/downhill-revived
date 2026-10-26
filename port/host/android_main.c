/* The Android start (finding 091). SDL's Java activity (port/android: DhdActivity over SDL's
   SDLActivity) loads libSDL3.so and libmain.so, then calls SDL_main here on its SDL thread. Before
   the game's own main (port/host64/launcher.c, compiled as dhd_launcher_main):
   - the log (stdout, stderr) goes to logcat (tag DownhillRevived) and to dhd.log;
   - the app's files folder (internal storage, SDL_GetAndroidInternalStoragePath) stands for the
     folder of dhd.exe: host_exe_path names dhd64 there, so dhd.ini, saves, dhd.log and the disc
     image's copy live there;
   - the disc image: SCUS-97177.iso in that folder. On the first start (or when the copy is not the
     game) the player picks the .iso once through Android's document picker (SDL's file dialog) and
     it is copied there (the content URI is read once; Android gives no path to it);
   - a new dhd.ini names the copy (iso) and turns the updater off (an APK is updated by reinstalling
     it); the player defaults (the native renderer at 2x, widescreen, movies skipped) come from
     host.c as on Windows;
   - SDL hints: landscape only, the Back button kept from closing the game.
   The game ends with exit() (the close of the activity, gs_pump_messages); SDL_main never returns
   to a second start in the same process. */
#include <SDL3/SDL.h>
#include <android/log.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#define TAG "DownhillRevived"
#define DISC_NAME "SCUS-97177.iso"

int dhd_launcher_main(int argc, char **argv);   /* launcher.c's main */
const char *host_disc_check(const char *path);  /* host.c: NULL when path is the game */

static char files[1024];      /* the app's files folder, with a trailing slash */
static char exe[1100];        /* files + "dhd64": what host_exe_path answers */

/* host_posix.c's host_exe_path on Android. */
const char *android_exe_path(void) { return exe; }

/* host_posix.c's host_error_box on Android: a message box (blocks until OK). */
void android_error_box(const char *title, const char *message)
{
    __android_log_print(ANDROID_LOG_ERROR, TAG, "%s: %s", title, message);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, NULL);
}

/* ---- the log: stdout and stderr through a pipe, line by line to logcat and dhd.log ---- */

static int log_pipe[2] = {-1, -1};
static FILE *log_file;
static volatile int log_busy;  /* the log thread is between a read and its last line written */

static void log_line(char *line)
{
    __android_log_write(ANDROID_LOG_INFO, TAG, line);
    if (log_file) { fputs(line, log_file); fputc('\n', log_file); fflush(log_file); }
}

/* Before the process ends (host_exit, exit, a fatal signal): the lines still in the pipe reach
   logcat and dhd.log, at most half a second. Only async-signal-safe calls besides fflush. */
void android_log_drain(void)
{
    if (log_pipe[0] < 0) return;
    fflush(stdout);
    for (int i = 0; i < 250; i++) {
        int pending = 0;
        if (ioctl(log_pipe[0], FIONREAD, &pending) != 0) return;
        if (!pending && !log_busy) return;
        usleep(2000);
    }
}

static void *log_thread(void *unused)
{
    (void)unused;
    char buf[2048];
    size_t have = 0;
    for (;;) {
        /* busy goes up before the read empties the pipe, so android_log_drain never sees neither */
        struct pollfd p = {log_pipe[0], POLLIN, 0};
        log_busy = 0;
        if (poll(&p, 1, -1) < 0) {
            if (errno == EINTR) continue;
            break;
        }
        log_busy = 1;
        ssize_t n = read(log_pipe[0], buf + have, sizeof buf - 1 - have);
        if (n < 0 && errno == EINTR) continue;
        if (n <= 0) break;
        have += (size_t)n;
        char *start = buf, *nl;
        while ((nl = memchr(start, '\n', (size_t)(buf + have - start)))) {
            *nl = 0;
            log_line(start);
            start = nl + 1;
        }
        have -= (size_t)(start - buf);
        memmove(buf, start, have);
        if (have == sizeof buf - 1) { buf[have] = 0; log_line(buf); have = 0; }  /* a very long line */
    }
    return NULL;
}

static void log_start(void)
{
    char path[1200];
    snprintf(path, sizeof path, "%sdhd.log", files);
    log_file = fopen(path, "w");
    if (pipe(log_pipe) != 0) return;
    dup2(log_pipe[1], STDOUT_FILENO);
    dup2(log_pipe[1], STDERR_FILENO);
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    pthread_t thread;
    if (pthread_create(&thread, NULL, log_thread, NULL) == 0) pthread_detach(thread);
    atexit(android_log_drain);
}

/* ---- the disc image ---- */

static SDL_Semaphore *picked;
static char chosen[4096];

static void SDLCALL on_pick(void *userdata, const char *const *list, int filter)
{
    (void)userdata; (void)filter;
    chosen[0] = 0;
    if (list && list[0]) snprintf(chosen, sizeof chosen, "%s", list[0]);
    SDL_SignalSemaphore(picked);
}

/* The document the player picks (a content:// URI), or "" when cancelled. */
static const char *pick(void)
{
    if (!picked) picked = SDL_CreateSemaphore(0);
    SDL_ShowOpenFileDialog(on_pick, NULL, NULL, NULL, 0, NULL, false);  /* no filter: any file */
    SDL_WaitSemaphore(picked);
    return chosen;
}

/* Copies the document at uri to path (through a .part file); 1 when done. */
static int copy_disc(const char *uri, const char *path)
{
    char part[1300];
    snprintf(part, sizeof part, "%s.part", path);
    SDL_IOStream *in = SDL_IOFromFile(uri, "rb");
    if (!in) { fprintf(stderr, "android: %s can't be opened (%s)\n", uri, SDL_GetError()); return 0; }
    FILE *out = fopen(part, "wb");
    if (!out) { SDL_CloseIO(in); fprintf(stderr, "android: %s can't be written\n", part); return 0; }
    static char block[1 << 20];
    uint64_t total = 0, next = 0;
    size_t n;
    int ok = 1;
    while ((n = SDL_ReadIO(in, block, sizeof block)) > 0) {
        if (fwrite(block, 1, n, out) != n) { ok = 0; break; }
        total += n;
        if (total >= next) { fprintf(stderr, "android: copying the disc image, %llu MB\n", (unsigned long long)(total >> 20)); next += 256u << 20; }
    }
    if (SDL_GetIOStatus(in) == SDL_IO_STATUS_ERROR) ok = 0;
    SDL_CloseIO(in);
    ok &= fclose(out) == 0;
    if (ok && total && rename(part, path) == 0) {
        fprintf(stderr, "android: disc image copied, %llu MB\n", (unsigned long long)(total >> 20));
        return 1;
    }
    unlink(part);
    return 0;
}

/* The copy of the disc image in the files folder, made on the first start; 0 if the player gave up. */
static int disc_ready(const char *path)
{
    struct stat st;
    const char *problem = stat(path, &st) == 0 ? host_disc_check(path) : "none";
    if (!problem) return 1;
    if (strcmp(problem, "none")) unlink(path);  /* not the game: ask again */
    char message[1024];
    snprintf(message, sizeof message, "%s%sChoose your Downhill Domination (USA) disc image: the game plays from "
             "your own copy of the disc, an .iso file (SCUS-97177). It is copied into the app once (a few GB "
             "of storage; this takes a minute or two with nothing on the screen).",
             strcmp(problem, "none") ? problem : "", strcmp(problem, "none") ? "\n\n" : "");
    for (;;) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Downhill Revived", message, NULL);
        const char *uri = pick();
        if (!*uri) return 0;
        fprintf(stderr, "android: disc image chosen: %s\n", uri);
        if (!copy_disc(uri, path)) {
            snprintf(message, sizeof message, "The file couldn't be copied (is there room on the phone?).\n\n"
                     "Choose your Downhill Domination (USA) disc image (SCUS-97177).");
            continue;
        }
        if (!(problem = host_disc_check(path))) return 1;
        unlink(path);
        snprintf(message, sizeof message, "%s\n\nChoose your Downhill Domination (USA) disc image (SCUS-97177).", problem);
    }
}

/* dhd.ini names the copy of the disc (host_disc_ready checks it in the core; DHD_ISO would skip
   that check) and turns the updater off. A new one gets both; an old one without an iso line gets
   it appended. The player defaults apply as soon as dhd.ini exists (host_settings_load). */
static void settings_start(const char *disc)
{
    char path[1200], line[1400];
    int has_iso = 0, exists = 0;
    snprintf(path, sizeof path, "%sdhd.ini", files);
    FILE *f = fopen(path, "r");
    if (f) {
        exists = 1;
        while (!has_iso && fgets(line, sizeof line, f)) {
            const char *s = line;
            while (*s == ' ' || *s == '\t') s++;
            has_iso = !strncasecmp(s, "iso", 3) && s[3] && strchr(" \t=", s[3]);
        }
        fclose(f);
    }
    if (has_iso) return;
    if (!(f = fopen(path, "a"))) return;
    if (!exists)
        fprintf(f, "; Downhill Revived (Android) settings: one \"key = value\" per line.\n"
                   "; An APK is updated by installing the new one.\n"
                   "auto_update = off\n");
    fprintf(f, "iso = %s\n", disc);
    fclose(f);
}

__attribute__((visibility("default"))) int SDL_main(int argc, char **argv)
{
    const char *storage = SDL_GetAndroidInternalStoragePath();
    if (!storage) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "no internal storage (%s)", SDL_GetError());
        return 1;
    }
    snprintf(files, sizeof files, "%s/", storage);
    snprintf(exe, sizeof exe, "%sdhd64", files);
    log_start();
    fprintf(stderr, "android: Downhill Revived in %s\n", files);
    {   /* a profile-collecting build (build_host64_android.py --pgo-generate): its counters go to the
           files folder (host_exit writes them; the process ends with _exit, so no atexit runs) */
        extern void __llvm_profile_set_filename(const char *) __attribute__((weak));
        static char profile[1100];
        snprintf(profile, sizeof profile, "%sdhd-%%p.profraw", files);
        if (__llvm_profile_set_filename) __llvm_profile_set_filename(profile);
    }
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
    SDL_SetHint(SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "1");    char disc[1200];
    snprintf(disc, sizeof disc, "%s" DISC_NAME, files);
    if (!disc_ready(disc)) {
        fprintf(stderr, "android: no disc image; the game ends\n");
        exit(0);
    }
    settings_start(disc);
    char *args[] = {exe, NULL};
    (void)argc; (void)argv;
    int code = dhd_launcher_main(1, args);
    fprintf(stderr, "android: the game ended (%d)\n", code);
    exit(code);
}
