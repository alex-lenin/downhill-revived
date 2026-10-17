/* ONLINE: the main menu's fourth row (port/patches.py patches func_001ED5B8), online races in
   the game's own popup menu style (menu_page.c, finding 050), over the online client
   (port/net, docs/online-protocol.md):
   - HOME: the player's name, the public races, create a public or a private race, join one
     with its code;
   - PUBLIC RACES: the server's list (GET /v1/lobbies, in a thread); Cross joins one;
   - NAME and CODE: text entry with the pad: left/right pick the letter, Cross adds it,
     Triangle deletes the last one (on an empty entry: back);
   - the room: its join code, the track (the host's left/right), this player's rider
     (left/right), the players with their ping, and the host's Start The Race once everyone is
     ready (every player is ready once in, as with DHD_NET_*).
   When the room's race starts the page closes and online.c takes over as for DHD_NET_*
   (hle_online_from_menu): the menu script from SINGLE PLAYER to the race, the riders, the
   start together.
   The server is DHD_NET_SERVER: dhd.ini's server = ..., else the official public server
   (host.c host_settings_load). The
   ISO must be the USA disc, as the online launcher checks (port/online/Online.ps1): its SHA-256
   is worked out in a thread when the page opens, remembered in iso-check.txt beside dhd.exe in
   the launcher's format. The name and the rider are saved to dhd.ini (name, rider).
   A server that refuses this release as too old (client_outdated, finding 054) gets the
   message "A new version is out. Update to play online." and HOME an Update Now row:
   the game ends and the first dhd.exe updates it and starts it again (update.c). */
#include "hle.h"
#include "menu_page.h"
#include "../net/net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <bcrypt.h>

/* Downhill Domination (USA) SCUS-97177, redump disc 30393 */
static const char us_iso_sha256[] = "0fe2b77713d3440cc3030741ea0b18f49e3feaa09671a30a421e0d4888a7eb86";

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
static const char name_letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 -";
enum { NAME_MAX = 12, CODE_LENGTH = 6, ROW_TEXT = 96, MAX_ROWS = 12, LIST_MAX = 8, MEMBER_ROWS = 4 };

enum { HOME, LIST, NAME, CODE, ROOM };
static MenuPage page;
static int screen, race;
static char text[MAX_ROWS][ROW_TEXT], status[128], footer[128];
static int outdated;  /* the server refused this release: HOME offers Update Now (row 5) */
/* 44 characters: the footer fits about 45 at 640 wide (0.1.0 shows the server's copy there too) */
static const char outdated_text[] = "A new version is out. Update to play online.";

static const char *server(void)
{
    const char *s = getenv("DHD_NET_SERVER");
    return s && *s ? s : NULL;
}

/* ---- the ISO check, in a thread ---- */

static volatile LONG iso_state;  /* 0 not started, 1 hashing, 2 done, 3 failed */
static char iso_hash[65];

/* The online launcher's cache: "size:ticks:hash", ticks the file's .NET LastWriteTimeUtc. */
static void iso_check_path(char *out, size_t size)
{
    char exe[MAX_PATH];
    GetModuleFileNameA(NULL, exe, sizeof exe);
    char *slash = strrchr(exe, '\\');
    if (slash) slash[1] = 0;
    snprintf(out, size, "%siso-check.txt", exe);
}

static DWORD WINAPI hash_iso(void *arg)
{
    (void)arg;
    const char *path = getenv("DHD_ISO");
    WIN32_FILE_ATTRIBUTE_DATA info;
    char key[64], cache[MAX_PATH], line[160];
    if (!path || !GetFileAttributesExA(path, GetFileExInfoStandard, &info)) {
        InterlockedExchange(&iso_state, 3);
        return 0;
    }
    unsigned long long size = (unsigned long long)info.nFileSizeHigh << 32 | info.nFileSizeLow;
    unsigned long long ticks = ((unsigned long long)info.ftLastWriteTime.dwHighDateTime << 32 |
                                info.ftLastWriteTime.dwLowDateTime) + 504911232000000000ull;  /* 1601 -> 0001 */
    snprintf(key, sizeof key, "%llu:%llu:", size, ticks);
    iso_check_path(cache, sizeof cache);
    FILE *f = fopen(cache, "r");
    if (f) {
        if (fgets(line, sizeof line, f) && !strncmp(line, key, strlen(key)) && strlen(line + strlen(key)) >= 64) {
            memcpy(iso_hash, line + strlen(key), 64);
            iso_hash[64] = 0;
            fclose(f);
            InterlockedExchange(&iso_state, 2);
            return 0;
        }
        fclose(f);
    }
    hle_log("online menu: checking the ISO's SHA-256\n");
    BCRYPT_ALG_HANDLE alg;
    BCRYPT_HASH_HANDLE h;
    uint8_t digest[32];
    static uint8_t buf[1 << 20];
    DWORD n;
    int ok = 0;
    HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (file != INVALID_HANDLE_VALUE && BCryptOpenAlgorithmProvider(&alg, BCRYPT_SHA256_ALGORITHM, NULL, 0) == 0) {
        if (BCryptCreateHash(alg, &h, NULL, 0, NULL, 0, 0) == 0) {
            while (ReadFile(file, buf, sizeof buf, &n, NULL) && n) BCryptHashData(h, buf, n, 0);
            ok = BCryptFinishHash(h, digest, sizeof digest, 0) == 0;
            BCryptDestroyHash(h);
        }
        BCryptCloseAlgorithmProvider(alg, 0);
    }
    if (file != INVALID_HANDLE_VALUE) CloseHandle(file);
    if (!ok) {
        InterlockedExchange(&iso_state, 3);
        return 0;
    }
    for (int i = 0; i < 32; i++) snprintf(iso_hash + 2 * i, 3, "%02x", digest[i]);
    if ((f = fopen(cache, "w"))) fprintf(f, "%s%s\n", key, iso_hash), fclose(f);
    InterlockedExchange(&iso_state, 2);
    return 0;
}

static void start_iso_check(void)
{
    const char *given = getenv("DHD_NET_ISO_SHA256");  /* the launcher's, when it started us */
    if (iso_state) return;
    if (given && strlen(given) == 64) {
        snprintf(iso_hash, sizeof iso_hash, "%s", given);
        iso_state = 2;
        return;
    }
    iso_state = 1;
    HANDLE t = CreateThread(NULL, 0, hash_iso, NULL, 0, NULL);
    if (t) CloseHandle(t);
    else iso_state = 3;
}

/* ---- the public list, in a thread ---- */

static volatile LONG list_state;  /* 0 none, 1 asking, 2 done */
static NetLobbyInfo list[LIST_MAX];
static int list_count, list_shown;  /* list_shown: the rows have the answer */

static DWORD WINAPI fetch_list(void *arg)
{
    (void)arg;
    list_count = net_list_lobbies(list, LIST_MAX);
    InterlockedExchange(&list_state, 2);
    return 0;
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
        if (!k) snprintf(name, sizeof name, "RIDER");
        for (int i = 0; r && *r && i < COUNT(riders); i++)
            if (riders[i].index == atoi(r)) rider = i;
    }
    if (!version[0]) hle_online_version(version, sizeof version);
}

/* Saved to dhd.ini and set for this run, as the online launcher does. */
static void save(const char *key, const char *env, const char *value)
{
    host_setting_save(key, value);
    _putenv_s(env, value);
}

/* What stops this PC from joining a race, or NULL. */
static const char *not_ready(void)
{
    const char *render = getenv("DHD_RENDER");
    if (!server()) return "No server: set server = ... in dhd.ini";
    if (!render || strcmp(render, "native")) return "Online races need the native renderer";
    if (iso_state == 1) return "Checking the ISO, a moment...";
    if (iso_state != 2) return "The ISO couldn't be read";
    if (strcmp(iso_hash, us_iso_sha256)) return "This ISO isn't the USA disc (SCUS-97177)";
    return NULL;
}

/* ---- the screens ---- */

static int entry_pick;           /* the letter on offer */
static char entry[16];
static int public_room, room_settings_sent, room_ready_sent, room_track = 0;

static int home_row;  /* HOME's row last chosen: the cursor goes back to it */

static void go(int to)
{
    screen = to;
    status[0] = 0;
    menu_page_rebuild_at(&page, to == HOME ? home_row : 0);
}

static void start_list(void)
{
    if (list_state == 1) return;
    list_state = 1;
    net_init(server(), name, version, iso_hash);
    HANDLE t = CreateThread(NULL, 0, fetch_list, NULL, 0, NULL);
    if (t) CloseHandle(t);
    else list_state = 2, list_count = -1;
}

static void enter_room(int create, const char *code)
{
    const char *why = not_ready();
    if (why) {
        snprintf(status, sizeof status, "%s", why);
        return;
    }
    net_init(server(), name, version, iso_hash);
    if (create) net_create_room();
    else if (net_join_room(code) < 0) {
        snprintf(status, sizeof status, "That code can't be one");
        return;
    }
    public_room = create == 2;
    room_settings_sent = room_ready_sent = 0;
    hle_log("online menu: %s %s\n", create ? "creating a room" : "joining", create ? (public_room ? "(public)" : "(private)") : code);
    go(ROOM);
}

static void page_build(MenuPage *p)
{
    switch (screen) {
    case HOME:
        p->title = "ONLINE";
        snprintf(text[0], ROW_TEXT, "Name:\t\t%s", name);
        menu_page_row(p, text[0], 1);
        menu_page_row(p, "Public Races", 1);
        menu_page_row(p, "Create A Public Race", 1);
        menu_page_row(p, "Create A Private Race", 1);
        menu_page_row(p, "Join With A Code", 1);
        if (outdated) menu_page_row(p, "Update Now", 1);
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
            snprintf(text[i], ROW_TEXT, "%.14s\t\t%d/%d %s%s", l->room_name[0] ? l->room_name : l->host_name,
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
        for (int i = 0; i < 3 + MEMBER_ROWS + 1; i++) {
            text[i][0] = 0;
            menu_page_row(p, text[i], i == 1 || i == 2);
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
        snprintf(text[0], ROW_TEXT, "%s\t\t%s[%c]", screen == NAME ? "Name:" : "Code:", entry, shown);
    else
        snprintf(text[0], ROW_TEXT, "%s\t\t%s", screen == NAME ? "Name:" : "Code:", entry);
}

static void room_update(MenuPage *p, NetLobby *l)
{
    if (l->state == NET_FAILED || l->state == NET_IDLE) {
        int too_old = !strcmp(l->error_code, "client_outdated");
        if (too_old) outdated = 1;
        snprintf(status, sizeof status, "%s",
                 too_old ? outdated_text : l->error[0] ? l->error : "The connection to the server failed");
        hle_log("online menu: refused: %s (%s)\n", l->error, l->error_code);
        net_leave();
        screen = HOME;
        menu_page_rebuild_at(p, too_old ? 5 : home_row);
        return;
    }
    int in = l->state == NET_LOBBY || l->state == NET_RACING;
    if (in && l->is_host && !room_settings_sent) {
        char room_name[40];
        snprintf(room_name, sizeof room_name, "%s", name);
        NetRoomSettings s = {tracks[room_track].index, -1, public_room, 4, -1, room_name};
        net_set_room(&s);
        room_settings_sent = 1;
    }
    if (in && !room_ready_sent) {
        hle_log("online menu: room %s%s, version %s\n", l->code, l->is_host ? " (host)" : "", version);
        net_pick(-1, riders[rider].index + 1, 1);  /* the lobby keeps index + 1 */
        net_set_ready(1);
        room_ready_sent = 1;
    }
    if (in && !l->is_host)
        for (int k = 0; k < COUNT(tracks); k++)
            if (tracks[k].index == l->track) room_track = k;
    if (l->started || l->state == NET_RACING) {
        hle_log("online menu: room %s: the race starts\n", l->code);
        race = 1;
        hle_online_from_menu();
        menu_page_close(p);
        return;
    }
    int ready = 0;
    for (int i = 0; i < l->nmembers; i++) ready += l->member[i].ready;
    int can_start = in && l->is_host && l->nmembers >= 2 && ready == l->nmembers;
    if (in) snprintf(text[0], ROW_TEXT, "Code:\t\t%s%s", l->code, l->is_public ? " (public)" : "");
    else snprintf(text[0], ROW_TEXT, "Connecting...");
    snprintf(text[1], ROW_TEXT, "Track:\t\t%s", tracks[room_track].name);
    snprintf(text[2], ROW_TEXT, "Rider:\t\t%s", riders[rider].name);
    for (int i = 0; i < MEMBER_ROWS; i++) {
        const NetMember *m = &l->member[i];
        char who[16];
        if (!in || i >= l->nmembers) {
            snprintf(text[3 + i], ROW_TEXT, "%s", in && i == l->nmembers ? "-" : "");
            continue;
        }
        snprintf(who, sizeof who, "%.12s", m->name);
        for (char *c = who; *c; c++) if (*c >= 'a' && *c <= 'z') *c -= 32;
        if (m->rtt_ms >= 0)
            snprintf(text[3 + i], ROW_TEXT, "%s%s\t\t%s %d ms", who, m->id == l->host_id ? " (host)" : "",
                     m->ready ? "READY" : "", m->rtt_ms);
        else
            snprintf(text[3 + i], ROW_TEXT, "%s%s\t\t%s", who, m->id == l->host_id ? " (host)" : "", m->ready ? "READY" : "");
    }
    snprintf(text[7], ROW_TEXT, "%s", !in ? "" : l->is_host ? "Start The Race" : "The host starts the race");
    if (in && !l->is_host && menu_page_cursor(p) == 1) menu_page_set_cursor(p, 2);  /* the host's track */
    menu_page_enable(p, 1, in && l->is_host);
    menu_page_enable(p, 2, in);
    menu_page_enable(p, 7, can_start);
    if (status[0]) p->footer = status;
    else if (!in) p->footer = NULL;
    else if (!l->is_host) p->footer = "Waiting for the host to start";
    else if (l->nmembers < 2) {
        snprintf(footer, sizeof footer, "Friends join with the code %s", l->code);
        p->footer = footer;
    } else p->footer = can_start ? NULL : "Waiting for everyone to be ready";
}

static void page_update(MenuPage *p, int cursor)
{
    p->footer = status[0] ? status : NULL;
    switch (screen) {
    case HOME:
        if (!status[0] && cursor >= 2 && not_ready()) p->footer = not_ready();
        break;
    case LIST:
        if (list_state == 2 && !list_shown) menu_page_rebuild(p);  /* the answer came: the rows again */
        break;
    case NAME:
    case CODE:
        entry_text();
        if (!status[0])
            p->footer = cursor == 0 ? GLYPH_LEFT GLYPH_RIGHT " LETTER  " GLYPH_CROSS " ADD  " GLYPH_TRIANGLE " DELETE" : NULL;
        break;
    case ROOM: {
        static NetLobby l;
        net_get_lobby(&l);
        room_update(p, &l);
        break;
    }
    }
}

static void page_accept(MenuPage *p, int row)
{
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
            update_request();  /* ends the game; the first dhd.exe updates and starts it again */
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
    case ROOM:
        if (row == 7) {
            hle_log("online menu: starting the race\n");
            net_start_race();
        }
        break;
    }
}

static void page_back(MenuPage *p)
{
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
        hle_log("online menu: leaving the room\n");
        net_leave();
        go(HOME);
        break;
    }
}

static void page_step(MenuPage *p, int row, int by)
{
    if ((screen == NAME || screen == CODE) && row == 0) {
        int n = screen == NAME ? (int)sizeof name_letters - 1 : (int)sizeof code_letters - 1;
        entry_pick = (entry_pick + by + n) % n;
        menu_page_sound(by > 0);
    } else if (screen == ROOM && row == 1) {
        NetLobby l;
        net_get_lobby(&l);
        if (!l.is_host) return;
        room_track = (room_track + by + COUNT(tracks)) % COUNT(tracks);
        NetRoomSettings s = {tracks[room_track].index, -1, -1, -1, -1, NULL};
        net_set_room(&s);
        menu_page_sound(by > 0);
    } else if (screen == ROOM && row == 2) {
        char value[8];
        rider = (rider + by + COUNT(riders)) % COUNT(riders);
        net_pick(-1, riders[rider].index + 1, 1);
        net_set_ready(1);
        snprintf(value, sizeof value, "%d", riders[rider].index);
        save("rider", "DHD_NET_RIDER", value);
        menu_page_sound(by > 0);
    }
    (void)p;
}

static void page_left(MenuPage *p, int row) { page_step(p, row, -1); }
static void page_right(MenuPage *p, int row) { page_step(p, row, 1); }

/* The main menu's ONLINE row (patched func_001ED5B8): the page until it closes; 1 when a room's
   race starts (online.c has it from here), 0 when the player backed out. */
int port_online_menu(void)
{
    load_player();
    start_iso_check();
    page.build = page_build;
    page.update = page_update;
    page.accept = page_accept;
    page.back = page_back;
    page.left = page_left;
    page.right = page_right;
    page.title = "ONLINE";
    screen = HOME;
    home_row = 0;
    race = 0;
    status[0] = 0;
    menu_page_run(&page);
    return race;
}

/* The main menu's labels (patched func_001ED5B8): its table has three; the fourth is ONLINE. */
int port_main_label(const int *table, int index)
{
    return index < 3 ? table[index] : (int)(uintptr_t)"ONLINE";
}
