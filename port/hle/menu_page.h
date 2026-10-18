/* A page in the game's own popup menu style, run by the game's popup code (finding 050): a
   SharedMenu (include/menu.h) in host memory with a copy of the base vtable D_00287218 whose
   build, update, accept, back and left/right slots call the page's functions below. The game
   draws the panel, rows, cursor, title and footer in its font, moves the cursor with up/down
   and plays its sounds; the page only says what the rows are and what the buttons do.
   PC SETTINGS (pc_settings.c) and the online screens (online_menu.c) are such pages. */
#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include <stdint.h>

/* The legend's button glyphs in the game's font (text id 0xA6 "\x8D PREV \x8F NEXT ...") */
#define GLYPH_LEFT "\x8D"
#define GLYPH_RIGHT "\x8F"
#define GLYPH_TRIANGLE "\xA7"
#define GLYPH_CROSS "\xAD"

typedef struct MenuPage MenuPage;
struct MenuPage {
    uint32_t menu[0x600 / 4];  /* the SharedMenu; first, so the game's `this` is the page */
    uint32_t vtable[17 * 2];
    int constructed;
    const char *title;
    const char *footer;        /* NULL: the game's PREV / NEXT / BACK / SELECT legend */
    void (*build)(MenuPage *); /* add the rows (menu_page_row); also after menu_page_rebuild */
    void (*update)(MenuPage *, int cursor);  /* every frame, before the game's update */
    void (*accept)(MenuPage *, int row);     /* Cross */
    void (*back)(MenuPage *);                /* Triangle; NULL: close the page */
    void (*left)(MenuPage *, int row), (*right)(MenuPage *, int row);
    int rebuild;               /* set by menu_page_rebuild: rows again at the next update */
    int rebuild_row;           /* and the cursor's row then */
    int armed;                 /* the buttons were all released since the page opened */
} __attribute__((aligned(16)));  /* the game stores a u128 at +0x490 with aligned SSE moves */

void menu_page_run(MenuPage *p);       /* modally, until it closes */
void menu_page_close(MenuPage *p);     /* as the game closes a page on Cross */
void menu_page_cancel(MenuPage *p);    /* as it closes one on Triangle */
void menu_page_row(MenuPage *p, const char *text, int enabled);  /* text: kept, not copied */
void menu_page_rebuild(MenuPage *p);   /* the rows change: build again, cursor on the first */
void menu_page_rebuild_at(MenuPage *p, int row);  /* the same, the cursor on that row */
int menu_page_cursor(const MenuPage *p);
void menu_page_set_cursor(MenuPage *p, int row);
int menu_page_rows(const MenuPage *p);
void menu_page_enable(MenuPage *p, int row, int enabled);  /* a row's flag, without building again */
void menu_page_sound(int next);        /* S_NEXT / S_PREVIOUS, as the value rows play them */

#endif
