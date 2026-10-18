/* Player preferences, separate from the memory card and its career progress.
   Called on the game thread only, after boot/load and accepted option pages. */
#include "hle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned char D_004225A0[];
extern int D_0039E82C[], D_0039E830[];
void func_00195C40(void *);
void func_0016C298(void *, int);
extern unsigned char D_00398C40[];
int func_002205A0(int);
void func_00220198(int, int);
int host_game_options_read(char *, size_t);

typedef struct {
    int speech, effects, music;
    unsigned char scheme[4], vibration[4], difficulty;
    unsigned char unused[18], stereo;
} Options;
_Static_assert(sizeof(Options) == 40, "retail options layout");

static int enabled(void)
{
    const char *s = getenv("DHD_SETTINGS");
    return !s || (_stricmp(s, "none") && strcmp(s, "0"));
}

void port_game_options_save(void)
{
    if (!enabled()) return;
    Options *o = (Options *)(D_004225A0 + 0x11A60);
    char value[256];
    snprintf(value, sizeof value, "1 %d %d %d %u %u %u %u %u %u %u %u %u %u %d %d",
        o->speech, o->effects, o->music, o->scheme[0], o->scheme[1], o->scheme[2], o->scheme[3],
        o->vibration[0], o->vibration[1], o->vibration[2], o->vibration[3], o->difficulty, o->stereo,
        D_0039E82C[0], D_0039E830[0]);
    if (!host_setting_save("game_options", value)) hle_log("options: could not save preferences\n");
    else hle_log("options: saved %s\n", value);
}

void port_game_options_apply(void)
{
    if (!enabled()) return;
    char text[256], extra;
    int v[16];
    if (!host_game_options_read(text, sizeof text)) return;
    if (sscanf(text, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %c",
        &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8], &v[9], &v[10], &v[11],
        &v[12], &v[13], &v[14], &v[15], &extra) != 16 || v[0] != 1) return;
    for (int i = 1; i <= 3; i++) if (v[i] < 0 || v[i] > 1032) return;
    for (int i = 4; i <= 7; i++) if (v[i] < 0 || v[i] > 2) return;
    for (int i = 8; i <= 15; i++) if (v[i] < 0 || v[i] > (i == 12 ? 2 : 1)) return;
    Options *o = (Options *)(D_004225A0 + 0x11A60);
    o->speech = v[1]; o->effects = v[2]; o->music = v[3];
    for (int i = 0; i < 4; i++) {
        o->scheme[i] = v[4+i]; o->vibration[i] = v[8+i];
        func_00220198(func_002205A0(i), o->vibration[i] != 0);
    }
    o->difficulty = v[12]; o->stereo = v[13];
    D_0039E82C[0] = v[14]; D_0039E830[0] = v[15];
    func_00195C40(o);
    func_0016C298(D_00398C40, o->difficulty);
    hle_log("options: restored %s\n", text);
}

void port_game_options_boot(void)
{
    static int done;
    if (!done) { done = 1; port_game_options_apply(); }
}
