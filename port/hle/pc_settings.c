/* PC SETTINGS: the page the front end's OPTIONS screen opens (item 9, port/patches.py patches
   func_001F0160), a page in the game's own popup menu style (menu_page.c, finding 050) as its
   AUDIO / OTHER page is; up/down, the cursor, the font, the panel and the sounds are the game's:
   - left/right change the row under the cursor; Cross (SELECT) keeps the changes and closes,
     Triangle (BACK) puts the values the page opened with back and closes, as AUDIO / OTHER does.
   Every change is saved to dhd.ini at once (host_setting_save keeps the other lines). Display,
   window size, picture shape, scaling, skip movies (the startup movies), vibration and unlock all
   apply at once; resolution, widescreen, the texture pack, the font, the frame rate and V-Sync at
   the next start ("*" and the footer say so). A setting the environment
   sets (DHD_*) is shown but can't be changed here. Nothing here writes game state: the page
   lives in host memory and the settings in dhd.ini, never in the memory card's save.
   A host that can do only some of the rows says which in DHD_PC_SETTINGS_ROWS, a comma list of
   the rows' variables; unset, every row is shown. A host that applies more rows at once than the
   ones marked live below lists their variables in DHD_PC_SETTINGS_LIVE (the row then sets its
   variable at once and the host follows it), and DHD_PC_SETTINGS_RESTART replaces the footer of
   a row that waits for the next start. The browser sets all three (port/web/www/settings.js);
   x64 sets none of them, so its page is as described above. */
#include "hle.h"
#include "../runtime/portable.h"
#include "menu_page.h"
#include "../include/fps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gs_window_apply(void);           /* port/gs/present.c */
void gs_present_options_apply(void);
int texpack_available(void);          /* port/gs/texpack.c */

enum { ROWS = 13 };

typedef struct { const char *text, *value; } Choice;
typedef struct {
    const char *label, *key, *env, *fallback;
    const Choice *choices;
    int count, live;
    int selected, running, opened;    /* choice indexes; -1: a value not in the list */
} Row;

/* Resolution: 1x up to what the renderer draws (DHD_SCALE_MAX, which the Rust native renderer sets:
   8, or less on a card with little memory; 4 on the others), then Window (choices_build). */
static const Choice scales[] = {{"1x", "1"}, {"2x", "2"}, {"3x", "3"}, {"4x", "4"}, {"5x", "5"}, {"6x", "6"},
                                {"7x", "7"}, {"8x", "8"}};
static Choice scale_list[sizeof scales / sizeof *scales + 1];
static const Choice displays[] = {{"Window", "windowed"}, {"Borderless", "borderless"}, {"Fullscreen", "fullscreen"}};
static const Choice sizes[] = {{"896x672", "896x672"}, {"1280x960", "1280x960"}, {"1440x1080", "1440x1080"},
                               {"1280x720", "1280x720"}, {"1920x1080", "1920x1080"}};
/* Window Size: sizes and the monitor's own (DHD_MONITOR_MODE, present.c) when it isn't one of them. */
static Choice size_list[sizeof sizes / sizeof *sizes + 1];
static char monitor_size[24];
/* Wide: widescreen's shape (the monitor's, port/hle/widescreen.c); 4:3 with widescreen off. */
static const Choice shapes[] = {{"4:3", "4:3"}, {"Wide", "wide"}, {"16:9", "16:9"}, {"Stretch", "stretch"}, {"Pixels", "pixels"}};
static const Choice filters[] = {{"Smooth", "linear"}, {"Sharp", "point"}};
static const Choice switches[] = {{"OFF", "0"}, {"ON", "1"}};
static const Choice toggles[] = {{"OFF", "off"}, {"ON", "on"}};
static const Choice fonts[] = {{"Original", "original"}, {"Sharp", "sharp"}};
/* Frame Rate: the rates up to port_fps_max (240; the browser sets DHD_FPS_MAX 144), then Monitor. */
static const Choice rates[] = {{"60", "60"}, {"120", "120"}, {"144", "144"}, {"165", "165"}, {"240", "240"}};
static Choice rate_list[sizeof rates / sizeof *rates + 1];

#define CHOICES(c) c, (int)(sizeof c / sizeof *c)
enum { RESOLUTION, DISPLAY, SIZE, SHAPE, SCALING, WIDESCREEN, MOVIES, VIBRATION, TEXTURES, FONT, RATE, VSYNC, UNLOCK };
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
    {"V-Sync:", "vsync", "DHD_VSYNC", "0", CHOICES(switches), 0},  /* no tearing (port/gs/gpu_rust.c) */
    {"Unlock All:", "unlock_all", "DHD_UNLOCK_ALL", "0", CHOICES(switches), 1},  /* port_unlock_all */
};

/* Unlock All (dhd.ini "unlock_all", DHD_UNLOCK_ALL): the game's unlock checks answer as if
   everything were unlocked, while it is on: tracks, riders and pro riders, sponsor rows, the
   shop's items and the preset bikes (patches.py's track mask, patches_unlock.py; finding 095).
   The save's own unlocks are never written, so turning it off brings them back. */
int port_unlock_all(void)
{
    static HleEnv env;
    const char *v = hle_env(&env, "DHD_UNLOCK_ALL");
    return v && atoi(v) != 0;
}

static int loaded;

/* The lists that depend on this PC: the scales the renderer draws, the monitor's size. */
static void choices_build(void)
{
    const char *most = host_getenv("DHD_SCALE_MAX");
    int scale_count = most && atoi(most) > 4 ? atoi(most) : 4, size_count = 0;
    if (scale_count > (int)(sizeof scales / sizeof *scales)) scale_count = (int)(sizeof scales / sizeof *scales);
    memcpy(scale_list, scales, (size_t)scale_count * sizeof *scales);
    scale_list[scale_count].text = "Window"; scale_list[scale_count].value = "window";
    rows[RESOLUTION].choices = scale_list; rows[RESOLUTION].count = scale_count + 1;
    for (; size_count < (int)(sizeof sizes / sizeof *sizes); size_count++) size_list[size_count] = sizes[size_count];
    const char *mode = host_getenv("DHD_MONITOR_MODE");
    int w = 0, h = 0;
    if (mode && sscanf(mode, "%dx%d", &w, &h) == 2 && w > 0 && h > 0) {
        snprintf(monitor_size, sizeof monitor_size, "%dx%d", w, h);
        int known = 0;
        for (int i = 0; i < size_count; i++) known |= !strcmp(sizes[i].value, monitor_size);
        if (!known) { size_list[size_count].text = size_list[size_count].value = monitor_size; size_count++; }
    }
    rows[SIZE].choices = size_list; rows[SIZE].count = size_count;
    int rate_count = 0;
    for (int i = 0; i < (int)(sizeof rates / sizeof *rates); i++)
        if (atoi(rates[i].value) <= port_fps_max()) rate_list[rate_count++] = rates[i];
    rate_list[rate_count].text = "Monitor"; rate_list[rate_count].value = "monitor";
    rows[RATE].choices = rate_list; rows[RATE].count = rate_count + 1;
}
static char footer[64];
static const char *restart_footer = "* Applies when the game starts again";
static int shown[ROWS], shown_count;  /* the page's rows: indexes into rows[] */

/* Whether env is one of list's comma-separated names. */
static int listed(const char *list, const char *env)
{
    size_t n = strlen(env);
    for (const char *at = list; at && (at = strstr(at, env)); at += n)
        if ((at == list || at[-1] == ',') && (at[n] == 0 || at[n] == ',')) return 1;
    return 0;
}

/* The rows the host can do (DHD_PC_SETTINGS_ROWS, else all), the ones it applies at once besides
   the live ones (DHD_PC_SETTINGS_LIVE) and its words for the others (DHD_PC_SETTINGS_RESTART). */
static void choose_rows(void)
{
    const char *list = host_getenv("DHD_PC_SETTINGS_ROWS"), *live = host_getenv("DHD_PC_SETTINGS_LIVE");
    const char *restart = host_getenv("DHD_PC_SETTINGS_RESTART");
    if (restart && *restart) restart_footer = restart;
    shown_count = 0;
    for (int i = 0; i < ROWS; i++) {
        if (!list || listed(list, rows[i].env)) shown[shown_count++] = i;
        if (live && listed(live, rows[i].env)) rows[i].live = 1;
    }
}

static Row *row_at(int index) { return index >= 0 && index < shown_count ? &rows[shown[index]] : NULL; }

/* The value the program runs with now: the variable (set from dhd.ini or the environment). */
static void current(const Row *r, char *out, size_t size)
{
    if (r == &rows[SIZE]) {
        const char *w = host_getenv("DHD_WINDOW_WIDTH"), *h = host_getenv("DHD_WINDOW_HEIGHT");
        snprintf(out, size, "%dx%d", w && atoi(w) > 0 ? atoi(w) : 896, h && atoi(h) > 0 ? atoi(h) : 672);
        return;
    }
    const char *v = host_getenv(r->env);
    snprintf(out, size, "%s", v && *v ? v : r->fallback);
    if (r == &rows[WIDESCREEN] || r == &rows[MOVIES] || r == &rows[VIBRATION] || r == &rows[VSYNC] || r == &rows[UNLOCK])
        snprintf(out, size, "%d", atoi(out) != 0);
    if (r == &rows[TEXTURES])
        snprintf(out, size, "%s", v && (!dhd_stricmp(v, "off") || !strcmp(v, "0") || !dhd_stricmp(v, "false") || !dhd_stricmp(v, "no"))
                 ? "off" : "on");
    if (r == &rows[FONT])  /* as font_sharp_on reads it */
        snprintf(out, size, "%s", v && (!dhd_stricmp(v, "original") || !dhd_stricmp(v, "off") || !strcmp(v, "0")) ? "original" : "sharp");
}

static int find(const Row *r, const char *value)
{
    for (int i = 0; i < r->count; i++)
        if (!dhd_stricmp(r->choices[i].value, value)) return i;
    return -1;
}

static int locked(const Row *r)
{
    return host_service_host_setting_locked(GADDR(r->env)) || (r == &rows[SIZE] && host_service_host_setting_locked(GADDR("DHD_WINDOW_HEIGHT")));
}

static MenuPage page;

/* A row on the page: "Label:  < Value >", "*" after it while it waits for the next start; no
   arrows when the environment sets it. */
static void format(Row *r)
{
    char value[40];
    int index = -1;
    for (int i = 0; i < shown_count; i++)
        if (&rows[shown[i]] == r) index = i;
    if (index < 0) return;
    if (r->selected >= 0) snprintf(value, sizeof value, "%s", r->choices[r->selected].text);
    else current(r, value, sizeof value);
    if (r->selected != r->running) strncat(value, " *", sizeof value - strlen(value) - 1);
    menu_page_value_set(&page, index, r->label, value, !locked(r));
}

/* Saves the row's choice and, for a live row, applies it. */
static void commit(Row *r, int apply)
{
    const Choice *c = &r->choices[r->selected];
    if (r == &rows[SIZE]) {
        char w[16], h[16];
        sscanf(c->value, "%15[0-9]x%15[0-9]", w, h);
        host_service_host_setting_save(GADDR("width"), GADDR(w));
        host_service_host_setting_save(GADDR("height"), GADDR(h));
        if (apply) { host_putenv_s("DHD_WINDOW_WIDTH", w); host_putenv_s("DHD_WINDOW_HEIGHT", h); }
    } else {
        host_service_host_setting_save(GADDR(r->key), GADDR(c->value));
        if (apply) host_putenv_s(r->env, c->value);
    }
    if (apply) hle_env_changed();
    if (!apply) return;
    r->running = r->selected;
    if (r == &rows[DISPLAY] || r == &rows[SIZE]) host_service_gs_window_apply();
    else if (r == &rows[SHAPE] || r == &rows[SCALING]) host_service_gs_present_options_apply();
    else if (r == &rows[VIBRATION]) host_service_host_pad_vibrate(0, 0, 0);
}

static void change(int index, int step)
{
    Row *r = row_at(index);
    if (!r) return;
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
    for (int i = 0; i < shown_count; i++) {
        menu_page_value(p, rows[shown[i]].label, "", 1);
        format(&rows[shown[i]]);
    }
}

static void page_update(MenuPage *p, int cursor)  /* the footer for the row */
{
    const Row *r = row_at(cursor);
    p->footer = NULL;
    if (r && locked(r)) {
        snprintf(footer, sizeof footer, "Set by %s outside the game", r == &rows[SIZE] ? "DHD_WINDOW_WIDTH" : r->env);
        p->footer = footer;
    } else if (r && r->selected != r->running) p->footer = restart_footer;
    else if (r == &rows[TEXTURES] && !host_service_texpack_available()) p->footer = "No pack: see texture_pack in dhd.ini";
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
        choices_build();
        choose_rows();
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
