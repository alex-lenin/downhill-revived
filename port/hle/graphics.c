/* SDK graphics boundaries that return GS data to the EE. */
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
static int defer = -1;

static void protect_views(uint32_t offset, int protection)
{
    static const uint32_t mirrors[3] = {0, 0x20000000, 0x30000000};
    for (unsigned m = 0; m < 3; m++) {
        host_mem_protect(mirrors[m] | (PS2_RAM_BASE + offset), 4096, protection);
        if (offset >= PS2_LOW_VIEW) host_mem_protect(mirrors[m] | offset, 4096, protection);
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
    if (!guarded_count && !host_service_gs_reads_pending()) return;
    host_service_graphics_wait();
    for (unsigned i = 0; i < guarded_count; i++) protect_views(guarded[i], HOST_MEM_READWRITE);
    guarded_count = 0;
    host_service_gs_settle_reads();
}

/* An access fault on a guarded page (the host calls this on the game thread): settle, and retry. */
static int guard_fault(uint32_t ctx, uint32_t address)
{
    if (!guarded_page(address)) return 0;
    graphics_settle();
    return 1;
}

static int guard(uint32_t destination, size_t bytes)
{
    uint32_t first = (destination & 0x0FFFFFFF) - PS2_RAM_BASE;
    uint32_t last = first + (uint32_t)bytes - 1;
    if (destination < PS2_RAM_BASE || last >= PS2_RAM_SIZE || last < first) return 0;
    for (uint32_t page = first & ~4095u; page <= last; page += 4096) {
        unsigned i = 0;
        while (i < guarded_count && guarded[i] != page) i++;
        if (i < guarded_count) continue;
        if (guarded_count == MAX_GUARDED) return 0;
        guarded[guarded_count++] = page;
        protect_views(page, HOST_MEM_NOACCESS);
    }
    return 1;
}

static int defer_read(uint32_t destination, size_t bytes)
{
    return guard(destination, bytes) && host_service_gs_defer_read_image(destination, bytes) == bytes;
}

/* Both addresses are guest addresses (the game calls this as func_00244228). */
int sceGsExecStoreImage(uint32_t packet, uint32_t destination) RETAIL(func_00244228);
int sceGsExecStoreImage(uint32_t packet, uint32_t destination)
{
    /* The SDK packet is one VIF qword and six GIF qwords. Execute it before
       reading, so pending raster work is visible to depth/occlusion queries. */
    kernel_poll();
    host_service_graphics_wait();
    host_service_vif_feed(1, packet, 28);
    host_service_graphics_wait(); /* A split GS worker must retire this readback setup too. */
    uint64_t size = host_service_gs_register(GS_TRXREG);
    unsigned bits = host_service_gs_bits_per_pixel((unsigned)(host_service_gs_register(GS_BITBLTBUF) >> 24) & 63);
    size_t bytes = (size_t)(size & 4095) * ((size >> 32) & 4095) * bits / 8;
    if (defer < 0) {
        const char *option = getenv("DHD_DEFER_READS");
        defer = !option || strcmp(option, "0");
        if (defer) defer = host_callback_fault_guard(HOST_CALLBACK(guard_fault), 0);
    }
    /* Guarded before it is deferred. A refusal leaves pages guarded: settling releases them. */
    if (!defer || !host_fault_guard_here() || !bytes ||
        !(defer_read(destination, bytes) || (graphics_settle(), defer_read(destination, bytes)))) {
        graphics_settle();
        if (host_service_gs_read_image(destination, bytes) != bytes) return -1;
    }
    /* Retail consumes FINISH and restores the bus and PATH3 direction. */
    host_service_gs_priv_write(GS_CSR, 2);
    *(volatile uint64_t *)GUEST(0x12001000) = host_service_gs_priv_read(GS_CSR);
    const uint32_t unmask = 0x06000000;
    host_service_vif_feed(1, GADDR(&unmask), 1);
    return 0;
}
