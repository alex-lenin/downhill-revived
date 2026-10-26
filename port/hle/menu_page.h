/* The port's own screens (finding 092): one small layer over the game's popup menus (finding 050).
   A page is a SharedMenu (include/menu.h) in host memory with a copy of the base vtable D_00287218
   whose build, update, accept, back and left/right slots call the page's functions below. The
   game draws the panel, the rows, the highlight, the title and the footer in its own font and
   colours, moves the cursor with up/down and plays its sounds; the page says what its rows are and
   what the buttons do. Every page is laid out as the game's centred popups are (the pause menu):
   the title, then the rows centred under it, then the footer.

   Rows, in order (each call returns the row's index):
   - menu_page_row: a list row or a button; Cross acts on it (accept);
   - menu_page_value: a value picker, "Label:  < Value >"; left/right change it (and Cross, as
     the page decides); arrows 0: shown without them (it can't be changed);
   - menu_page_text: a line to read, never selected.
   The footer (p->footer) says what to do; NULL shows the game's PREV / NEXT / BACK / SELECT legend.

   The mouse (menu_mouse_*, below; any of the game's popups gets the same): the row under the
   pointer is selected when the mouse moves, a left click is Cross (on a value picker: its arrows,
   else the next value), a right click is Triangle, the wheel moves the cursor or changes the value.
   PC SETTINGS (pc_settings.c), EXIT GAME (exit_menu.c) and the online pages (online_menu.c) are
   such pages. */
#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include <stdint.h>

/* The legend's button glyphs in the game's font (text id 0xA6 "\x8D PREV \x8F NEXT ...") */
#define GLYPH_LEFT "\x8D"
#define GLYPH_RIGHT "\x8F"
#define GLYPH_TRIANGLE "\xA7"
#define GLYPH_CROSS "\xAD"

enum { MENU_PAGE_ROWS = 24, MENU_PAGE_TEXT = 80 };
enum { MENU_ROW_BUTTON, MENU_ROW_VALUE, MENU_ROW_TEXT };

typedef struct MenuPage MenuPage;
struct MenuPage {
    uint32_t menu[0x600 / 4];  /* the SharedMenu; first, so the game's `this` is the page */
    uint32_t vtable[17 * 2];
    int constructed;
    const char *title;
    const char *footer;        /* NULL: the game's PREV / NEXT / BACK / SELECT legend */
    void (*build)(MenuPage *); /* add the rows (menu_page_row ...); also after menu_page_rebuild */
    void (*update)(MenuPage *, int cursor);  /* every frame, before the game's update */
    void (*accept)(MenuPage *, int row);     /* Cross */
    void (*back)(MenuPage *);                /* Triangle; NULL: close the page */
    void (*left)(MenuPage *, int row), (*right)(MenuPage *, int row);
    int rebuild;               /* set by menu_page_rebuild: rows again at the next update */
    int rebuild_row;           /* and the cursor's row then */
    int armed;                 /* the buttons were all released since the page opened */
    unsigned char kind[MENU_PAGE_ROWS];     /* MENU_ROW_* of each row */
    char text[MENU_PAGE_ROWS][MENU_PAGE_TEXT];  /* the value rows' text */
} __attribute__((aligned(16)));  /* the game stores a u128 at +0x490 with aligned SSE moves */

void menu_page_run(MenuPage *p);       /* modally, until it closes */
void menu_page_close(MenuPage *p);     /* as the game closes a page on Cross */
void menu_page_cancel(MenuPage *p);    /* as it closes one on Triangle */
int menu_page_row(MenuPage *p, const char *text, int enabled);  /* text: kept, not copied */
int menu_page_value(MenuPage *p, const char *label, const char *value, int arrows);  /* copied */
int menu_page_text(MenuPage *p, const char *text);  /* kept, not copied */
/* A value row's label and value again (and whether it shows its arrows), in place. */
void menu_page_value_set(MenuPage *p, int row, const char *label, const char *value, int arrows);
void menu_page_rebuild(MenuPage *p);   /* the rows change: build again, cursor on the first */
void menu_page_rebuild_at(MenuPage *p, int row);  /* the same, the cursor on that row */
int menu_page_cursor(const MenuPage *p);
void menu_page_set_cursor(MenuPage *p, int row);
int menu_page_rows(const MenuPage *p);
void menu_page_enable(MenuPage *p, int row, int enabled);  /* a row's flag, without building again */
void menu_page_sound(int next);        /* S_NEXT / S_PREVIOUS, as the value rows play them */

#endif
