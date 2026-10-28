/* The x64 side of a guest test program (port/tools/testprog.py): a test's C, compiled for wasm32
   as the core is and translated by wasm2c (module name "guest"), runs here. As in the core, the
   program's C library reaches the host through runtime/wasi_adapter.c, which turns its WASI calls
   into host_api.h services; those services are here (console, files, environment, clock, exit, the
   float control word), with the two WASI calls the adapter leaves to the runtime: the program's
   arguments (main's argv) and random bytes (the stack protector's canary).
   A trap (assert's abort, an unreachable, an out-of-bounds access) ends the run with exit code 134. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "guest.h"
#include "wasm-rt-impl.h"
#if defined(__x86_64__) || defined(_M_X64)
#include <xmmintrin.h>
#endif
#ifdef _WIN32
#define fseek64 _fseeki64
#define ftell64 _ftelli64
#else
#define fseek64 fseeko
#define ftell64 ftello
#endif

struct w2c_wasi__snapshot__preview1 { int unused; };
struct w2c_env { int unused; };
static w2c_guest guest;
static struct w2c_wasi__snapshot__preview1 wasi;
static struct w2c_env env;
static wasm_rt_memory_t *memory;
static int arg_count;
static char **args;

static u8 *at(u32 address, u32 size)
{
    if ((u64)address + size > memory->size) {
        fprintf(stderr, "guest: host call outside memory (0x%08x+%u)\n", address, size);
        exit(134);
    }
    return memory->data + address;
}

static const char *string(u32 address)
{
    const char *s = (const char *)at(address, 1);
    if (!memchr(s, 0, memory->size - address)) {
        fprintf(stderr, "guest: unterminated string at 0x%08x\n", address);
        exit(134);
    }
    return s;
}

/* ---- WASI: arguments and random bytes ---- */

static void put32(u32 address, u32 value) { memcpy(at(address, 4), &value, 4); }

u32 w2c_wasi__snapshot__preview1_args_sizes_get(struct w2c_wasi__snapshot__preview1 *w, u32 count, u32 size)
{
    u32 total = 0;
    (void)w;
    for (int i = 0; i < arg_count; i++)
        total += (u32)strlen(args[i]) + 1;
    put32(count, (u32)arg_count);
    put32(size, total);
    return 0;
}

u32 w2c_wasi__snapshot__preview1_args_get(struct w2c_wasi__snapshot__preview1 *w, u32 pointers, u32 buffer)
{
    (void)w;
    for (int i = 0; i < arg_count; i++) {
        u32 len = (u32)strlen(args[i]) + 1;
        put32(pointers + 4 * i, buffer);
        memcpy(at(buffer, len), args[i], len);
        buffer += len;
    }
    return 0;
}

u32 w2c_wasi__snapshot__preview1_random_get(struct w2c_wasi__snapshot__preview1 *w, u32 buffer, u32 size)
{
    (void)w;
    u8 *p = at(buffer, size);
    for (u32 i = 0; i < size; i++)
        p[i] = (u8)(0xA5 ^ (i * 29));  /* any bytes but zeros */
    return 0;
}

/* ---- host_api.h ---- */

void w2c_env_host_console_write(struct w2c_env *e, u32 address, u32 bytes)
{
    (void)e;
    fwrite(at(address, bytes), 1, bytes, stdout);
    fflush(stdout);
}

void w2c_env_host_exit(struct w2c_env *e, u32 code)
{
    (void)e;
    fflush(stdout);
    exit((int)code);
}

/* The host's environment, read at every call, into the guest's buffer (testprog.GUEST_API: the
   game's host keeps such values in its own arena). 1 when the variable is set. */
u32 w2c_env_test_host_getenv(struct w2c_env *e, u32 name, u32 buffer, u32 size)
{
    (void)e;
    const char *value = getenv(string(name));
    if (!value)
        return 0;
    size_t n = strlen(value) + 1;
    if (n > size) {
        fprintf(stderr, "guest: %s is longer than %u bytes\n", string(name), size);
        exit(134);
    }
    memcpy(at(buffer, (u32)n), value, n);
    return 1;
}

void w2c_env_host_putenv_s(struct w2c_env *e, u32 name, u32 value)
{
    (void)e;
#ifdef _WIN32
    _putenv_s(string(name), string(value));
#else
    if (*string(value)) setenv(string(name), string(value), 1);
    else unsetenv(string(name));
#endif
}

void w2c_env_host_putenv(struct w2c_env *e, u32 text)
{
    (void)e;
    const char *s = string(text), *eq = strchr(s, '=');
    char name[256];
    if (!eq || (size_t)(eq - s) >= sizeof name)
        return;
    memcpy(name, s, (size_t)(eq - s));
    name[eq - s] = 0;
#ifdef _WIN32
    _putenv_s(name, eq + 1);
#else
    if (eq[1]) setenv(name, eq + 1, 1);
    else unsetenv(name);
#endif
}

f64 w2c_env_host_wall_seconds(struct w2c_env *e)
{
    static struct timespec start;
    struct timespec t;
    (void)e;
    timespec_get(&t, TIME_UTC);
    if (!start.tv_sec && !start.tv_nsec)
        start = t;
    return (double)(t.tv_sec - start.tv_sec) + (t.tv_nsec - start.tv_nsec) * 1e-9;
}

u32 w2c_env_host_fp_csr(struct w2c_env *e)
{
    (void)e;
#if defined(__x86_64__) || defined(_M_X64)
    return _mm_getcsr();
#else
    return 0;  /* no MXCSR: round to nearest (host_api.h) */
#endif
}

void w2c_env_host_fp_set_csr(struct w2c_env *e, u32 csr)
{
    (void)e; (void)csr;
#if defined(__x86_64__) || defined(_M_X64)
    _mm_setcsr(csr);
#endif
}

u64 w2c_env_host_cycles(struct w2c_env *e) { (void)e; return 0; }  /* timing diagnostics only */

/* VU1's registers as VU0 sees them at 0x4000: a test program has no VU1, so zeros */
void w2c_env_host_vu1_file(struct w2c_env *e, u32 address) { (void)e; memset(at(address, 64 * 16), 0, 64 * 16); }

/* Files: the host's, by the path the guest names (relative to the program's folder). */
static FILE *files[64];
static FILE *file(u32 f) { return f && f < 64 ? files[f] : NULL; }

u32 w2c_env_host_file_open(struct w2c_env *e, u32 path, u32 mode)
{
    (void)e;
    if (mode & 8)  /* HOST_FILE_DISCARD: none here */
        return 0;
    for (u32 i = 1; i < 64; i++)
        if (!files[i]) {
            files[i] = fopen(string(path), mode & 4 ? "w+b" : mode & 2 ? "r+b" : "rb");
            return files[i] ? i : 0;
        }
    return 0;
}

void w2c_env_host_file_close(struct w2c_env *e, u32 f)
{
    (void)e;
    if (file(f)) {
        fclose(files[f]);
        files[f] = NULL;
    }
}

u64 w2c_env_host_file_read(struct w2c_env *e, u32 f, u32 dst, u32 size)
{ (void)e; return file(f) ? (u64)fread(at(dst, size), 1, size, files[f]) : (u64)-1; }
u32 w2c_env_host_file_write(struct w2c_env *e, u32 f, u32 src, u32 size)
{ (void)e; return file(f) ? (u32)fwrite(at(src, size), 1, size, files[f]) : 0; }
u32 w2c_env_host_file_flush(struct w2c_env *e, u32 f) { (void)e; return file(f) ? (u32)fflush(files[f]) : 1; }
u32 w2c_env_host_file_seek(struct w2c_env *e, u32 f, u64 offset)
{ (void)e; return file(f) ? (u32)!!fseek64(files[f], (long long)offset, SEEK_SET) : 1; }

u64 w2c_env_host_file_size(struct w2c_env *e, u32 f)
{
    (void)e;
    if (!file(f))
        return 0;
    long long now = ftell64(files[f]);
    fseek64(files[f], 0, SEEK_END);
    long long size = ftell64(files[f]);
    fseek64(files[f], now, SEEK_SET);
    return (u64)size;
}

int main(int argc, char **argv)
{
    arg_count = argc;
    args = argv;
    wasm_rt_init();
    wasm2c_guest_instantiate(&guest, GUEST_IMPORTS);
    memory = w2c_guest_memory(&guest);
    wasm_rt_trap_t trap = WASM_RT_SETJMP(g_wasm_rt_jmp_buf);
    if (trap) {
        fflush(stdout);
        fprintf(stderr, "guest: trap: %s\n", wasm_rt_strerror(trap));
        return 134;
    }
    w2c_guest_0x5Fstart(&guest);  /* returns only when main returned 0 */
    fflush(stdout);
    return 0;
}
