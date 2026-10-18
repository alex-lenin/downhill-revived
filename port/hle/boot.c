/* The PC entry point: what the PS2's crt0 (_start) does, on the host (finding 020).

   _start clears .bss, sets up the main thread (SetupThread: stack at _stack, D_93CAE0) and the
   heap (SetupHeap at _end, D_8A0400), runs the SDK's startup (func_0025F0A0), then calls main
   (func_001B6708) with the argument block at D_0029E580 and exits with its result. On the host
   .bss is already zero (memory.c), and of the SDK's startup only the parts that aren't kernel
   patches run: func_0025F0A0 also copies code into kernel memory (func_0025EF90, func_0025F330,
   func_0025F1B0), which the native kernel (kernel.c) doesn't need. */
#include "hle.h"
#include "ps2asm.h"
#include "fp_trap.h"
#include "splash.h"

#include <stdio.h>
#include <stdlib.h>

extern char _gp[];
extern char D_93CAE0[];       /* _stack */
extern int32_t D_0029E580[];  /* argc, then argv[] */

void func_0025EEB8(void);     /* libc: the semaphores of its locks */
void func_0025B1A8(void);     /* libkernl: the thread behind the i* kernel calls */
int func_001B6708(int argc, char **argv);  /* main */
void func_0025F298(int code);  /* exit */

extern char D_8A0400[];       /* _end */

extern int32_t D_002837EC[];  /* .ctors: the count (or -1), then the C++ static constructors */

/* crtstuff's __do_global_ctors (func_001FB608, entered at func_001FB610 past padding), which
   __main (func_001FB6C0) runs once from main: the constructors, last first. */
void do_global_ctors(void) RETAIL(func_001FB608);
void do_global_ctors(void)
{
    int n = D_002837EC[0];
    if (n == -1)
        for (n = 0; D_002837EC[n + 1]; n++)
            ;
    for (; n > 0; n--)
        ((void (*)(void))(uintptr_t)D_002837EC[n])();
}

static void root(void)
{
    func_0025EEB8();
    func_0025B1A8();
    int code = func_001B6708(D_0029E580[0], (char **)&D_0029E580[1]);
    func_0025F298(code);
}

int main(int argc, char **argv)
{
    (void)argc, (void)argv;
    host_console_init();  /* no console window: where the log goes */
    update_save_environment();  /* as started, for starting an updated dhd.exe (update.c) */
    host_settings_load();  /* before the relaunch, so the child inherits the variables */
    splash_show();  /* the start window, until the game's window shows (splash.c) */
    if (!getenv("DHD_RELAUNCHED")) splash_step(4, "Checking for updates...");
    update_at_start();  /* first process: a newer release offered (dhd.ini auto_update) */
    if (!getenv("DHD_RELAUNCHED")) splash_step(25, "Checking your disc image...");
    if (host_disc_ready())  /* the disc image: asked once, here in the first process */
        return 0;  /* the player cancelled */
    if (!ps2_low_view_reserved()) {  /* first run: again, with the RAM's own addresses kept free */
        splash_step(40, "Starting the game...");
        return update_after_game(host_relaunch_reserving(PS2_LOW_VIEW, PS2_RAM_SIZE - PS2_LOW_VIEW));
    }
    splash_step(50, "Reading the game's data from your disc...");
    if (!ps2_memory_init() || !game_data_load())  /* the data image, from the player's disc */
        return 1;
    host_widescreen_init();  /* data patches, once the data image is in place */
    kernel_init();
    ps2_fp_trap_init();
    ps2_vu0_init();  /* the native VU0 microprograms (vu0_micro.c) */
    /* SetupThread(_gp, _stack, 0, ...): the stack grows down from _stack towards _end (the game's
       own heap starts just above _stack, func_0020B628) */
    kernel_run(root, (uint32_t)(uintptr_t)_gp, (uint32_t)(uintptr_t)D_8A0400,
               (uint32_t)(D_93CAE0 - D_8A0400));
    return 0;
}
