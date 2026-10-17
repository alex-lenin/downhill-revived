/* The EE's DMA controller (finding 020). The game starts a transfer by writing a channel's
   registers and setting CHCR.STR. MMIO store helpers start transfers immediately;
   dma_poll at kernel entries also finds any pending channels. Each transfer ends as the DMAC would:
   STR cleared, then the channel's interrupt. Scratchpad transfers copy, VIF transfers go to the
   VIFs (vif.c, normal or source chain mode); GIF transfers go to the GS (PATH3). */
#include "hle.h"

#include <string.h>

#include "ps2asm.h"
#include "../gs/gs.h"

enum {
    DMA_VIF0, DMA_VIF1, DMA_GIF, DMA_FROM_IPU, DMA_TO_IPU, DMA_SIF0, DMA_SIF1, DMA_SIF2,
    DMA_FROM_SPR, DMA_TO_SPR, DMA_CHANNELS
};

static const uint32_t channel_base[DMA_CHANNELS] = {
    0x10008000, 0x10009000, 0x1000A000, 0x1000B000, 0x1000B400,
    0x1000C000, 0x1000C400, 0x1000C800, 0x1000D000, 0x1000D400,
};
static const char *const channel_name[DMA_CHANNELS] = {
    "VIF0", "VIF1", "GIF", "fromIPU", "toIPU", "SIF0", "SIF1", "SIF2", "fromSPR", "toSPR",
};

typedef struct {  /* a channel's registers, 16 bytes apart */
    uint32_t chcr, pad0[3], madr, pad1[3], qwc, pad2[3], tadr, pad3[3];
    uint32_t asr0, pad4[3], asr1, pad5[3], pad6[8], sadr;
} Channel;

enum { CHCR_MODE = 0xC, CHCR_MODE_CHAIN = 0x4, CHCR_STR = 0x100 };

/* Host pointer of a DMA address: RAM (whose host addresses equal the PS2 values the game
   wrote, memory.c) or, with bit 31, the scratchpad. */
static void *dma_ptr(uint32_t addr)
{
    if (addr & 0x80000000)
        return (void *)(uintptr_t)(PS2_SCRATCHPAD + (addr & 0x3FF0));
    return (void *)(uintptr_t)(addr & 0x0FFFFFF0);
}

static void scratchpad_copy(int channel, Channel *c)
{
    if ((c->chcr & CHCR_MODE) == CHCR_MODE_CHAIN) {
        hle_log("dma %s: chain mode not written\n", channel_name[channel]);
        return;
    }
    uint8_t *ram = dma_ptr(c->madr), *spr = (uint8_t *)(uintptr_t)PS2_SCRATCHPAD;
    for (uint32_t q = 0; q < (c->qwc & 0xFFFF); q++) {  /* the scratchpad address wraps */
        uint8_t *s = spr + ((c->sadr + q * 16) & 0x3FF0);
        if (channel == DMA_TO_SPR)
            memcpy(s, ram + q * 16, 16);
        else
            memcpy(ram + q * 16, s, 16);
    }
    c->madr += (c->qwc & 0xFFFF) * 16;
    c->sadr = (c->sadr + (c->qwc & 0xFFFF) * 16) & 0x3FF0;
    c->qwc = 0;
}

enum { TAG_REFE, TAG_CNT, TAG_NEXT, TAG_REF, TAG_REFS, TAG_CALL, TAG_RET, TAG_END };
enum { CHCR_ASP = 0x30, CHCR_TTE = 0x40, CHCR_TIE = 0x80 };

/* A transfer from memory to a VIF: qwc qwords from MADR (normal mode), or the source chain
   from TADR. A chain's tags say where each block is and where the next tag is; with TTE the
   VIF also gets each tag's upper half (two VIF codes). */
static void feed(int unit, const uint32_t *words, uint32_t count)
{
    graphics_feed(unit, words, count);
}

static void transfer_out(Channel *c, int unit)
{
    int guard;
    if ((c->chcr & CHCR_MODE) != CHCR_MODE_CHAIN) {
        feed(unit, dma_ptr(c->madr), (c->qwc & 0xFFFF) * 4);
        c->madr += (c->qwc & 0xFFFF) * 16;
        c->qwc = 0;
        return;
    }
    for (guard = 0; guard < 0x100000; guard++) {
        const uint32_t *tag = dma_ptr(c->tadr);
        uint32_t qwc = tag[0] & 0xFFFF, id = tag[0] >> 28 & 7, addr = tag[1], sp = (c->chcr & CHCR_ASP) >> 4;
        int end = 0;
        c->chcr = (c->chcr & 0xFFFF) | (tag[0] & 0xFFFF0000);  /* the tag's upper bits show in CHCR */
        if (unit != 2 && (c->chcr & CHCR_TTE))
            graphics_feed(unit, tag + 2, 2);
        c->madr = id == TAG_REFE || id == TAG_REF || id == TAG_REFS ? addr : c->tadr + 16;
        feed(unit, dma_ptr(c->madr), qwc * 4);
        c->madr += qwc * 16;
        switch (id) {
        case TAG_REFE: case TAG_END: end = 1; break;
        case TAG_CNT: c->tadr = c->madr; break;
        case TAG_NEXT: c->tadr = addr; break;
        case TAG_REF: case TAG_REFS: c->tadr += 16; break;
        case TAG_CALL:
            if (sp < 2) {
                (&c->asr0)[sp * 4] = c->madr;
                c->chcr = (c->chcr & ~CHCR_ASP) | (sp + 1) << 4;
            }
            c->tadr = addr;
            break;
        case TAG_RET:
            if (sp) {
                c->tadr = (&c->asr0)[(sp - 1) * 4];
                c->chcr = (c->chcr & ~CHCR_ASP) | (sp - 1) << 4;
            } else
                end = 1;
            break;
        }
        if (end || (tag[0] >> 31 && c->chcr & CHCR_TIE))
            break;
    }
    c->qwc = 0;
}

void dma_poll(void)
{
    static uint16_t dropped;
    static int active;
    if (active) return;
    active = 1;
    for (int ch = 0; ch < DMA_CHANNELS; ch++) {
        Channel *c = (Channel *)(uintptr_t)channel_base[ch];
        if (!(c->chcr & CHCR_STR))
            continue;
        if (ch == DMA_FROM_SPR || ch == DMA_TO_SPR)
            scratchpad_copy(ch, c);
        else if (ch == DMA_VIF0 || ch == DMA_VIF1 || ch == DMA_GIF)
            transfer_out(c, ch);
        else if (!(dropped & 1 << ch)) {
            dropped |= (uint16_t)(1 << ch);
            hle_log("dma %s: transfers dropped (no renderer yet)\n", channel_name[ch]);
        }
        c->chcr &= ~CHCR_STR;
        kernel_dmac_interrupt(ch);
    }
    active = 0;
}

/* sceGsSyncPath: waits for the drawing paths (VIF1, GIF, VU1) to go idle; here they finish at
   once. */
int sceGsSyncPath(int mode, int timeout) RETAIL(func_002433A8);
int sceGsSyncPath(int mode, int timeout)
{
    (void)mode, (void)timeout;
    kernel_poll();  /* tight render loops still deliver vsync and window messages */
    graphics_wait();
    return 0;
}

/* Installed by kernel_init. The MMIO store has already reached the mapped page. */
void dma_mmio_write(uintptr_t address, uint64_t value, unsigned size)
{
    if (address == PS2_INTC_STAT) {
        kernel_intc_write((uint32_t)value);
        return;
    }
    if (address >= 0x10003C00 && address < 0x10004000) graphics_wait();
    if (address >= 0x10005000 && address + size <= 0x10005010) {
        graphics_feed(1, (const uint32_t *)address, size / 4);
        return;
    }
    for (unsigned ch = 0; ch < DMA_CHANNELS; ch++)
        if (address == channel_base[ch] && (value & CHCR_STR)) {
            dma_poll();
            return;
        }
    if (address >= 0x12000000 && address < 0x12001100 && !(address & 15)) {
        graphics_wait();
        gs_priv_write(&gs, (unsigned)address, value);
        if (address == 0x12001000)
            *(volatile uint64_t *)address = gs_priv_read(&gs, GS_CSR);
    }
}
