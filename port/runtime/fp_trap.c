/* First-fault SSE diagnostics. The linker map is read before traps are armed.
   Host/CRT IEEE operations retry one instruction with exceptions masked;
   single-step then restores the diagnostic mask. Game faults stay fatal. */
#include "fp_trap.h"
#include <windows.h>
#include <xmmintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uintptr_t address; char name[160], object[160]; } Symbol;
static Symbol symbols[12000];
static unsigned count, enabled;
static __declspec(thread) unsigned step_active, step_mxcsr;
enum { TRAP_MASK = 0x680 }; /* invalid, divide by zero, overflow */

static int compare(const void *a, const void *b)
{
    uintptr_t x = ((const Symbol *)a)->address, y = ((const Symbol *)b)->address;
    return (x > y) - (x < y);
}

static const Symbol *symbol_at(uintptr_t address)
{
    unsigned lo = 0, hi = count;
    while (lo < hi) {
        unsigned mid = lo + (hi - lo) / 2;
        if (symbols[mid].address <= address) lo = mid + 1;
        else hi = mid;
    }
    return lo ? &symbols[lo - 1] : NULL;
}

static int game_object(const char *object)
{
    /* Static helpers need coverage too, even without a retail function name. */
    size_t digits = strspn(object, "0123456789abcdefABCDEF");
    return digits == 8 && (!strcmp(object + 8, ".c.o") || !strcmp(object + 8, ".cpp.o"));
}

static LONG WINAPI fault(EXCEPTION_POINTERS *e)
{
    CONTEXT *c = e->ContextRecord;
    uint32_t *mxcsr = (uint32_t *)(c->ExtendedRegisters + 24);
    DWORD code = e->ExceptionRecord->ExceptionCode;
    if (code == EXCEPTION_SINGLE_STEP && step_active) {
        *mxcsr = step_mxcsr & ~63u;
        c->EFlags &= ~0x100u;
        step_active = 0;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    if (code != EXCEPTION_FLT_INVALID_OPERATION && code != EXCEPTION_FLT_DIVIDE_BY_ZERO &&
        code != EXCEPTION_FLT_OVERFLOW && code != 0xC00002B4 /* multiple faults */ &&
        code != 0xC00002B5 /* multiple traps */)
        return EXCEPTION_CONTINUE_SEARCH;
    const Symbol *s = symbol_at(c->Eip);
    extern char __ImageBase[];
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(__ImageBase + ((IMAGE_DOS_HEADER *)__ImageBase)->e_lfanew);
    int in_image = c->Eip >= (uintptr_t)__ImageBase &&
                   c->Eip < (uintptr_t)__ImageBase + nt->OptionalHeader.SizeOfImage;
    int game = in_image && s && (game_object(s->object) || !strncmp(s->name, "_func_", 6) ||
                                !strncmp(s->name, "_rc_", 4) ||
                                strstr(s->object, "ps2asm.c.o") || strstr(s->object, "recomp.c.o"));
    if (!game) {
        /* Do not change host results. Clear accrued flags before retry. */
        step_mxcsr = *mxcsr;
        step_active = 1;
        *mxcsr = (*mxcsr | TRAP_MASK) & ~63u;
        c->EFlags |= 0x100u;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    _mm_setcsr((_mm_getcsr() | 0x1F80u) & ~63u); /* safe CRT diagnostics */
    fprintf(stderr, "FP trap: exception=%08lX EIP=%08lX %s+0x%lX (%s) MXCSR=%08X\n",
            code, c->Eip, s->name, (unsigned long)(c->Eip - s->address), s->object, *mxcsr);
    unsigned char *ip = (unsigned char *)(uintptr_t)c->Eip;
    fprintf(stderr, "  instruction:");
    for (unsigned i = 0; i < 16; i++) fprintf(stderr, " %02X", ip[i]);
    fprintf(stderr, "\n");
    for (unsigned i = 0; i < 8; i++) {
        const uint32_t *v = (const uint32_t *)(c->ExtendedRegisters + 160 + i * 16);
        fprintf(stderr, "  xmm%u: %08X %08X %08X %08X\n", i, v[0], v[1], v[2], v[3]);
    }
    fflush(stderr);
    return EXCEPTION_CONTINUE_SEARCH;
}

void ps2_fp_trap_init(void)
{
    const char *env = getenv("DHD_FP_TRAP");
    if (!env || strcmp(env, "1")) return;
    char path[MAX_PATH], line[1024];
    GetModuleFileNameA(NULL, path, sizeof path);
    char *dot = strrchr(path, '.');
    if (!dot) abort();
    strcpy(dot, ".map");
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "FP trap: missing linker map %s\n", path); abort(); }
    while (fgets(line, sizeof line, f)) {
        unsigned section, offset;
        unsigned long long address;
        char name[160], object[160], flag[16];
        int fields = sscanf(line, " %x:%x %159s %llx %159s", &section, &offset, name,
                            &address, object);
        if (fields == 5 && section == 1) {
            if (!strcmp(object, "f") || !strcmp(object, "i"))
                sscanf(line, " %x:%x %159s %llx %15s %159s", &section, &offset, name,
                       &address, flag, object);
            if (count == sizeof symbols / sizeof *symbols) abort();
            symbols[count].address = (uintptr_t)address;
            strcpy(symbols[count].name, name);
            strcpy(symbols[count++].object, object);
        }
    }
    fclose(f);
    if (!count) { fprintf(stderr, "FP trap: empty linker map\n"); abort(); }
    qsort(symbols, count, sizeof *symbols, compare);
    AddVectoredExceptionHandler(1, fault);
    enabled = 1;
    fprintf(stderr, "FP trap: armed invalid/divide/overflow, %u code symbols\n", count);
}

void ps2_fp_trap_enable(void)
{
    if (enabled) _mm_setcsr(_mm_getcsr() & ~(TRAP_MASK | 63u));
}

void ps2_x87_checkpoint(unsigned site, unsigned after)
{
    /* Host libraries may reset MXCSR masks. Re-arm the opt-in SSE diagnostic
       at game boundaries even when x87 sampling itself is disabled. */
    ps2_fp_trap_enable();
    static int diagnostic = -1;
    if (diagnostic < 0) {
        const char *value = getenv("DHD_X87_CHECK");
        diagnostic = value && !strcmp(value, "1");
        if (diagnostic) fprintf(stderr, "x87 check: enabled, status and tags preserved\n");
    }
    if (!diagnostic) return;
    static unsigned frame, samples;
    if (site == 0x001B4128 && !after) ++frame;
    ++samples;
    unsigned env[7];
    __asm__ volatile("fnstenv %0\n\tfldenv %0" : "=m"(env) : : "memory");
    unsigned sw = env[1] & 0xffff, tags = env[2] & 0xffff;
    if (!(sw & 0x3841) && tags == 0xffff) {
        if (site == 0x001B4128 && !after && !(frame % 300))
            fprintf(stderr, "x87 check: frame=%u samples=%u SW=%04X TAG=%04X clean\n",
                    frame, samples, sw, tags);
        return;
    }
    static unsigned last_frame = ~0u, last_state, last_tags;
    unsigned state = sw & 0x3841;
    if (frame == last_frame && state == last_state && tags == last_tags) return;
    last_frame = frame; last_state = state; last_tags = tags;
    /* CRT diagnostics run on a temporary empty stack; restore every register,
       tag, control bit and sticky fault afterwards. This is not a game repair. */
    unsigned saved[27];
    __asm__ volatile("fnsave %0" : "=m"(saved) : : "memory");
    fprintf(stderr, "x87 fault: frame=%u site=%08X %s SW=%04X TOP=%u TAG=%04X FIP=%08X\n",
            frame, site, after ? "after" : "before",
            sw, (sw >> 11) & 7, tags, env[3]);
    fflush(stderr);
    __asm__ volatile("frstor %0" : : "m"(saved) : "memory");
}
