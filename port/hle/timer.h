#ifndef PORT_EE_TIMER_H
#define PORT_EE_TIMER_H
#include <stdint.h>
typedef struct { uint64_t cycle, remainder; unsigned mode; } EeTimer;
/* Polls bus-clock modes. Registers are COUNT/MODE/COMP/HOLD, 16 bytes apart. */
void ee_timer_update(EeTimer *timer, volatile uint32_t *registers, uint64_t bus_cycle);
void timer_poll(uint64_t bus_cycle);
/* Opt-in deterministic HLE clock: fixed sub-field progress at kernel polls,
   jump to an interrupt deadline when idle. No wall-clock input. */
void ee_frame_clock_reset(int enabled);
int ee_frame_clock_enabled(void);
uint64_t ee_frame_clock_cycles(void);
void ee_frame_clock_poll(void);
void ee_frame_clock_advance(uint64_t deadline);
#endif
