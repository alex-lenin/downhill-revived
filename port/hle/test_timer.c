#include "timer.h"
#include <assert.h>
#include <stdio.h>
int main(void)
{
    ee_frame_clock_reset(0);
    ee_frame_clock_poll(); ee_frame_clock_advance(100);
    assert(ee_frame_clock_cycles() == 0 && !ee_frame_clock_enabled());
    ee_frame_clock_reset(1);
    for (unsigned i = 0; i < 16; ++i) ee_frame_clock_poll();
    assert(ee_frame_clock_cycles() == UINT64_C(1024) * 16);
    ee_frame_clock_advance(2460057);
    assert(ee_frame_clock_cycles() == 2460057);
    ee_frame_clock_advance(12); /* never moves backwards */
    assert(ee_frame_clock_cycles() == 2460057);
    ee_frame_clock_reset(1);
    assert(ee_frame_clock_cycles() == 0);
    EeTimer timer = {0}; uint32_t registers[16] = {0};
    registers[4] = 0x82;
    ee_timer_update(&timer, registers, 0);
    ee_timer_update(&timer, registers, 256 * 9619 + 128);
    assert(registers[0] == 9619 && timer.remainder == 128);
    registers[0] = 0; /* the engine resets COUNT every frame */
    ee_timer_update(&timer, registers, 256 * 9620);
    assert(registers[0] == 1);
    registers[0] = 65535;
    ee_timer_update(&timer, registers, 256 * 9621);
    assert(registers[0] == 0 && (registers[4] & 0x800));
    registers[4] = 0;
    ee_timer_update(&timer, registers, 9000000);
    assert(registers[0] == 0);
    registers[4] = 0x81;
    ee_timer_update(&timer, registers, 9000000);
    ee_timer_update(&timer, registers, 9000032);
    assert(registers[0] == 2);
    puts("EE bus-clock timer tests passed"); return 0;
}
