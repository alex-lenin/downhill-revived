#ifndef DHD_FP_TRAP_H
#define DHD_FP_TRAP_H
/* Diagnostic only; disabled unless DHD_FP_TRAP=1. Call on every game fiber. */
void ps2_fp_trap_init(void);
void ps2_fp_trap_enable(void);
/* Observational x87 ABI diagnostics; never clears game status or stack state. */
void ps2_x87_checkpoint(unsigned site, unsigned after);
#endif
