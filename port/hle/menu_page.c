/* Pages in the game's popup menu style (menu_page.h), the way its AUDIO / OTHER page is built
   (build func_0015F0C8, update func_0015E750, vtable D_00284FB8; finding 050):
   - a SharedMenu object (include/menu.h, 0x4FC bytes), constructed by the game (func_0018D9D0),
     here in host memory, with a copy of the base vtable D_00287218 whose build, update, accept,
     back and left/right slots call the page's functions;
   - rows added as strings (func_0018E408), which the game keeps by pointer, so a page rewrites
     a row's text in place as func_0015F4C8 rewrites the volumes;
   - run modally by the popup manager D_003F52D0 (func_00190108: its own frame loop until the
     page closes), which draws, moves the cursor with up/down and plays the sounds. */
#include "hle.h"
#include "menu_page.h"

#include <string.h>

/* The game's popup menus (src/unsorted, finding 050) */
extern char D_003F52D0[];             /* the popup manager: +0x20 its view */
extern uint32_t D_00287218[];         /* SharedMenu's base vtable */
extern char D_0029DE68[];             /* the text table the ids index */
void *func_0018D9D0(void *menu);      /* SharedMenu constructor */
void func_0018E408(void *menu, const char *text, int enabled);  /* add a row */
void func_0018DC20(void *menu);       /* base update: cursor, drawing */
void func_0018E468(void *menu);       /* close (back) */
void func_00190688(void *manager);    /* close (accept) */
void func_00190108(void *manager, void *menu);  /* run modally */
void func_001B5AF0(void *manager, int a, int b);
void func_0023D650(int view, int *width, int *height);
void port_menu_sound(int next);       /* S_NEXT or S_PREVIOUS (patched into 001EB1D8.cpp) */
int func_00220230(int port);          /* the buttons held on a pad (0 when none is there) */

/* A page opened by a press (the X that chose a row of the menu below, or the screen before)
   would take that same press as its own while the button is still down: a page acts on
   buttons only once they have all been released. */
static int buttons_held(void) { return (func_00220230(0) | func_00220230(1)) & 0xFFFF; }

enum { VTABLE_SLOTS = 17, ITEM_SIZE = 16 };
enum { X_CURSOR = 0x0, X_ITEMS = 0x18, X_COUNT = 0x418, X_TITLE = 0x438, X_FOOTER = 0x448, X_ROW_Y = 0x460,
       X_ROW_X = 0x464, X_ROW_STEP = 0x468, X_46C = 0x46C, X_470 = 0x470, X_BACK_BUTTON = 0x4A8,
       X_4BB = 0x4BB, X_4C4 = 0x4C4, X_VTABLE = 0x4F8 };
enum { TEXT_PREV_NEXT = 0xA6 };       /* "<- PREV -> NEXT /\ BACK X SELECT" */

typedef struct {                      /* MenuItem (include/menu.h) */
    short x, y;
    short id;                         /* text id; 0: no text, unless string is set */
    char enabled, placed;             /* placed: x, y fixed, no automatic layout */
    const char *string;               /* drawn instead of the id's text when set */
    void *table;
} Item;

#define MENU(p, field, type) (*(type *)((char *)(p)->menu + (field)))

static void set_text(Item *item, short id, const char *string)
{
    item->id = id;
    item->string = string;
    item->table = D_0029DE68;
}

static void footer(MenuPage *p)
{
    if (p->footer) set_text(&MENU(p, X_FOOTER, Item), 1, p->footer);  /* any id: the string is drawn */
    else set_text(&MENU(p, X_FOOTER, Item), TEXT_PREV_NEXT, NULL);
}

static void rows(MenuPage *p)
{
    MENU(p, X_COUNT, int) = 0;
    p->build(p);
}

/* ---- the vtable's slots ---- */

static void slot_build(void *menu)  /* slot 1, as func_0015F0C8 */
{
    MenuPage *p = menu;
    int width = 0, height = 0;
    func_0023D650(*(int *)(D_003F52D0 + 0x20), &width, &height);
    int half = width >> 1;
    Item *title = &MENU(p, X_TITLE, Item), *foot = &MENU(p, X_FOOTER, Item);
    MENU(p, X_ROW_Y, int) = 0x54;
    MENU(p, X_ROW_STEP, int) = 10;
    MENU(p, X_ROW_X, int) = half - 0x88;
    title->x = (short)half; title->y = 0x40; title->placed = 1;
    foot->x = (short)half; foot->y = 0xCC; foot->placed = 1;
    MENU(p, X_4BB, char) = 1;
    MENU(p, X_46C, int) = 1;
    MENU(p, X_4C4, int) = 0;
    MENU(p, X_BACK_BUTTON, int) = 5;  /* Triangle: back (slot 8) */
    MENU(p, X_470, int) = 0x14;
    set_text(title, 1, p->title);
    footer(p);
    rows(p);
    func_001B5AF0(D_003F52D0, 1, 1);
}

static void slot_update(void *menu)  /* slot 5, as func_0015E750: the page, then the base */
{
    MenuPage *p = menu;
    if (!p->armed && !buttons_held()) p->armed = 1;
    if (p->rebuild) {
        p->rebuild = 0;
        rows(p);
        menu_page_set_cursor(p, p->rebuild_row);
    }
    if (p->update) p->update(p, menu_page_cursor(p));
    set_text(&MENU(p, X_TITLE, Item), 1, p->title);
    footer(p);
    func_0018DC20(menu);
}

static void slot_accept(void *menu, int index)  /* slot 7, Cross */
{
    MenuPage *p = menu;
    if (!p->armed) return;
    if (p->accept) p->accept(p, index);
    else menu_page_close(p);
}

static void slot_back(void *menu)  /* slot 8, Triangle */
{
    MenuPage *p = menu;
    if (!p->armed) return;
    if (p->back) p->back(p);
    else menu_page_cancel(p);
}

static void slot_left(void *menu, int index)  /* slot 9 */
{
    MenuPage *p = menu;
    if (p->armed && p->left) p->left(p, index);
}

static void slot_right(void *menu, int index)  /* slot 10 */
{
    MenuPage *p = menu;
    if (p->armed && p->right) p->right(p, index);
}

/* ---- the page's calls ---- */

void menu_page_run(MenuPage *p)
{
    p->armed = 0;
    if (!p->constructed) {
        memcpy(p->vtable, D_00287218, sizeof p->vtable);  /* {delta, function} pairs, slot 1 on */
        p->vtable[1 * 2 + 1] = (uint32_t)(uintptr_t)slot_build;
        p->vtable[5 * 2 + 1] = (uint32_t)(uintptr_t)slot_update;
        p->vtable[7 * 2 + 1] = (uint32_t)(uintptr_t)slot_accept;
        p->vtable[8 * 2 + 1] = (uint32_t)(uintptr_t)slot_back;
        p->vtable[9 * 2 + 1] = (uint32_t)(uintptr_t)slot_left;
        p->vtable[10 * 2 + 1] = (uint32_t)(uintptr_t)slot_right;
        func_0018D9D0(p->menu);
        MENU(p, X_VTABLE, uint32_t *) = p->vtable;
        p->constructed = 1;
    } else
        menu_page_rebuild(p);  /* the rows as they are now, should the game keep the old ones */
    func_00190108(D_003F52D0, p->menu);
}

void menu_page_close(MenuPage *p) { (void)p; func_00190688(D_003F52D0); }
void menu_page_cancel(MenuPage *p) { func_0018E468(p->menu); }
void menu_page_row(MenuPage *p, const char *text, int enabled) { func_0018E408(p->menu, text, enabled); }
void menu_page_rebuild(MenuPage *p) { menu_page_rebuild_at(p, 0); }
void menu_page_rebuild_at(MenuPage *p, int row) { p->rebuild = 1, p->rebuild_row = row; }
int menu_page_cursor(const MenuPage *p) { return MENU(p, X_CURSOR, int); }
int menu_page_rows(const MenuPage *p) { return MENU(p, X_COUNT, int); }
void menu_page_sound(int next) { port_menu_sound(next); }

void menu_page_enable(MenuPage *p, int row, int enabled)
{
    if (row >= 0 && row < menu_page_rows(p)) ((Item *)((char *)p->menu + X_ITEMS))[row].enabled = (char)(enabled != 0);
}

/* On the row, else on the first enabled one after it (the game skips disabled rows too). */
void menu_page_set_cursor(MenuPage *p, int row)
{
    int n = menu_page_rows(p);
    if (n <= 0) { MENU(p, X_CURSOR, int) = 0; return; }
    if (row < 0) row = 0;
    if (row >= n) row = n - 1;
    for (int k = 0; k < n; k++) {
        int i = (row + k) % n;
        if (((Item *)((char *)p->menu + X_ITEMS))[i].enabled) { MENU(p, X_CURSOR, int) = i; return; }
    }
    MENU(p, X_CURSOR, int) = row;
}
