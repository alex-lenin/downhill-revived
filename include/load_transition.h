#ifndef DHD_LOAD_TRANSITION_H
#define DHD_LOAD_TRANSITION_H
typedef struct LoadTransitionState {char x0[9];char pad[3];int xC,x10[4],x20;char pad2[0x18];int x3C,x40,x44,x48,x4C,x50;char x54,x55,x56,pad3;int x58,x5C;char x60,x61;} LoadTransitionState;
typedef char LoadTransitionState_size_check[sizeof(LoadTransitionState) == 0x64 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
LoadTransitionState *func_001B4A60(LoadTransitionState *);
#if defined(__mips__)
void InitializeLoadTransitionDiscard(void *) __asm__("func_001B4A60");
#else
static inline LoadTransitionState *InitializeLoadTransitionDiscard(void *state) { return func_001B4A60((LoadTransitionState *)state); }
#endif
#ifdef __cplusplus
}
#endif
#endif
