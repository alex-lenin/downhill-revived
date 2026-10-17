#ifndef DHD_MENU_H
#define DHD_MENU_H
typedef struct MenuItem {
    int x0;
    short x4;
    char x6;
    char x7;
    int x8;
    int xC;
} MenuItem;

typedef struct MenuLink {
    void *vptr;
    int x4;
} MenuLink;

typedef struct SharedMenu {
    int x0;
    void *x4;
    void *x8;
    void *xC;
    void *x10;
    void *x14;
    MenuItem items[64];
    int x418;
    int x41C;
    int x420;
    char pad424[0x434 - 0x424];
    int x434;
    MenuItem x438;
    MenuItem x448;
    char pad458[0x45C - 0x458];
    int x45C;
    int x460;
    int x464;
    int x468;
    int x46C;
    int x470;
    int x474;
    int x478;
    int x47C;
    int x480;
    char pad484[0x4A0 - 0x484];
    int x4A0;
    int x4A4;
    int x4A8;
    int x4AC;
    int x4B0;
    char pad4B4[0x4B8 - 0x4B4];
    char x4B8;
    char x4B9;
    char x4BA;
    char x4BB;
    char x4BC;
    char pad4BD[0x4C0 - 0x4BD];
    MenuLink x4C0;
    MenuLink *x4C8;
    char pad4CC[0x4F4 - 0x4CC];
    int x4F4;
    void *vptr;
} SharedMenu;


typedef char SharedMenu_size_check[sizeof(SharedMenu) == 0x4FC ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
SharedMenu *func_0018D9D0(SharedMenu *);
/* Global UI sounds; no menu object is read by either implementation. */
void PlayMenuSelect(void) __asm__("func_001F7640");
void PlayMenuBack(void) __asm__("func_001F7680");
#if defined(__mips__)
void PlayMenuSelectLegacy(const void *) __asm__("func_001F7640");
void PlayMenuBackLegacy(const void *) __asm__("func_001F7680");
#else
static inline void PlayMenuSelectLegacy(const void *menu) { PlayMenuSelect(); }
static inline void PlayMenuBackLegacy(const void *menu) { PlayMenuBack(); }
#endif
#if defined(__mips__)
void InitializeMenuDiscard(SharedMenu *menu) __asm__("func_0018D9D0");
#else
static inline void InitializeMenuDiscard(SharedMenu *menu) { func_0018D9D0(menu); }
#endif

#ifdef __cplusplus
}
#endif
#endif
