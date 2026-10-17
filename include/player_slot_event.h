#ifndef DHD_PLAYER_SLOT_EVENT_H
#define DHD_PLAYER_SLOT_EVENT_H
typedef struct PlayerSlotEvent {
    char pad0[0x12A4];
    int x12A4;
    int x12A8;
    int x12AC;
    int x12B0;
    char pad12B4[0x4];
    int x12B8;
    int x12BC;
    int x12C0;
} PlayerSlotEvent;
#ifdef __cplusplus
extern "C" {
#endif
void func_001790E8(PlayerSlotEvent *, int, int, int, int, int);
#if defined(__mips__)
void SetPlayerSlotEventLegacy(void *, int, int, int, int, int, int) __asm__("func_001790E8");
#else
static inline void SetPlayerSlotEventLegacy(void *slot, int a, int b, int c, int d, int e, int unused) { func_001790E8((PlayerSlotEvent *)slot,a,b,c,d,e); }
#endif
#ifdef __cplusplus
}
#endif
#endif
