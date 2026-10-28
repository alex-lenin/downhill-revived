/* Online rooms in the game's popup style (menu_page.c), with physical keyboard text entry.
   The flow (finding 092): Create or Join waits on the ONLINE page until the server has the player
   in the room; then the game's own rider screen, once, and its ride to the shop, whose exit
   (port_online_shell, menu 18) opens the RACE ROOM over the rider screen (the rider on its
   stand, Change Rider to choose again); the host starts; loading; the race. The shop's own pages
   (bike, jersey, colour) are skipped as since finding 065's rework: see finding 092. The host chooses the track and starts when everyone is ready. The
   existing online.c setup event carries each complete preview choice. */
#include "hle.h"
#include "menu_page.h"
#include "text_input.h"
#include "settings_table.h"  /* the rider name's rule (dhd_settings, port/render/settings) */
#include "../net/net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Downhill Domination (USA) SCUS-97177, redump disc 30393 (whole file 0fe2b777...): its game files
   hash, dhd_disc's USA_FILES_SHA256 (port/render/disc, finding 098). Only the files count, so a
   good dump with other sectors around them is taken too. */
static const char us_files_sha256[] = "c8c17bcae383daf9f2feadc7f6d7476c43d659ab8d66127a299b01311f2a3476";

/* The tracks with a 2-4 player level (online.c mp_level) and the riders every save has
   (online.c regular): the game's indexes and names, as the online launcher lists them. */
static const struct { int index; const char *name; } tracks[] = {
    {0, "ALPINE"}, {1, "MOAB"}, {2, "JUNGLE"}, {3, "PERU"}, {4, "PERU TDH"}, {5, "GLACIER"},
    {6, "ALPINE MX"}, {7, "MOAB MX"}, {8, "JUNGLE MX"}, {9, "PERU MX"}, {10, "JUNGLE TD"},
    {11, "AUBER ST"}, {12, "FUJI"}, {13, "MOAB TDH"}, {14, "GLACIER X"}, {15, "AUBER MX"},
    {16, "AUBER STD"}, {19, "BC"}, {20, "CITY"}, {22, "GLACIER T"}, {23, "MOSH"}, {24, "ALPINE T"},
    {25, "BC TDX"}};
static const struct { int index; const char *name; } riders[] = {
    {0, "COSMO"}, {1, "MAI"}, {2, "AJAX"}, {3, "JONNIE"}, {4, "KALOLO"}, {5, "T-BAG"}, {8, "DRUD"},
    {10, "RAKEL"}, {13, "JEN"}, {14, "WOODY"}};
#define COUNT(a) ((int)(sizeof a / sizeof *a))

static const char code_letters[] = "23456789ABCDEFGHJKMNPQRSTUVWXYZ";  /* the server's join codes */
static const char name_letters[] = SETTING_NAME_LETTERS;
enum { NAME_MAX = SETTING_NAME_MAX, CODE_LENGTH = 6, ROW_TEXT = 96, MAX_ROWS = 12, LIST_MAX = 8, MEMBER_ROWS = 4 };

enum { HOME, LIST, NAME, CODE, ROOM };
/* The room's rows */
enum { R_CODE, R_TRACK, R_BOTS, R_SELECT, R_MEMBER, R_READY = R_MEMBER + MEMBER_ROWS, R_START, R_LEAVE, R_ROWS };
/* The room's options, carried in the server's "laps" setting (Downhill Domination has no laps):
   every PC builds the same race from them (online.c hle_online_ai_count). */
enum { ROOM_NO_BOTS = 1 };
static const char *const rider_names[16] = {
    "COSMO", "MAI", "AJAX", "JONNIE", "KALOLO", "T-BAG", "ERIC CARTER", "TARA LLANES", "DRUD",
    "BRIAN LOPES", "RAKEL", "MISSY GIOVE", "RICHIE SCHLEY", "JEN", "WOODY", "KINETICLOPS"};
static MenuPage page;
static int screen, race;
static int choosing, selected, selection_requested, room_focus;
/* The room over the rider screen (port_online_shell): in_shell while it runs; room_pending
   while the camera flies to the rider screen it opens over; hold_rider until the press that
   closed it is released; resume_home: back on the title, ONLINE opens again. */
static int in_shell, room_pending, hold_rider, resume_home;
/* connecting: Create or Join was chosen; the page waits for the server's room (connect_poll). */
static int connecting;
static int chosen_rider, chosen_bike, ready_track, ready_options;
static int chosen_spot;  /* the chosen rider's place on the rider screen's carousel (D_004C6910), 0 none */
static int public_room, room_settings_sent, room_ready_sent, room_track = 0, room_options;
static int32_t chosen_outfit[3], chosen_cycle[3];
#define SHELL_WORD(a) (*(int32_t *)(uintptr_t)(0x08000000u + (a)))
#define SETUP_WORD(offset) SHELL_WORD(0x4B25A0 + (offset))
void port_online_choose_begin(int flight);
void port_online_choose_end(void);
void func_001F67F0(int enabled);
void func_001F6798(int enabled);
void func_001F5B08(int enabled);
void func_001F6940(void);
void func_0021E428(int enabled);
void func_001F65E0(int enabled);
void func_001F5578(void);
void func_001F60C8(int rider);
void func_00203590(void *animation, int resource, int bank, int index);
void func_00203FE0(void *animation, int enabled);
void func_002038E0(void *animation, int enabled);
void func_001F50F0(int resource, int index);
void func_001F57D0(int resource, int enabled);
void func_001C59D8(void *sound);
void *func_0023D040(int view);
void func_0023D020(int view, void *camera);
void func_0023C8D8(int mode, int view);
void func_0023D898(int view, float *distance, float *aspect);
void func_0023D8D8(int view, float distance, float aspect);
float func_00203C28(void *animation);
void func_001F79D8(int level, int frames);  /* the shell's screen fade */
extern int D_00275CB0[];  /* each rider's camera take out of the shop (func_001F1500) */
int func_00220230(int port);  /* the buttons held on a pad */
static float shell_distance, shell_aspect;
static int offline_mode, offline_submode, offline_players;
void hle_online_selected_ride(int rider, const int32_t *outfit, const int32_t *bike);

/* SharedMenu can change a one-view shell's render mode in place; the stock popup
   manager only restores its view descriptor when it began with multiple views. */
static void run_page(void)
{
    void *camera = func_0023D040(0);
    int mode = SHELL_WORD(0x75A9D8);
    float distance, aspect;
    func_0023D898(0, &distance, &aspect);
    menu_page_run(&page);
    func_0023D020(0, camera);
    func_0023C8D8(mode, 0);
    func_0023D8D8(0, distance, aspect);
    host_service_text_input_end();
}

/* The retail rider screen's Back cleanup (func_001F24B8), also needed when
   intercepting GET READY: changing only MENU bypasses all of these side effects. */
static void selection_end(void)
{
    void *animation = (void *)(uintptr_t)(0x08000000u + 0x4C6370);
    void *sound = (void *)(uintptr_t)(0x08000000u + 0x4B2A90);
    func_001F67F0(0);
    func_001F6798(0);
    func_001F5B08(0);
    func_001F6940();
    SHELL_WORD(0x29E024) = 0;
    func_0021E428(0);
    func_001F65E0(0);
    func_001F5578();
    func_001F60C8(0);
    for (int i = 0; i < 7; i++) SETUP_WORD(0x2A8 + 4 * i) = -1;
    SHELL_WORD(0x29DF68) = 0;
    SHELL_WORD(0x4C6910) = 1;
    func_00203590(animation, 0x2330, 0, 1);
    func_00203FE0(animation, 1);
    func_002038E0(animation, 0);
    func_001F50F0(0x2330, 1);
    func_001F57D0(0x232B, 0);
    func_001F57D0(0x233D, 0);
    func_001F57D0(0x235F, 0);
    func_001F57D0(0x2329, 1);
    func_001C59D8(sound);
    uintptr_t music = (uint32_t)SHELL_WORD(0x4B2A90 + 0x1310);
    if (music) *(float *)(music + 0xB8) = 1.0f;
    func_0023D8D8(0, shell_distance, shell_aspect);
    SHELL_WORD(0x29DED8) = 15;
    SHELL_WORD(0x29DEEC) = 3;
    port_online_choose_end();
}

/* Online everyone has every rider, bike and track, as retail's unlock-all code gives them
   (func_001F0780: D_004325A0.x2180 the 16 riders, x2188 the 18 bikes, x2114 the tracks but 21
   and 23, D_004225A0.x12190 23 more flags), and the rider screen's pros too: their statues come
   alive from the save's rider bits D_004225A0.x12110 (0-13), which func_001BB670 copies into the
   screen's table (D_004B25A0.x3C). The player's own progress comes back when they leave online
   play, and the memory card keeps it meanwhile: its writes are held (mc.c). */
enum { UNLOCK_PROS = 0x4346B0, UNLOCK_TRACKS = 0x4346B4, UNLOCK_RIDERS = 0x434720, UNLOCK_BIKES = 0x434728,
       UNLOCK_FLAGS = 0x434730 };
void func_001BB670(void *game);
static int unlocked;
static int32_t kept_pros, kept_tracks, kept_riders, kept_bikes;
static uint8_t kept_flags[23];

static void unlock_all(void)
{
    if (unlocked) return;
    uint8_t *flags = (uint8_t *)(uintptr_t)(0x08000000u + UNLOCK_FLAGS);
    kept_pros = SHELL_WORD(UNLOCK_PROS), kept_tracks = SHELL_WORD(UNLOCK_TRACKS);
    kept_riders = SHELL_WORD(UNLOCK_RIDERS), kept_bikes = SHELL_WORD(UNLOCK_BIKES);
    memcpy(kept_flags, flags, sizeof kept_flags);
    SHELL_WORD(UNLOCK_PROS) |= 0x3FFF;
    func_001BB670((void *)(uintptr_t)(0x08000000u + 0x4B25A0));
    SHELL_WORD(UNLOCK_TRACKS) |= 0x035FFFFF;
    SHELL_WORD(UNLOCK_RIDERS) |= 0xFFFF;
    SHELL_WORD(UNLOCK_BIKES) |= 0x3FFFF;
    memset(flags, 1, sizeof kept_flags);
    hle_mc_hold = unlocked = 1;
    hle_log("online menu: everything unlocked while online\n");
}

/* Back to offline play: the player's own unlocks again. */
void port_online_relock(void)
{
    if (!unlocked) return;
    SHELL_WORD(UNLOCK_PROS) = kept_pros, SHELL_WORD(UNLOCK_TRACKS) = kept_tracks;
    SHELL_WORD(UNLOCK_RIDERS) = kept_riders, SHELL_WORD(UNLOCK_BIKES) = kept_bikes;
    func_001BB670((void *)(uintptr_t)(0x08000000u + 0x4B25A0));
    memcpy((uint8_t *)(uintptr_t)(0x08000000u + UNLOCK_FLAGS), kept_flags, sizeof kept_flags);
    hle_mc_hold = unlocked = 0;
    hle_log("online menu: the player's own unlocks again\n");
}

static void reset_selection(void)
{
    selected = selection_requested = room_focus = 0;
    chosen_spot = 0;
    room_settings_sent = room_ready_sent = 0;
}

static char text[MAX_ROWS][ROW_TEXT], status[128], footer[128];
static int outdated;  /* the server refused this release: HOME offers Update Now (row 5) */
/* 44 characters: the footer fits about 45 at 640 wide (0.1.0 shows the server's copy there too) */
static const char outdated_text[] = "A new version is out. Update to play online.";

static const char *server(void)
{
    const char *s = getenv("DHD_NET_SERVER");
    return s && *s ? s : NULL;
}

/* ---- the disc image's check ---- */

static volatile int iso_state;  /* 0 not started, 1 hashing, 2 done, 3 failed */

/* A thread's result for the screen: a sequentially consistent store (the Interlocked exchanges were these). */
static void set_state(volatile int *state, int value) { __atomic_store_n(state, value, __ATOMIC_SEQ_CST); }
static char iso_hash[65];  /* the game files hash, the one the server gets */

/* The check's result in the log, once: a disc image that boots (its SCUS_971.77 is right,
   game_data.c) can still be a dump whose other files differ, and online needs the USA disc's.
   note: what differs (the first file that isn't the USA disc's), or "". */
static void log_iso(const char *how, const char *note)
{
    const char *path = getenv("DHD_ISO");
    if (!strcmp(iso_hash, us_files_sha256))
        hle_log("online menu: the disc image has the USA disc's game files (files SHA-256 %s, %s)\n", iso_hash, how);
    else
        hle_log("online menu: the disc image %s has game files different from the USA disc's (redump 30393)%s%s: "
                "files SHA-256 %s, online needs %s (%s). A bad or modified dump: it can't be used online\n",
                path ? path : "(none)", *note ? ": " : "", note, iso_hash, us_files_sha256, how);
}

/* iso-check.txt, the check's cache (dhd_disc, shared with the launcher). */
static void iso_check_path(char *out, size_t size)
{
    const char *data = host_getenv("DHD_DATA_DIR");  /* the launcher's folder for the player's files */
    if (data) { snprintf(out, size, "%s/iso-check.txt", data); return; }
    char exe[HOST_PATH_MAX];
    host_exe_path(exe, sizeof exe);
    char *slash = strrchr(exe, '\\');
    if (slash) slash[1] = 0;
    snprintf(out, size, "%siso-check.txt", exe);
}

/* 64-bit hosts check on a host thread (finding 082): the blocking part (the cache, else reading
   the image's files) runs on the host, and page_update picks up the answer, so the menu stays
   responsive. */
static void start_iso_check(void)
{
    const char *given = getenv("DHD_NET_ISO_SHA256");  /* a tool's files hash */
    const char *path = getenv("DHD_ISO");
    char cache[HOST_PATH_MAX];
    if (iso_state) return;
    if (given && strlen(given) == 64) {
        snprintf(iso_hash, sizeof iso_hash, "%s", given);
        iso_state = 2;
        log_iso("given by DHD_NET_ISO_SHA256", "");
        return;
    }
    if (!path || !host_path_exists(path)) {
        hle_log("online menu: the disc image %s can't be found for the online check\n", path ? path : "(none)");
        set_state(&iso_state, 3);
        return;
    }
    iso_check_path(cache, sizeof cache);
    hle_log("online menu: checking the disc image's game files\n");
    set_state(&iso_state, 1);
    if (!host_service_disc_check_start(GADDR(path), GADDR(cache))) {
        hle_log("online menu: the disc image's check couldn't start\n");
        set_state(&iso_state, 3);
    }
}

/* One poll of the checking host job: the hash lands in iso_hash. */
static void poll_iso(void)
{
    static uint8_t digest[32];
    static char note[256];
    const char *path = getenv("DHD_ISO");
    int r;
    if (iso_state != 1) return;
    r = host_service_disc_check_poll(GADDR(digest), GADDR(note), sizeof note);
    if (!r) return;
    if (r == -1) {
        hle_log("online menu: reading the disc image %s failed during its online check: %s\n", path, note);
        set_state(&iso_state, 3);
        return;
    }
    if (r == -2) {
        /* no file system to hash, or a cut dump: no hash, and not the USA disc's files */
        memset(iso_hash, '0', 64), iso_hash[64] = 0;
        set_state(&iso_state, 2);
        hle_log("online menu: the disc image %s can't have the USA disc's game files: %s. A bad or modified dump: "
                "it can't be used online\n", path, note);
        return;
    }
    for (int i = 0; i < 32; i++) snprintf(iso_hash + 2 * i, 3, "%02x", digest[i]);
    set_state(&iso_state, 2);
    log_iso(r == 2 ? "from iso-check.txt" : "checked now", note);
}

/* ---- the public list ---- */

static volatile int list_state;  /* 0 none, 1 asking, 2 done */
static NetLobbyInfo list[LIST_MAX];
static int list_count, list_shown;  /* list_shown: the rows have the answer */

/* One poll of the fetching host job: the rows land in list, as before. */
static void poll_list(void)
{
    int r;
    if (list_state != 1) return;
    r = host_service_net_list_lobbies_poll(GADDR(list), LIST_MAX);
    if (r == -2) return;
    list_count = r;
    if (r < 0) hle_log("online menu: the public list: the server %s didn't answer\n", server());
    set_state(&list_state, 2);
}

/* ---- the player ---- */

static char name[NAME_MAX + 1], version[64];
static int rider = 0;  /* index into riders[] */

static void load_player(void)
{
    const char *n = getenv("DHD_NET_NAME"), *r = getenv("DHD_NET_RIDER");
    if (!name[0]) {
        int k = 0;
        for (const char *c = n ? n : ""; *c && k < NAME_MAX; c++) {
            char up = (char)(*c >= 'a' && *c <= 'z' ? *c - 32 : *c);
            if (strchr(name_letters, up)) name[k++] = up;
        }
        name[k] = 0;
        if (!k) snprintf(name, sizeof name, "%s", SETTING_NAME_UNNAMED);
        for (int i = 0; r && *r && i < COUNT(riders); i++)
            if (riders[i].index == atoi(r)) rider = i;
    }
    if (!version[0]) hle_online_version(version, sizeof version);
}

/* Saved to dhd.ini and set for this run, as the online launcher does. */
static void save(const char *key, const char *env, const char *value)
{
    host_service_host_setting_save(GADDR(key), GADDR(value));
    host_putenv_s(env, value);
    hle_env_changed();
}

/* What stops this PC from joining a race, or NULL. */
static const char *not_ready(void)
{
    const char *render = getenv("DHD_RENDER");
    if (!server()) return "No server: set server = ... in dhd.ini";
    if (!render || strcmp(render, "native")) return "Online races need the native renderer";
    if (iso_state == 1) return "Checking the disc image, a moment...";
    if (iso_state != 2) return "Can't read the disc image: choose it again";
    if (strcmp(iso_hash, us_files_sha256)) return "This disc's game files aren't the USA disc's";
    return NULL;
}

/* Why the disc image keeps this PC offline, as lines under ONLINE's rows (the footer holds one
   short line), NULL-ended; NULL when the image is fine. Each line fits the 640-wide screen. */
static const char *const *disc_note(void)
{
    static const char *const hashing[] = {"", "Checking the disc image for online...", NULL};
    static const char *const unreadable[] = {"", "The disc image couldn't be read for", "the online check. Choose it again",
                                             "in the launcher.", NULL};
    static const char *const different[] = {"", "The disc image's game files are", "different from the USA disc's (a bad",
                                            "or modified dump). Make a new dump or", "choose another image in the launcher.",
                                            NULL};
    if (iso_state == 1) return hashing;
    if (iso_state == 3) return unreadable;
    if (iso_state == 2 && strcmp(iso_hash, us_files_sha256)) return different;
    return NULL;
}

/* ---- the screens ---- */

static int entry_pick;           /* the letter on offer */
static char entry[16];
static int home_row;  /* HOME's row last chosen: the cursor goes back to it */
static const char *const *disc_shown;  /* the disc_note HOME's rows have */

static void go(int to)
{
    host_service_text_input_end();
    screen = to;
    if (to == NAME || to == CODE) host_service_text_input_begin();
    if (to == ROOM) room_focus = selected;
    status[0] = 0;
    menu_page_rebuild_at(&page, to == HOME ? home_row : 0);
}

static void start_list(void)
{
    if (list_state == 1) return;
    list_state = 1;
    host_service_net_init(GADDR(server()), GADDR(name), GADDR(version), GADDR(iso_hash));
    /* The fetch itself runs on a host thread; page_update picks up the rows. */
    if (!host_service_net_list_lobbies_start(LIST_MAX)) list_state = 2, list_count = -1;
}

static void enter_room(int create, const char *code)
{
    const char *why = not_ready();
    if (why) {
        snprintf(status, sizeof status, "%s", why);
        hle_log("online menu: can't %s: %s\n", create ? "create a room" : "join", why);
        return;
    }
    host_service_net_init(GADDR(server()), GADDR(name), GADDR(version), GADDR(iso_hash));
    if (create) host_service_net_create_room();
    else if (host_service_net_join_room(GADDR(code)) < 0) {
        snprintf(status, sizeof status, "That code can't be one");
        return;
    }
    reset_selection();
    choosing = 0;
    public_room = create == 2;
    room_settings_sent = !create;  /* a guest who becomes host keeps the room as it is */
    room_ready_sent = 0;
    connecting = 1;
    snprintf(status, sizeof status, "Connecting to the server...");
    hle_log("online menu: %s %s\n", create ? "creating a room" : "joining", create ? (public_room ? "(public)" : "(private)") : code);
}

static void page_build(MenuPage *p)
{
    switch (screen) {
    case HOME:
        p->title = "ONLINE";
        snprintf(text[0], ROW_TEXT, "Name:  %s", name);
        menu_page_row(p, text[0], 1);
        menu_page_row(p, "Public Races", 1);
        menu_page_row(p, "Create A Public Race", 1);
        menu_page_row(p, "Create A Private Race", 1);
        menu_page_row(p, "Join With A Code", 1);
        if (outdated) menu_page_row(p, "Update Now", 1);
        disc_shown = disc_note();
        for (const char *const *line = disc_shown; line && *line; line++) menu_page_text(p, *line);
        break;
    case LIST:
        p->title = "PUBLIC RACES";
        list_shown = list_state == 2;
        if (list_state != 2) menu_page_row(p, "Looking for races...", 0);
        else if (list_count < 0) menu_page_row(p, "The server didn't answer", 0);
        else if (list_count == 0) menu_page_row(p, "No public races right now", 0);
        for (int i = 0; list_state == 2 && i < list_count; i++) {
            const NetLobbyInfo *l = &list[i];
            const char *track = "?";
            for (int k = 0; k < COUNT(tracks); k++)
                if (tracks[k].index == l->track) track = tracks[k].name;
            int same = !strcmp(l->version, version);
            snprintf(text[i], ROW_TEXT, "%.14s  %d/%d  %s%s", l->room_name[0] ? l->room_name : l->host_name,
                     l->players, l->max_players, track, same ? "" : " (other version)");
            menu_page_row(p, text[i], same && l->players < l->max_players);
        }
        menu_page_row(p, "Look Again", list_state == 2);
        break;
    case NAME:
    case CODE:
        p->title = screen == NAME ? "YOUR NAME" : "JOIN WITH A CODE";
        menu_page_row(p, text[0], 1);  /* written every frame */
        menu_page_row(p, screen == NAME ? "Done" : "Join", 1);
        break;
    case ROOM:
        p->title = "RACE ROOM";
        for (int i = 0; i < R_ROWS; i++) {
            text[i][0] = 0;
            if (i == R_TRACK) menu_page_value(p, "Track:", "", 0);
            else if (i == R_BOTS) menu_page_value(p, "Computer Riders:", "", 0);
            else if (i == R_SELECT || i == R_READY || i == R_START || i == R_LEAVE) menu_page_row(p, text[i], 1);
            else menu_page_text(p, text[i]);
        }
        break;
    }
}

static void entry_text(void)
{
    const char *letters = screen == NAME ? name_letters : code_letters;
    int max = screen == NAME ? NAME_MAX : CODE_LENGTH;
    char shown = letters[entry_pick] == ' ' ? '-' : letters[entry_pick];
    if ((int)strlen(entry) < max)
        snprintf(text[0], ROW_TEXT, "%s  %s[%c]", screen == NAME ? "Name:" : "Code:", entry, shown);
    else
        snprintf(text[0], ROW_TEXT, "%s  %s", screen == NAME ? "Name:" : "Code:", entry);
}

/* The server refused or lost the room: its reason for the footer. */
static void refused(const NetLobby *l)
{
    int too_old = !strcmp(l->error_code, "client_outdated");
    if (too_old) outdated = 1;
    snprintf(status, sizeof status, "%s",
             too_old ? outdated_text : l->error[0] ? l->error : "The connection to the server failed");
    hle_log("online menu: refused: %s (%s)\n", l->error, l->error_code);
    host_service_net_leave();
    reset_selection();
    connecting = 0;
    if (too_old) home_row = 5;
}

/* In the room: the host's settings, then this member's first (not ready) state, once each. */
static void room_sync(const NetLobby *l)
{
    if (l->is_host && !room_settings_sent) {
        char room_name[40];
        snprintf(room_name, sizeof room_name, "%s", name);
        NetRoomSettings s = {tracks[room_track].index, room_options, public_room, 4, -1, room_name};
        host_service_net_set_room(GADDR(&s));
        room_settings_sent = 1;
    }
    if (!room_ready_sent) {
        hle_log("online menu: room %s%s, version %s\n", l->code, l->is_host ? " (host)" : "", version);
        host_service_net_set_ready(0);
        room_ready_sent = 1;
    }
}

/* Create or Join was chosen: once the server has this player in the room, everything is
   unlocked and the page closes for the game's rider screen (port_online_menu
   returns 2); a refusal stays here with its reason. Triangle gives up (page_back). */
static void connect_poll(MenuPage *p)
{
    static NetLobby l;
    host_service_net_get_lobby(GADDR(&l));
    if (l.state == NET_FAILED || l.state == NET_IDLE) {
        refused(&l);
        if (outdated && screen == HOME) menu_page_rebuild_at(p, home_row);
        return;
    }
    if (l.state != NET_LOBBY && l.state != NET_RACING) return;
    room_sync(&l);
    for (int k = 0; k < COUNT(tracks); k++)
        if (tracks[k].index == l.track) room_track = k;
    room_options = l.laps;
    connecting = 0;
    status[0] = 0;
    unlock_all();
    choosing = 1;
    race = 2;
    hle_log("online menu: in room %s: choosing the rider\n", l.code);
    menu_page_close(p);
}

static void room_update(MenuPage *p, NetLobby *l)
{
    if (l->state == NET_FAILED || l->state == NET_IDLE) {
        refused(l);
        if (in_shell) {  /* the rider screen's Back, then ONLINE again with the reason */
            race = 0;
            menu_page_close(p);
            return;
        }
        screen = HOME;
        menu_page_rebuild_at(p, home_row);
        return;
    }
    int in = l->state == NET_LOBBY || l->state == NET_RACING;
    if (in) room_sync(l);
    if (in)
        for (int k = 0; k < COUNT(tracks); k++)
            if (tracks[k].index == l->track) room_track = k;
    if (l->started || l->state == NET_RACING) {
        hle_log("online menu: room %s: the race starts\n", l->code);
        race = 1;
        hle_online_selected_ride(chosen_rider, chosen_outfit, chosen_cycle);
        hle_online_from_menu();
        menu_page_close(p);
        return;
    }
    int self_ready = 0;
    for (int i = 0; i < l->nmembers; i++)
        if (l->member[i].id == l->member_id) self_ready = l->member[i].ready;
    /* Wait for the server to acknowledge unready before handing input to the picker.
       If a start won the race, the earlier started branch loads the committed selection. */
    if (in && selection_requested && !self_ready) {
        selection_requested = 0;
        choosing = 1;
        unlock_all();
        race = 2;
        menu_page_close(p);
        return;
    }
    int own_ready = 0, conflict = 0;
    for (int i = 0; i < l->nmembers; i++) {
        if (l->member[i].id == l->member_id) own_ready = l->member[i].ready;
        else if (selected && l->member[i].rider == chosen_rider / 32) conflict = 1;
    }
    if (in) room_options = l->laps;
    if (own_ready && (conflict || ready_track != l->track || ready_options != l->laps)) host_service_net_set_ready(0);
    int ready = 0;
    for (int i = 0; i < l->nmembers; i++) ready += l->member[i].ready;
    int can_start = in && l->is_host && l->nmembers >= 2 && ready == l->nmembers;
    if (in) snprintf(text[R_CODE], ROW_TEXT, "Code: %s   %d/%d Players", l->code, l->nmembers, l->max_players);
    else snprintf(text[R_CODE], ROW_TEXT, "Connecting...");
    const char *track_name = NULL;
    for (int k = 0; k < COUNT(tracks); k++) if (tracks[k].index == l->track) track_name = tracks[k].name;
    char track_number[8];
    snprintf(track_number, sizeof track_number, "%d", l->track);
    if (!track_name) track_name = track_number;
    const char *bots = l->laps & ROOM_NO_BOTS ? "OFF" : "ON";
    menu_page_value_set(p, R_TRACK, "Track:", track_name, in && l->is_host);
    menu_page_value_set(p, R_BOTS, "Computer Riders:", bots, in && l->is_host);
    snprintf(text[R_SELECT], ROW_TEXT, "%s", selected ? "Change Rider" : "Choose Rider");
    for (int i = 0; i < MEMBER_ROWS; i++) {
        const NetMember *m = &l->member[i];
        char who[24], ping[16] = "";
        if (!in || i >= l->nmembers) {  /* an open place, dimmed (not a gap in the list) */
            snprintf(text[R_MEMBER + i], ROW_TEXT, "%s", in && i < l->max_players ? "Open" : "");
            continue;
        }
        snprintf(who, sizeof who, "%.12s%s", m->name, m->id == l->host_id ? " (host)" : "");
        for (char *c = who; *c; c++) if (*c >= 'a' && *c <= 'z') *c -= 32;
        if (m->rtt_ms >= 0) snprintf(ping, sizeof ping, " %d ms", m->rtt_ms);
        snprintf(text[R_MEMBER + i], ROW_TEXT, "%s  %s%s%s", who,
                 m->rider >= 1 && m->rider <= 16 ? rider_names[m->rider - 1] : "", m->ready ? "  READY" : "", ping);
    }
    int missing = (l->nmembers < 2 ? 2 : l->nmembers) - ready;
    snprintf(text[R_READY], ROW_TEXT, "%s", own_ready ? "Not Ready" : "Ready");
    snprintf(text[R_START], ROW_TEXT, "%s", l->is_host ? "Start The Race" : "The host starts the race");
    snprintf(text[R_LEAVE], ROW_TEXT, "Leave Room");
    menu_page_enable(p, R_TRACK, in && l->is_host);
    menu_page_enable(p, R_BOTS, in && l->is_host);
    menu_page_enable(p, R_SELECT, in);
    menu_page_enable(p, R_READY, in && selected && !conflict);
    menu_page_enable(p, R_START, can_start);
    menu_page_enable(p, R_LEAVE, 1);
    if (room_focus) {
        menu_page_set_cursor(p, conflict ? R_SELECT : R_READY);
        room_focus = 0;
    }
    /* what to do next, else what everyone waits for */
    if (status[0]) p->footer = status;
    else if (!in) p->footer = "No connection: Leave Room to give up";
    else if (conflict) p->footer = "Rider taken: change your selection";
    else if (!selected) p->footer = "Choose your rider first";
    else if (l->nmembers < 2) {
        snprintf(footer, sizeof footer, "Waiting for players: join code %s", l->code);
        p->footer = footer;
    } else if (!own_ready) p->footer = "Ready when you like your selection";
    else if (missing > 0) {
        snprintf(footer, sizeof footer, "Waiting for %d player%s to get ready", missing, missing == 1 ? "" : "s");
        p->footer = footer;
    } else p->footer = l->is_host ? "Everyone is ready: start when you like" : "Everyone is ready: the host starts";
}

static void page_accept(MenuPage *p, int row);
static void page_step(MenuPage *p, int row, int by);

/* Over the rider screen the room closes (port_online_shell takes the rider screen's Back to
   the title and opens ONLINE there); over the title it is ONLINE's own page. */
static void leave_room(MenuPage *p)
{
    hle_log("online menu: leaving the room\n");
    host_service_net_leave();
    reset_selection();
    if (in_shell) {
        race = 0;
        menu_page_close(p);
    } else
        go(HOME);
}

static void page_update(MenuPage *p, int cursor)
{
    if (getenv("DHD_ONLINE_MENU_TRACE")) {
        static char last[160];
        char trace[160];
        snprintf(trace, sizeof trace, "view=%d row=%d entry=%s", screen, cursor,
                 screen == NAME || screen == CODE ? entry : "");
        if (strcmp(last, trace)) {
            snprintf(last, sizeof last, "%s", trace);
            hle_log("online menu: %s\n", trace);
        }
    }
    p->footer = status[0] ? status : NULL;
    poll_iso();   /* the hashing host job: its digest lands in iso_hash */
    poll_list();  /* the fetching host job: its rows land in list */
    if (connecting) {
        connect_poll(p);
        p->footer = status[0] ? status : NULL;
        return;
    }
    switch (screen) {
    case HOME:
        if (disc_note() != disc_shown) {  /* the check has ended: its lines under the rows */
            menu_page_rebuild_at(p, cursor);
            break;
        }
        if (!status[0] && (cursor >= 2 || disc_shown) && not_ready()) p->footer = not_ready();
        break;
    case LIST:
        if (list_state == 2 && !list_shown) menu_page_rebuild(p);  /* the answer came: the rows again */
        break;
    case NAME:
    case CODE:
        {
            int action = host_service_text_input_edit(GADDR(entry), (screen == NAME ? NAME_MAX : CODE_LENGTH) + 1, GADDR(screen == NAME ? name_letters : code_letters));
            if (action < 0) { go(HOME); return; }
            if (action > 0) { page_accept(p, 1); return; }
        }
        entry_text();
        if (!status[0])
            p->footer = screen == CODE ? "TYPE/CTRL+V - ENTER JOIN - ESC BACK" : "TYPE OR PAD - ENTER DONE - ESC CANCEL";
        break;
    case ROOM: {
        static NetLobby l;
        host_service_net_get_lobby(GADDR(&l));
        room_update(p, &l);
        break;
    }
    }
}

static void page_accept(MenuPage *p, int row)
{
    if (connecting) return;  /* Triangle gives up; nothing else meanwhile */
    status[0] = 0;
    switch (screen) {
    case HOME:
        home_row = row;
        if (row == 0) {
            snprintf(entry, sizeof entry, "%s", name);
            entry_pick = 0;
            go(NAME);
        } else if (row == 1) {
            if (!server()) snprintf(status, sizeof status, "No server: set server = ... in dhd.ini");
            else {
                start_list();
                go(LIST);
            }
        } else if (row == 2 || row == 3)
            enter_room(row == 2 ? 2 : 1, NULL);
        else if (row == 4) {
            entry[0] = 0;
            entry_pick = 0;
            go(CODE);
        } else if (row == 5 && outdated) {
            hle_log("online menu: Update Now\n");
            host_service_update_request();  /* ends the game; the launcher updates it */
        }
        break;
    case LIST:
        if (list_state == 2 && row < list_count && list_count > 0) enter_room(0, list[row].code);
        else if (list_state == 2) {
            start_list();
            menu_page_rebuild(p);
        }
        break;
    case NAME:
    case CODE: {
        const char *letters = screen == NAME ? name_letters : code_letters;
        int max = screen == NAME ? NAME_MAX : CODE_LENGTH;
        size_t n = strlen(entry);
        if (row == 0) {
            if ((int)n < max) entry[n] = letters[entry_pick], entry[n + 1] = 0;
            if (screen == CODE && (int)n + 1 == max) menu_page_set_cursor(p, 1);  /* the code is whole: Join */
        } else if (screen == NAME) {
            while (n && entry[n - 1] == ' ') entry[--n] = 0;
            if (n) {
                snprintf(name, sizeof name, "%s", entry);
                save("name", "DHD_NET_NAME", name);
            }
            go(HOME);
        } else if ((int)n == CODE_LENGTH)
            enter_room(0, entry);
        else
            snprintf(status, sizeof status, "A code has %d letters and digits", CODE_LENGTH);
        break;
    }
    case ROOM: {
        NetLobby l;
        host_service_net_get_lobby(GADDR(&l));
        if (row == R_TRACK || row == R_BOTS) {
            page_step(p, row, 1);
            break;
        }
        if (row == R_SELECT) {
            host_service_net_set_ready(0);
            selection_requested = 1;
        } else if (row == R_READY && !strcmp(text[R_READY], "Not Ready"))  /* the label shows the last snapshot */
            host_service_net_set_ready(0);
        else if (row == R_READY) { ready_track = l.track, ready_options = l.laps; host_service_net_set_ready(1); }
        else if (row == R_START && l.is_host) host_service_net_start_race();
        else if (row == R_LEAVE) leave_room(p);
        break;
    }
    }
}

static void page_back(MenuPage *p)
{
    if (connecting) {
        hle_log("online menu: connecting given up\n");
        host_service_net_leave();
        reset_selection();
        connecting = 0;
        status[0] = 0;
        return;
    }
    switch (screen) {
    case HOME:
        menu_page_cancel(p);
        break;
    case NAME:
    case CODE: {
        size_t n = strlen(entry);
        if (n) entry[n - 1] = 0;
        else go(HOME);
        break;
    }
    case LIST:
        go(HOME);
        break;
    case ROOM:
        leave_room(p);
        break;
    }
}

static void page_step(MenuPage *p, int row, int by)
{
    if (connecting) return;
    if (screen == ROOM && (row == R_TRACK || row == R_BOTS)) {  /* the host's track and options */
        NetLobby l;
        host_service_net_get_lobby(GADDR(&l));
        if (!l.is_host) return;
        NetRoomSettings s = {-1, -1, -1, -1, -1, NULL};
        if (row == R_TRACK) {
            room_track = (room_track + by + COUNT(tracks)) % COUNT(tracks);
            s.track = tracks[room_track].index;
        } else
            s.laps = room_options = l.laps ^ ROOM_NO_BOTS;
        host_service_net_set_room(GADDR(&s));
        menu_page_sound(by > 0);
        return;
    }
    if ((screen == NAME || screen == CODE) && row == 0) {
        int n = screen == NAME ? (int)sizeof name_letters - 1 : (int)sizeof code_letters - 1;
        entry_pick = (entry_pick + by + n) % n;
        menu_page_sound(by > 0);
    }
    (void)p;
}

static void page_left(MenuPage *p, int row) { page_step(p, row, -1); }
static void page_right(MenuPage *p, int row) { page_step(p, row, 1); }

/* The main menu's ONLINE row (patched func_001ED5B8), also when port_online_resume opens it
   again: the page until it closes; 2 when a room was joined (the rider screen next, as SINGLE
   PLAYER's), 0 when the player backed out. */
int port_online_menu(void)
{
    int resumed = resume_home;
    resume_home = 0;
    choosing = 0;
    reset_selection();
    load_player();
    offline_mode = SETUP_WORD(0x10C);
    offline_submode = SETUP_WORD(0x110);
    offline_players = SETUP_WORD(0x288);
    func_0023D898(0, &shell_distance, &shell_aspect);
    start_iso_check();
    page.build = page_build;
    page.update = page_update;
    page.accept = page_accept;
    page.back = page_back;
    page.left = page_left;
    page.right = page_right;
    page.title = "ONLINE";
    screen = HOME;
    if (!resumed) {
        home_row = 0;
        status[0] = 0;  /* a room that closed leaves its reason for HOME */
    }
    race = 0;
    in_shell = 0;
    connecting = 0;
    run_page();
    if (!race) { host_service_net_leave(); reset_selection(); connecting = 0; }
    return race;
}

/* The main menu (patched func_001ED5B8), every frame: 1 once to choose ONLINE again after a
   room closed, when the camera is back on the title. */
int port_online_resume(void)
{
    void *animation = (void *)(uintptr_t)(0x08000000u + 0x4C6370);
    if (!resume_home) return 0;
    func_00203590(animation, 0x2330, 0, 1);
    float t = func_00203C28(animation);
    return t <= 0.0f || t >= 1.0f;
}

/* The main menu's labels (patched func_001ED5B8): its table has three; the fourth is ONLINE. */
int port_main_label(const int *table, int index)
{
    return index < 3 ? table[index] : (int)(uintptr_t)"ONLINE";
}

static void keep_selection(void)
{
    chosen_rider = SETUP_WORD(0x290);
    chosen_spot = SHELL_WORD(0x4C6910 + 4 * SHELL_WORD(0x29DF68));
    chosen_bike = SETUP_WORD(0x11C);
    for (int i = 0; i < 3; i++) {
        chosen_outfit[i] = SETUP_WORD(0x1B8 + 4 * i);
        chosen_cycle[i] = SETUP_WORD(0x11C + 4 * i);
    }
    selected = 1;
    host_service_net_pick(-1, chosen_rider / 32, chosen_bike);
    host_service_net_set_ready(0);
    hle_log("online menu: selection rider %d (carousel place %d, player %d), outfit %d %d %d, bike %d %d %d\n",
            chosen_rider, chosen_spot, SHELL_WORD(0x29DF68), chosen_outfit[0], chosen_outfit[1], chosen_outfit[2],
            chosen_cycle[0], chosen_cycle[1], chosen_cycle[2]);
}

/* The shell's frame, before its menu runs; 1 skips that menu this frame. Only online selection
   enters here. The room is a page over the rider screen (menu 3):
   - GET READY (menu 9) keeps the choice and flies the camera back to the rider screen, as the
     main menu's SINGLE PLAYER does, instead of loading a race before everyone is ready;
   - the rider screen's Back (Triangle) opens the room instead of leaving for the title;
   - Change Selection closes the room on the rider screen itself;
   - leaving the room (or losing it) takes the rider screen's own Back to the title, where the
     main menu opens ONLINE again (port_online_resume). */
int port_online_shell(void)
{
    if (!choosing) return 0;
    int menu = SHELL_WORD(0x29DED8);
    if (menu == 16) {
        /* Single races only: bypass the career's NEW / CONTINUE and mode menus. */
        SHELL_WORD(0x29DED8) = 11;
        SHELL_WORD(0x29DEEC) = 1;
        SETUP_WORD(0x10C) = 12;
        SETUP_WORD(0x110) = 0;
        return 0;
    }
    NetLobby connection;
    host_service_net_get_lobby(GADDR(&connection));
    int lost = connection.state == NET_FAILED || connection.state == NET_IDLE;
    if (menu == 18) {
        /* The shop's START: when its camera has flown out, retail goes on to the level chooser,
           which sets up the race's loading (func_00139BF8) and takes the camera. The choice is
           complete here: keep it and leave as the rider screen's Back does; the room (with the
           host's track) opens over the rider screen. */
        void *animation = (void *)(uintptr_t)(0x08000000u + 0x4C6370);
        int rider_index = SHELL_WORD(0x4C6910 + 4 * SHELL_WORD(0x29DF68));
        func_00203590(animation, 0x2330, 0, D_00275CB0[rider_index]);
        if (func_00203C28(animation) < 1.0f) return 0;
        if (!lost) keep_selection();
        selection_end();
        func_001F79D8(0, 0x3C);  /* the shop's exit faded to black; the level chooser fades back */
        return 1;  /* the main menu next frame: the flight to the rider screen below */
    }
    if (room_pending && (menu == 15 || menu == 0)) room_pending = 0;  /* backed out meanwhile */
    if (room_pending) {
        /* The rider screen's own frames put the camera on it; the room's page freezes it. */
        if (menu != 3 || ++room_pending < 45) return 0;
        room_pending = 0;
    } else if (menu == 9 || menu == 15 || menu == 0) {
        if (menu == 9 && !lost) keep_selection();
        func_0023D8D8(0, shell_distance, shell_aspect);
        port_online_choose_begin(0);  /* to the rider screen without the title's flight */
        room_pending = 1;
        return 0;
    } else if (menu == 3) {
        /* Read the pads, not func_001F4A98: its press latch would take the rider screen's own. */
        static int was_held;
        int held = (func_00220230(0) | func_00220230(1)) & 0xFFFF;
        int back = held & 0x1000 && !(was_held & 0x1000);  /* Triangle */
        was_held = held;
        if (hold_rider) {
            if (held) return 1;
            hold_rider = 0;
        }
        if (!back && !lost) return 0;
    } else
        return 0;

    in_shell = 1;
    func_001F57D0(0x232B, 0);  /* the rider screen's name plates, which would show through */
    func_001F57D0(0x233D, 0);
    func_001F57D0(0x235F, 0);
    go(ROOM);
    race = 0;
    run_page();
    in_shell = 0;
    if (race == 2) {
        hold_rider = 1;  /* the rider screen is here: choose on it */
        return 1;
    }
    choosing = 0;
    if (race == 1) {
        SHELL_WORD(0x29DED8) = 9;
        SHELL_WORD(0x29DEEC) = 8;
        SHELL_WORD(0x29DF40) = 120;
        SHELL_WORD(0x29DF44) = 0;
        return 1;
    }
    reset_selection();
    host_service_net_leave();
    selection_end();
    SETUP_WORD(0x10C) = offline_mode;
    SETUP_WORD(0x110) = offline_submode;
    SETUP_WORD(0x288) = offline_players;
    resume_home = 1;
    port_online_relock();
    return 1;
}

/* Back from the shop the rider screen shows the chosen rider (port_online_choose_begin), not the
   carousel's first place, which the rider screen's Back cleanup (selection_end) leaves. */
int port_online_rider_spot(void)
{
    int spot = selected && chosen_spot >= 1 && chosen_spot <= 14 ? chosen_spot : 1;
    hle_log("online menu: the rider screen on carousel place %d\n", spot);
    return spot;
}

/* Guests use the same retail handoff into the shop, without choosing a competing track. */
int port_online_guest_track(void)
{
    if (!choosing) return 0;
    NetLobby lobby;
    host_service_net_get_lobby(GADDR(&lobby));
    return !lobby.is_host;
}

int port_online_single_race(void) { return choosing; }

/* A track's name as the online launcher lists it (presence.c); NULL for a track not in the list. */
const char *port_track_name(int index)
{
    for (int k = 0; k < COUNT(tracks); k++)
        if (tracks[k].index == index) return tracks[k].name;
    return NULL;
}
