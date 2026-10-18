/* The start window (splash.c): shown by the first dhd.exe until the game's window shows. */
#ifndef SPLASH_H
#define SPLASH_H
void splash_show(void);                         /* first process, at start */
void splash_step(int percent, const char *text);  /* either process: what is being done */
void splash_close(void);                        /* either process: the game's window is up */
#endif
