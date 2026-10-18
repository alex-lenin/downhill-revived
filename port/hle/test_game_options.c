/* Preference restore must never touch career bytes, and malformed files must
   leave every game byte unchanged. Build with game_options.c. */
#include "hle.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
unsigned char D_004225A0[0x12240], D_00398C40[1];
int D_0039E82C[1], D_0039E830[1];
static char saved[256];
static int apply_count, difficulty, vibration[8];
void hle_log(const char *fmt, ...) { (void)fmt; }
int host_setting_save(const char *key, const char *value) {
    assert(!strcmp(key,"game_options")); snprintf(saved,sizeof saved,"%s",value); return 1;
}
int host_game_options_read(char *value, size_t size) { snprintf(value,size,"%s",saved); return *saved != 0; }
void func_00195C40(void *o) { assert(o == D_004225A0+0x11A60); apply_count++; }
void func_0016C298(void *p,int d) { assert(p == D_00398C40); difficulty=d; }
int func_002205A0(int p) { return p*2; }
void func_00220198(int p,int v) { assert(p>=0 && p<8); vibration[p]=v; }
void port_game_options_apply(void), port_game_options_save(void);
int main(void) {
    unsigned char before[sizeof D_004225A0];
    memset(D_004225A0,0xA5,sizeof D_004225A0);
    memcpy(before,D_004225A0,sizeof before);
    port_game_options_apply(); assert(!memcmp(before,D_004225A0,sizeof before));
    strcpy(saved,"1 800 808 400 0 1 2 1 1 0 1 0 2 1 1 0");
    port_game_options_apply(); assert(apply_count==1 && difficulty==2);
    assert(vibration[0]==1 && vibration[2]==0 && vibration[4]==1 && vibration[6]==0);
    assert(!memcmp(before,D_004225A0,0x11A60));
    assert(!memcmp(before+0x11A88,D_004225A0+0x11A88,sizeof before-0x11A88));
    port_game_options_save(); assert(!strcmp(saved,"1 800 808 400 0 1 2 1 1 0 1 0 2 1 1 0"));
    memcpy(before,D_004225A0,sizeof before);
    strcpy(saved,"1 800 808 400 0 1 9 1 1 0 1 0 2 1 1 0");
    port_game_options_apply(); assert(!memcmp(before,D_004225A0,sizeof before));
    strcpy(saved,"1 800 808 400 0 1 2 1 1 0 1 0 2 1 1 0 trailing");
    port_game_options_apply(); assert(!memcmp(before,D_004225A0,sizeof before));
    _putenv_s("DHD_SETTINGS","none");
    strcpy(saved,"1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    port_game_options_apply(); port_game_options_save();
    assert(!memcmp(before,D_004225A0,sizeof before));
    assert(!strcmp(saved,"1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"));
    puts("game options: round trip, career isolation, malformed input, tool bypass PASS");
    return 0;
}
