/* The module's side of wasi_snapshot_preview1: the 16 imports that wasi-libc's stdio and file layer
 * (libc.a) reference, defined here so the module has no WASI import. Each maps onto a host service
 * (host_console_write, host_file_*, host_wall_seconds, host_exit) or returns a WASI errno. Nothing
 * reports success without doing the work: where there is no host service the answer is an errno.
 *
 * Descriptors: 0 has no input (EBADF); 1 and 2 are the console; 3 is the one preopened directory,
 * "/", and 4 and up are host files opened by path_open. wasi-libc opens files only below a
 * preopen, and resolves any other path ("F:\x", "x.tsv") against its cwd "/", so path_open gets
 * the path as the game wrote it (minus a leading '/') and uses it as a host path (relative to the
 * host working directory). Process arguments are not exposed (ENOSYS); getenv reads the host's. */
#include <stddef.h>
#include <stdint.h>
#include "../host/host_api.h"

#define W_EBADF 8
#define W_EINVAL 28
#define W_EIO 29
#define W_ENOENT 44
#define W_ENOSYS 52
#define W_ENOTSUP 58
#define W_ESPIPE 70

#define W_FILETYPE_CHARACTER_DEVICE 2
#define W_FILETYPE_REGULAR_FILE 4
#define W_RIGHT_FD_READ (1ull << 1)
#define W_RIGHT_FD_WRITE (1ull << 6)
#define W_OFLAG_CREAT 1
#define W_OFLAG_TRUNC 8
#define W_WHENCE_SET 0
#define W_WHENCE_CUR 1
#define W_WHENCE_END 2
#define W_CLOCK_REALTIME 0
#define W_CLOCK_MONOTONIC 1

#define PREOPEN_FD 3
#define FIRST_FILE_FD 4
#define W_PREOPENTYPE_DIR 0
#define W_FILETYPE_DIRECTORY 3
#define MAX_FILE_FDS 16
#define MAX_PATH 256

typedef struct {
    HostFile file;  /* HOST_FILE_NONE when the slot is free */
    uint64_t pos;   /* the position from the start, kept for SEEK_CUR */
    int writable;
} FileSlot;

static FileSlot files[MAX_FILE_FDS];

typedef struct {
    uint32_t buf;
    uint32_t len;
} WasiIov;  /* iovec and ciovec are both two u32 in wasm32 */

static FileSlot *slot_of(uint32_t fd) {
    if (fd < FIRST_FILE_FD || fd - FIRST_FILE_FD >= MAX_FILE_FDS) return NULL;
    FileSlot *s = &files[fd - FIRST_FILE_FD];
    return s->file == HOST_FILE_NONE ? NULL : s;
}

static void put_u32(uint32_t addr, uint32_t v) { *(uint32_t *)(uintptr_t)addr = v; }
static void put_u64(uint32_t addr, uint64_t v) { *(uint64_t *)(uintptr_t)addr = v; }

int __imported_wasi_snapshot_preview1_fd_write(uint32_t fd, uint32_t iovs, uint32_t iovs_len, uint32_t nwritten) {
    const WasiIov *v = (const WasiIov *)(uintptr_t)iovs;
    uint64_t total = 0;
    if (fd == 1 || fd == 2) {
        for (uint32_t k = 0; k < iovs_len; k++) {
            if (v[k].len) host_console_write(v[k].buf, v[k].len);
            total += v[k].len;
        }
        put_u32(nwritten, (uint32_t)total);
        return 0;
    }
    FileSlot *s = slot_of(fd);
    if (!s) return W_EBADF;
    if (!s->writable) return W_EBADF;
    for (uint32_t k = 0; k < iovs_len; k++) {
        if (!v[k].len) continue;
        size_t n = host_file_write(s->file, (const void *)(uintptr_t)v[k].buf, v[k].len);
        s->pos += n;
        total += n;
        if (n < v[k].len) break;
    }
    /* The C library's buffer is the only one, as for a native program: its fflush reaches the
       file (a report read while the game runs, sound_debug.h's WAV header). */
    host_file_flush(s->file);
    put_u32(nwritten, (uint32_t)total);
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_read(uint32_t fd, uint32_t iovs, uint32_t iovs_len, uint32_t nread) {
    const WasiIov *v = (const WasiIov *)(uintptr_t)iovs;
    FileSlot *s = slot_of(fd);
    if (!s) return W_EBADF;
    uint64_t total = 0;
    for (uint32_t k = 0; k < iovs_len; k++) {
        if (!v[k].len) continue;
        int64_t n = host_file_read(s->file, (void *)(uintptr_t)v[k].buf, v[k].len);
        if (n < 0) return W_EIO;
        s->pos += (uint64_t)n;
        total += (uint64_t)n;
        if ((uint64_t)n < v[k].len) break;  /* end of file */
    }
    put_u32(nread, (uint32_t)total);
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_seek(uint32_t fd, uint64_t offset, uint32_t whence, uint32_t newoffset) {
    FileSlot *s = slot_of(fd);
    if (!s) return (fd == 0 || fd == 1 || fd == 2) ? W_ESPIPE : W_EBADF;
    uint64_t target;
    if (whence == W_WHENCE_SET) {
        target = offset;
    } else if (whence == W_WHENCE_CUR) {
        target = s->pos + offset;
    } else if (whence == W_WHENCE_END) {
        target = host_file_size(s->file) + offset;
    } else {
        return W_EINVAL;
    }
    if (host_file_seek(s->file, target) != 0) return W_EINVAL;
    s->pos = target;
    put_u64(newoffset, target);
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_close(uint32_t fd) {
    FileSlot *s = slot_of(fd);
    if (!s) return W_EBADF;
    host_file_close(s->file);
    s->file = HOST_FILE_NONE;
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_fdstat_get(uint32_t fd, uint32_t buf) {
    uint8_t *p = (uint8_t *)(uintptr_t)buf;  /* wasi fdstat: filetype u8 @0, flags u16 @2, rights @8, @16 */
    uint8_t type;
    if (fd == 1 || fd == 2) {
        type = W_FILETYPE_CHARACTER_DEVICE;
    } else if (fd == PREOPEN_FD) {
        type = W_FILETYPE_DIRECTORY;
    } else if (slot_of(fd)) {
        type = W_FILETYPE_REGULAR_FILE;
    } else {
        return W_EBADF;
    }
    for (int k = 0; k < 24; k++) p[k] = 0;
    p[0] = type;
    put_u64(buf + 8, slot_of(fd) && !slot_of(fd)->writable ? W_RIGHT_FD_READ : ~0ull);
    put_u64(buf + 16, ~0ull);
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_fdstat_set_flags(uint32_t fd, uint32_t flags) {
    (void)fd;
    (void)flags;
    return W_ENOTSUP;
}

int __imported_wasi_snapshot_preview1_fd_prestat_get(uint32_t fd, uint32_t buf) {
    if (fd != PREOPEN_FD) return W_EBADF;  /* the end of the prestat list */
    *(uint8_t *)(uintptr_t)buf = W_PREOPENTYPE_DIR;  /* prestat: tag u8 @0, name length u32 @4 */
    put_u32(buf + 4, 1);
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_prestat_dir_name(uint32_t fd, uint32_t path, uint32_t path_len) {
    if (fd != PREOPEN_FD) return W_EBADF;
    if (path_len < 1) return W_EINVAL;
    *(char *)(uintptr_t)path = '/';
    return 0;
}

/* The environment is the host's, read at every call: settings change it through host_putenv,
   and on i686 the C library's getenv sees those changes at once. wasi-libc's getenv would read a
   snapshot instead, and its environ start-up exits with 71 (EX_OSERR) on the ENOSYS below. */
char *getenv(const char *name) { return (char *)host_getenv(name); }

int __imported_wasi_snapshot_preview1_environ_sizes_get(uint32_t count, uint32_t size) {
    (void)count;
    (void)size;
    return W_ENOSYS;
}

int __imported_wasi_snapshot_preview1_environ_get(uint32_t environ, uint32_t buf) {
    (void)environ;
    (void)buf;
    return W_ENOSYS;
}

int __imported_wasi_snapshot_preview1_clock_time_get(uint32_t id, uint64_t precision, uint32_t out) {
    (void)precision;
    if (id == W_CLOCK_MONOTONIC) {
        put_u64(out, (uint64_t)(host_wall_seconds() * 1e9));  /* since the first call, not the epoch */
        return 0;
    }
    if (id == W_CLOCK_REALTIME) return W_ENOTSUP;  /* no host epoch time service */
    return W_EINVAL;
}

int __imported_wasi_snapshot_preview1_path_open(uint32_t dirfd, uint32_t dirflags, uint32_t path, uint32_t path_len,
                                                uint32_t oflags, uint64_t rights_base, uint64_t rights_inheriting,
                                                uint32_t fdflags, uint32_t fd_out) {
    if (dirfd != PREOPEN_FD) return W_EBADF;  /* the path is a host path */
    (void)dirflags;
    (void)rights_inheriting;
    (void)fdflags;
    if (path_len >= MAX_PATH) return W_EINVAL;
    char name[MAX_PATH];
    const char *src = (const char *)(uintptr_t)path;
    for (uint32_t k = 0; k < path_len; k++) name[k] = src[k];
    name[path_len] = 0;
    int writable = (rights_base & W_RIGHT_FD_WRITE) != 0;
    unsigned mode = writable ? (HOST_FILE_WRITE | ((oflags & (W_OFLAG_CREAT | W_OFLAG_TRUNC)) ? HOST_FILE_TRUNCATE : 0))
                             : HOST_FILE_READ;
    unsigned slot = 0;
    while (slot < MAX_FILE_FDS && files[slot].file != HOST_FILE_NONE) slot++;
    if (slot == MAX_FILE_FDS) return W_EIO;
    HostFile f = host_file_open(name, mode);
    if (f == HOST_FILE_NONE) return W_ENOENT;
    files[slot].file = f;
    files[slot].pos = 0;
    files[slot].writable = writable;
    put_u32(fd_out, FIRST_FILE_FD + slot);
    return 0;
}

int __imported_wasi_snapshot_preview1_path_remove_directory(uint32_t fd, uint32_t path, uint32_t path_len) {
    (void)fd;
    (void)path;
    (void)path_len;
    return W_ENOTSUP;
}

int __imported_wasi_snapshot_preview1_path_rename(uint32_t fd, uint32_t old_path, uint32_t old_len, uint32_t new_fd,
                                                  uint32_t new_path, uint32_t new_len) {
    (void)fd;
    (void)old_path;
    (void)old_len;
    (void)new_fd;
    (void)new_path;
    (void)new_len;
    return W_ENOTSUP;
}

int __imported_wasi_snapshot_preview1_path_unlink_file(uint32_t fd, uint32_t path, uint32_t path_len) {
    (void)fd;
    (void)path;
    (void)path_len;
    return W_ENOTSUP;
}

void __imported_wasi_snapshot_preview1_proc_exit(uint32_t code) {
    host_exit((int)code);
    __builtin_unreachable();
}
