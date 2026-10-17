/* The PS2 address space on the PC (port/tools/pctarget.py, finding 020).

   The 32 MB of PS2 RAM are one shared memory block, mapped three times: at PS2_RAM_BASE (cached
   addresses), and at 0x20000000 and 0x30000000 + PS2_RAM_BASE, where the game's uncached
   (address | 0x20000000) and uncached-accelerated (| 0x30000000) pointers land. PS2_RAM_BASE is
   below 0x10000000, so a DMA address (pointer & 0x0FFFFFFF) is the host pointer itself.
   The game's data image is copied in at its PS2 addresses from the player's disc
   (ps2_image_load), with the PC build's changes (port/tools/data.py); .bss is zero.
   The scratchpad (16 KB at 0x70000000) and the EE hardware register pages are plain memory at
   their PS2 addresses, and so is VU memory (0x11000000-0x1100FFFF: VU0's micro and data memory
   at 0x11000000 and 0x11004000, VU1's at 0x11008000 and 0x1100C000), which the game also writes
   through those addresses. */
#include "memory.h"
#include "ps2asm.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

/* {PS2 address, value} of every word of the image that is not the retail one (port/tools/data.py
   fixups.s): pointers, which are host addresses here, and code addresses, which are the
   recompiled functions. */
extern const uint32_t ps2_fixups[][2] __asm__("_ps2_fixups");
extern const uint32_t ps2_fixups_end[][2] __asm__("_ps2_fixups_end");

/* Data words that hold a fixed PS2 RAM address as a plain number: the base of the game's big heap
   (0xA00000, func_0020B628) and the movie player's buffers at the top of RAM (.sdata of
   func_00223BB0's unit). They get PS2_RAM_BASE added after the fixups. */
static const uint32_t ram_address_words[] = {0x29D9D4, 0x29DB0C, 0x29DB10, 0x29DB14, 0x29DB18, 0x29DB1C};

static void *map_at(HANDLE h, uintptr_t at)
{
    return MapViewOfFileEx(h, FILE_MAP_ALL_ACCESS, 0, 0, PS2_RAM_SIZE, (void *)at);
}

/* The low view, at the PS2 addresses themselves: into the parent's reservation. */
static int map_low(HANDLE h, uint32_t mirror)
{
    uintptr_t at = mirror | PS2_LOW_VIEW;
    if (!mirror)
        VirtualFree((void *)at, 0, MEM_RELEASE);
    return MapViewOfFileEx(h, FILE_MAP_ALL_ACCESS, 0, PS2_LOW_VIEW, PS2_RAM_SIZE - PS2_LOW_VIEW, (void *)at)
           == (void *)at;
}

int ps2_low_view_reserved(void)
{
    MEMORY_BASIC_INFORMATION m;
    return VirtualQuery((void *)PS2_LOW_VIEW, &m, sizeof m) && m.State == MEM_RESERVE
           && m.AllocationBase == (void *)PS2_LOW_VIEW && m.RegionSize >= PS2_RAM_SIZE - PS2_LOW_VIEW;
}

static int alloc_at(uintptr_t at, size_t size)
{
    return VirtualAlloc((void *)at, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) == (void *)at;
}

int ps2_memory_init(void)
{
    HANDLE h = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, PS2_RAM_SIZE, NULL);
    if (!h || map_at(h, PS2_RAM_BASE) != (void *)PS2_RAM_BASE
        || map_at(h, 0x20000000 | PS2_RAM_BASE) != (void *)(0x20000000 | PS2_RAM_BASE)
        || map_at(h, 0x30000000 | PS2_RAM_BASE) != (void *)(0x30000000 | PS2_RAM_BASE)) {
        fprintf(stderr, "ps2_memory_init: can't map PS2 RAM at 0x%08X (error %lu)\n", PS2_RAM_BASE,
                GetLastError());
        return 0;
    }
    if (!map_low(h, 0) || !map_low(h, 0x20000000) || !map_low(h, 0x30000000)) {
        fprintf(stderr, "ps2_memory_init: can't map PS2 RAM at its own addresses from 0x%08X (error %lu)\n",
                PS2_LOW_VIEW, GetLastError());
        return 0;
    }
    if (!alloc_at(PS2_SCRATCHPAD, 0x4000) || !alloc_at(0x10000000, 0x10000) || !alloc_at(0x12000000, 0x2000)
        || !alloc_at(0x11000000, 0x10000)) {
        fprintf(stderr, "ps2_memory_init: can't map the scratchpad, VU memory or the hardware registers\n");
        return 0;
    }
    ps2.vu0_code = (uint8_t *)(uintptr_t)0x11000000;
    ps2.vu0_mem = (ps2_q128 *)(uintptr_t)0x11004000;
    return 1;
}

void ps2_image_load(const uint8_t *retail)
{
    memcpy(PS2_PTR(PS2_IMAGE_START), retail, PS2_IMAGE_END - PS2_IMAGE_START);
    for (const uint32_t (*f)[2] = ps2_fixups; f < ps2_fixups_end; f++)
        *(uint32_t *)PS2_PTR((*f)[0]) = (*f)[1];
    for (size_t i = 0; i < sizeof ram_address_words / sizeof *ram_address_words; i++)
        *(uint32_t *)PS2_PTR(ram_address_words[i]) += PS2_RAM_BASE;
    ps2_vu_aot_load(PS2_PTR(PS2_IMAGE_START));
}
