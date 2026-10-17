#ifndef DHD_MENU_ITEMS_H
#define DHD_MENU_ITEMS_H
struct SharedMenu;
typedef struct MenuSelectionState { char pad0[0xC]; int xC, x10; } MenuSelectionState;
#ifdef __cplusplus
extern "C" {
#endif
void func_001848E0(MenuSelectionState *, int index);
int func_001873C0(struct SharedMenu *, int item, int flags);
void func_0018E3A8(struct SharedMenu *, int item, const void *context, int flags);
#if defined(__mips__)
void SetMenuSelectionLegacy(MenuSelectionState *, int, int) __asm__("func_001848E0");
#else
static inline void SetMenuSelectionLegacy(MenuSelectionState *state, int index, int unused) {
    func_001848E0(state, index);
}
#endif
#ifdef __cplusplus
}
#endif
#endif
