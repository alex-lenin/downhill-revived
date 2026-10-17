/* EE counters 0..3. The engine uses Timer 1 at BUS/256 (MODE=0x82) to
   advance its frame number; a static MMIO page freezes menu input polling. */
#include "timer.h"

static int frame_clock_enabled;
static uint64_t frame_clock_cycles;
void ee_frame_clock_reset(int enabled) { frame_clock_enabled = enabled; frame_clock_cycles = 0; }
int ee_frame_clock_enabled(void) { return frame_clock_enabled; }
uint64_t ee_frame_clock_cycles(void) { return frame_clock_cycles; }
void ee_frame_clock_poll(void)
{
    /* 1024 bus cycles (about 7 us) per kernel boundary: deterministic HLE scheduling that keeps
       polling loops moving, not EE instruction cycle accounting. Frames are paced by the vblank
       wait in sceGsSyncV (kernel.c, INTC_STAT), so this only has to stay small: a race frame
       crosses about 400 boundaries (func_0021B8A8's ~375 FlushCache calls), and at the old
       quarter-millisecond that alone overran a field, so the race stepped at 30 fps where
       PCSX2 runs 60. */
    if (frame_clock_enabled) frame_clock_cycles += UINT64_C(1024);
}
void ee_frame_clock_advance(uint64_t deadline)
{
    if (frame_clock_enabled && deadline > frame_clock_cycles) frame_clock_cycles = deadline;
}

void ee_timer_update(EeTimer *timer, volatile uint32_t *reg, uint64_t now)
{
    unsigned mode = reg[4];
    uint64_t elapsed = now - timer->cycle;
    timer->cycle = now;
    if ((mode & 0xFF) != timer->mode) { timer->remainder = 0; elapsed = 0; }
    timer->mode = mode & 0xFF;
    if (!(mode & 0x80) || (mode & 3) == 3 || (mode & 4)) return;
    unsigned divisor = (mode & 3) == 0 ? 1 : (mode & 3) == 1 ? 16 : 256;
    uint64_t ticks = elapsed + timer->remainder;
    timer->remainder = ticks % divisor;
    ticks /= divisor;
    uint64_t count = (reg[0] & 65535) + ticks;
    if (count >= 65536) reg[4] |= 1u << 11;
    reg[0] = (uint32_t)count & 65535;
}

void timer_poll(uint64_t now)
{
    static EeTimer timers[4];
    for (unsigned i = 0; i < 4; i++)
        ee_timer_update(&timers[i], (volatile uint32_t *)(uintptr_t)(0x10000000 + i * 0x800), now);
}
