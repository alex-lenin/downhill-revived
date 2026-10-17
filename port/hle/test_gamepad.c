/* gamepad.c smoke test (finding 059): SDL3 starts, enumerates the controllers that are plugged in
   and maps each to a pad, without the game. With no controller it only shows that SDL runs.
       test_gamepad [seconds]    also prints each player's state while it changes
   Built by port/tools/test_runtime.py (run there for 0 seconds). */
#include "hle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void host_thread_name(const char *name) { (void)name; }

int main(int argc, char **argv)
{
    double seconds = argc > 1 ? atof(argv[1]) : 0;
    if (!gamepad_start()) {
        puts("test_gamepad: SDL didn't start");
        return 1;
    }
    int players = 0;
    HostPad last[4];
    memset(last, 0, sizeof last);
    for (int i = 0; i < 4; i++) {
        HostPad p;
        if (!gamepad_read(i, &p)) continue;
        players++;
        printf("player %d now: buttons %04X left %d,%d right %d,%d\n", i + 1, p.buttons, p.left_x, p.left_y,
               p.right_x, p.right_y);
    }
    printf("test_gamepad: SDL running, %d controller(s)\n", players);
    for (DWORD end = GetTickCount() + (DWORD)(seconds * 1000); GetTickCount() < end; Sleep(10))
        for (int i = 0; i < 4; i++) {
            HostPad p;
            if (gamepad_read(i, &p) && memcmp(&p, &last[i], sizeof p)) {
                printf("player %d: buttons %04X left %3d,%3d right %3d,%3d L2 %3d R2 %3d\n", i + 1, p.buttons,
                       p.left_x, p.left_y, p.right_x, p.right_y, p.pressure[10], p.pressure[11]);
                gamepad_vibrate(i, (p.buttons & PAD_CROSS) != 0, p.pressure[11]);  /* Cross / R2 rumble */
                last[i] = p;
            }
        }
    for (int i = 0; i < 4; i++) gamepad_vibrate(i, 0, 0);
    Sleep(50);
    return 0;
}
