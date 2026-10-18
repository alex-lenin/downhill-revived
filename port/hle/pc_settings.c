/* PC SETTINGS: the page the front end's OPTIONS screen opens (item 9, port/patches.py patches
   func_001F0160), a page in the game's own popup menu style (menu_page.c, finding 050) as its
   AUDIO / OTHER page is; up/down, the cursor, the font, the panel and the sounds are the game's:
   - left/right change the row under the cursor; Cross (SELECT) keeps the changes and closes,
     Triangle (BACK) puts the values the page opened with back and closes, as AUDIO / OTHER does.
   Every change is saved to dhd.ini at once (host_setting_save keeps the other lines). Display,
   window size, picture shape, scaling, skip movies (the startup movies) and vibration apply at
   once; resolution, widescreen, the texture pack and the font at
   the next start ("*" and the footer say so). A setting the environment
   sets (DHD_*) is shown but can't be changed here. Nothing here writes game state: the page
   lives in host memory and the settings in dhd.ini, never in the memory card's save. */
#include "hle.h"
#include "menu_page.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gs_window_apply(void);           /* port/gs/present.c */
void gs_present_options_apply(void);
int texpack_available(void);          /* port/gs/texpack.c */

enum { ROWS = 11 };

typedef struct { const char *text, *value; } Choice;
typedef struct {
    const char *label, *key, *env, *fallback;
    const Choice *choices;
    int count, live;
    int selected, running, opened;    /* choice indexes; -1: a value not in the list */
    char text[64];
} Row;

static const Choice scales[] = {{"1x", "1"}, {"2x", "2"}, {"3x", "3"}, {"4x", "4"}, {"Window", "window"}};
static const Choice displays[] = {{"Window", "windowed"}, {"Borderless", "borderless"}, {"Fullscreen", "fullscreen"}};
static const Choice sizes[] = {{"896x672", "896x672"}, {"1280x960", "1280x960"}, {"1440x1080", "1440x1080"},
                               {"1280x720", "1280x720"}, {"1920x1080", "1920x1080"}};
/* Wide: widescreen's shape (the monitor's, port/hle/widescreen.c); 4:3 with widescreen off. */
static const Choice shapes[] = {{"4:3", "4:3"}, {"Wide", "wide"}, {"16:9", "16:9"}, {"Stretch", "stretch"}, {"Pixels", "pixels"}};
static const Choice filters[] = {{"Smooth", "linear"}, {"Sharp", "point"}};
static const Choice switches[] = {{"OFF", "0"}, {"ON", "1"}};
static const Choice toggles[] = {{"OFF", "off"}, {"ON", "on"}};
static const Choice fonts[] = {{"Original", "original"}, {"Sharp", "sharp"}};
static const Choice rates[] = {{"60", "60"}, {"120", "120"}, {"144", "144"}, {"Monitor", "monitor"}};

#define CHOICES(c) c, (int)(sizeof c / sizeof *c)
enum { RESOLUTION, DISPLAY, SIZE, SHAPE, SCALING, WIDESCREEN, MOVIES, VIBRATION, TEXTURES, FONT };
static Row rows[ROWS] = {
    {"Resolution:", "scale", "DHD_SCALE", "2", CHOICES(scales), 0},
    {"Display:", "window", "DHD_WINDOW", "windowed", CHOICES(displays), 1},
    {"Window Size:", NULL, "DHD_WINDOW_WIDTH", "896x672", CHOICES(sizes), 1},  /* width and height */
    {"Picture:", "DHD_ASPECT", "DHD_ASPECT", "4:3", CHOICES(shapes), 1},
    {"Scaling:", "DHD_PRESENT_FILTER", "DHD_PRESENT_FILTER", "linear", CHOICES(filters), 1},
    {"Widescreen:", "widescreen", "DHD_WIDESCREEN", "0", CHOICES(switches), 0},
    {"Skip Movies:", "skip_movies", "DHD_SKIP_MOVIES", "0", CHOICES(switches), 1},  /* the startup movies */
    {"Vibration:", "vibration", "DHD_VIBRATION", "1", CHOICES(switches), 1},
    {"Texture Pack:", "textures", "DHD_TEXTURES", "on", CHOICES(toggles), 0},  /* port/gs/texpack.c */
    {"Font:", "font", "DHD_FONT", "sharp", CHOICES(fonts), 0},  /* port/gs/font_sharp.c */
    {"Frame Rate:", "fps", "DHD_FPS", "60", CHOICES(rates), 0},
};

static MenuPage page;
static int loaded;
static char footer[64];

/* The value the program runs with now: the variable (set from dhd.ini or the environment). */
static void current(const Row *r, char *out, size_t size)
{
    if (r == &rows[SIZE]) {
        const char *w = getenv("DHD_WINDOW_WIDTH"), *h = getenv("DHD_WINDOW_HEIGHT");
        snprintf(out, size, "%dx%d", w && atoi(w) > 0 ? atoi(w) : 896, h && atoi(h) > 0 ? atoi(h) : 672);
        return;
    }
    const char *v = getenv(r->env);
    snprintf(out, size, "%s", v && *v ? v : r->fallback);
    if (r == &rows[WIDESCREEN] || r == &rows[MOVIES] || r == &rows[VIBRATION])
        snprintf(out, size, "%d", atoi(out) != 0);
    if (r == &rows[TEXTURES])
        snprintf(out, size, "%s", v && (!_stricmp(v, "off") || !strcmp(v, "0") || !_stricmp(v, "false") || !_stricmp(v, "no"))
                 ? "off" : "on");
    if (r == &rows[FONT])  /* as font_sharp_on reads it */
        snprintf(out, size, "%s", v && (!_stricmp(v, "original") || !_stricmp(v, "off") || !strcmp(v, "0")) ? "original" : "sharp");
}

static int find(const Row *r, const char *value)
{
    for (int i = 0; i < r->count; i++)
        if (!_stricmp(r->choices[i].value, value)) return i;
    return -1;
}

static int locked(const Row *r)
{
    return host_setting_locked(r->env) || (r == &rows[SIZE] && host_setting_locked("DHD_WINDOW_HEIGHT"));
}

static void format(Row *r)
{
    char value[32];
    if (r->selected >= 0) snprintf(value, sizeof value, "%s", r->choices[r->selected].text);
    else current(r, value, sizeof value);
    /* the game's own layout: the label padded, two tabs, the value ("Music Volume: \t\t1024") */
    snprintf(r->text, sizeof r->text, "%-14s\t\t%s%s", r->label, value, r->selected != r->running ? " *" : "");
}

/* Saves the row's choice and, for a live row, applies it. */
static void commit(Row *r, int apply)
{
    const Choice *c = &r->choices[r->selected];
    if (r == &rows[SIZE]) {
        char w[16], h[16];
        sscanf(c->value, "%15[0-9]x%15[0-9]", w, h);
        host_setting_save("width", w);
        host_setting_save("height", h);
        if (apply) { _putenv_s("DHD_WINDOW_WIDTH", w); _putenv_s("DHD_WINDOW_HEIGHT", h); }
    } else {
        host_setting_save(r->key, c->value);
        if (apply) _putenv_s(r->env, c->value);
    }
    if (!apply) return;
    r->running = r->selected;
    if (r == &rows[DISPLAY] || r == &rows[SIZE]) gs_window_apply();
    else if (r == &rows[SHAPE] || r == &rows[SCALING]) gs_present_options_apply();
    else if (r == &rows[VIBRATION]) host_pad_vibrate(0, 0, 0);
}

static void change(int index, int step)
{
    if (index < 0 || index >= ROWS) return;
    Row *r = &rows[index];
    if (locked(r)) return;
    r->selected = r->selected < 0 ? (step > 0 ? 0 : r->count - 1) : (r->selected + step + r->count) % r->count;
    menu_page_sound(step > 0);
    commit(r, r->live);
    /* Widescreen brings its picture shape (the monitor's, "wide"); saved for the next start,
       when the wider view comes, not shown stretched now. */
    if (r == &rows[WIDESCREEN] && !locked(&rows[SHAPE])) {
        rows[SHAPE].selected = find(&rows[SHAPE], r->selected ? "wide" : "4:3");
        commit(&rows[SHAPE], 0);
        format(&rows[SHAPE]);
    }
    format(r);
    hle_log("pc settings: %s %s\n", r->label, r->choices[r->selected].value);
}

/* ---- the page ---- */

static void page_build(MenuPage *p)
{
    for (int i = 0; i < ROWS; i++) {
        format(&rows[i]);
        menu_page_row(p, rows[i].text, 1);
    }
}

static void page_update(MenuPage *p, int cursor)  /* the footer for the row */
{
    const Row *r = cursor >= 0 && cursor < ROWS ? &rows[cursor] : NULL;
    p->footer = NULL;
    if (r && locked(r)) {
        snprintf(footer, sizeof footer, "Set by %s outside the game", r == &rows[SIZE] ? "DHD_WINDOW_WIDTH" : r->env);
        p->footer = footer;
    } else if (r && r->selected != r->running) p->footer = "* Applies when the game starts again";
    else if (r == &rows[TEXTURES] && !texpack_available()) p->footer = "No pack: see texture_pack in dhd.ini";
}

static void page_back(MenuPage *p)  /* Triangle: the values the page opened with, close */
{
    for (int i = 0; i < ROWS; i++) {
        Row *r = &rows[i];
        if (r->selected == r->opened || r->opened < 0) continue;
        r->selected = r->opened;
        commit(r, r->live || r->opened == r->running);
    }
    menu_page_cancel(p);
}

static void page_left(MenuPage *p, int index) { (void)p; change(index, -1); }
static void page_right(MenuPage *p, int index) { (void)p; change(index, 1); }

/* OPTIONS, item 9 (patched func_001F0160): run the page until it closes. */
void port_pc_settings(void)
{
    page.title = "PC SETTINGS";
    page.build = page_build;
    page.update = page_update;
    page.accept = NULL;  /* Cross: keep, close */
    page.back = page_back;
    page.left = page_left;
    page.right = page_right;
    if (!loaded) {  /* what runs now; restart rows keep their pending choice across visits */
        char value[32];
        for (int i = 0; i < ROWS; i++) {
            current(&rows[i], value, sizeof value);
            rows[i].selected = rows[i].running = find(&rows[i], value);
        }
        loaded = 1;
    }
    for (int i = 0; i < ROWS; i++) rows[i].opened = rows[i].selected;
    menu_page_run(&page);
}

/* OPTIONS' rows (patched func_001F0160): row 9 is this page; the game's table has an unused
   "UNLOCK RIDERS/BIKES(test)" there, which stays as it is. */
char *port_options_label(char *label, int index)
{
    return index == 9 ? (char *)"PC SETTINGS" : label;
}
