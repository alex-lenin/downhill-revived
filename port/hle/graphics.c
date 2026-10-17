/* SDK graphics boundaries that return GS data to the EE. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "hle.h"
#include "../gs/gs.h"
#include "../runtime/memory.h"

/* Deferred store-image reads (gs_defer_read_image): until the bytes are written, their RAM pages
   are inaccessible in every view of PS2 RAM, so the first access from the game, on any path,
   settles them before it proceeds. Kernel calls on PS2 RAM (file I/O) fail on such pages instead
   of faulting: graphics_settle runs before them. */
enum { MAX_GUARDED = 64 };
static uint32_t guarded[MAX_GUARDED];  /* PS2 RAM offsets of the guarded 4 KB pages */
static unsigned guarded_count;
static DWORD guard_thread;
static int defer = -1;

static void protect_views(uint32_t offset, DWORD protection)
{
    DWORD old;
    static const uint32_t mirrors[3] = {0, 0x20000000, 0x30000000};
    for (unsigned m = 0; m < 3; m++) {
        VirtualProtect((void *)(uintptr_t)(mirrors[m] | (PS2_RAM_BASE + offset)), 4096, protection, &old);
        if (offset >= PS2_LOW_VIEW) VirtualProtect((void *)(uintptr_t)(mirrors[m] | offset), 4096, protection, &old);
    }
}

static int guarded_page(uintptr_t address)
{
    uint32_t offset = (uint32_t)address & 0x0FFFFFFF;
    if (offset >= PS2_RAM_BASE) offset -= PS2_RAM_BASE;
    else if (offset < PS2_LOW_VIEW) return 0;
    if (offset >= PS2_RAM_SIZE) return 0;
    for (unsigned i = 0; i < guarded_count; i++)
        if (guarded[i] == (offset & ~4095u)) return 1;
    return 0;
}

void graphics_settle(void)
{
    if (!guarded_count && !gs_reads_pending(&gs)) return;
    graphics_wait();
    for (unsigned i = 0; i < guarded_count; i++) protect_views(guarded[i], PAGE_READWRITE);
    guarded_count = 0;
    gs_settle_reads(&gs);
}

static LONG CALLBACK guard_fault(EXCEPTION_POINTERS *e)
{
    const EXCEPTION_RECORD *r = e->ExceptionRecord;
    if (r->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || r->NumberParameters < 2 ||
        GetCurrentThreadId() != guard_thread || !guarded_page(r->ExceptionInformation[1]))
        return EXCEPTION_CONTINUE_SEARCH;
    graphics_settle();
    return EXCEPTION_CONTINUE_EXECUTION;
}

static int guard(void *destination, size_t bytes)
{
    uint32_t first = ((uint32_t)(uintptr_t)destination & 0x0FFFFFFF) - PS2_RAM_BASE;
    uint32_t last = first + (uint32_t)bytes - 1;
    if ((uint32_t)(uintptr_t)destination < PS2_RAM_BASE || last >= PS2_RAM_SIZE || last < first) return 0;
    for (uint32_t page = first & ~4095u; page <= last; page += 4096) {
        unsigned i = 0;
        while (i < guarded_count && guarded[i] != page) i++;
        if (i < guarded_count) continue;
        if (guarded_count == MAX_GUARDED) return 0;
        guarded[guarded_count++] = page;
        protect_views(page, PAGE_NOACCESS);
    }
    return 1;
}

static int defer_read(void *destination, size_t bytes)
{
    return guard(destination, bytes) && gs_defer_read_image(&gs, destination, bytes) == bytes;
}

int sceGsExecStoreImage(const uint32_t *packet, void *destination) RETAIL(func_00244228);
int sceGsExecStoreImage(const uint32_t *packet, void *destination)
{
    /* The SDK packet is one VIF qword and six GIF qwords. Execute it before
       reading, so pending raster work is visible to depth/occlusion queries. */
    kernel_poll();
    graphics_wait();
    vif_feed(1, packet, 28);
    graphics_wait(); /* A split GS worker must retire this readback setup too. */
    uint64_t size = gs.reg[GS_TRXREG];
    unsigned bits = gs_bits_per_pixel((unsigned)(gs.reg[GS_BITBLTBUF] >> 24) & 63);
    size_t bytes = (size_t)(size & 4095) * ((size >> 32) & 4095) * bits / 8;
    if (defer < 0) {
        const char *option = getenv("DHD_DEFER_READS");
        defer = !option || strcmp(option, "0");
        if (defer) {
            guard_thread = GetCurrentThreadId();
            defer = AddVectoredExceptionHandler(1, guard_fault) != NULL;
        }
    }
    /* Guarded before it is deferred. A refusal leaves pages guarded: settling releases them. */
    if (!defer || GetCurrentThreadId() != guard_thread || !bytes ||
        !(defer_read(destination, bytes) || (graphics_settle(), defer_read(destination, bytes)))) {
        graphics_settle();
        if (gs_read_image(&gs, destination, bytes) != bytes) return -1;
    }
    /* Retail consumes FINISH and restores the bus and PATH3 direction. */
    gs_priv_write(&gs, GS_CSR, 2);
    *(volatile uint64_t *)(uintptr_t)0x12001000 = gs_priv_read(&gs, GS_CSR);
    const uint32_t unmask = 0x06000000;
    vif_feed(1, &unmask, 1);
    return 0;
}
