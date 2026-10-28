/* What the OS files of the host (host_win32.c, host_posix.c) give the shared host files
   (host_sdl.c, port/hle/host.c) beyond host_api.h. Paths are UTF-8 on every host, as SDL's are. */
#ifndef PORT_HOST_OS_H
#define PORT_HOST_OS_H

#include <stdio.h>

/* A C library FILE for path, mode "r", "rb", "r+b", "w" or "w+b". A read shares writes and
   deletes on Windows as well, so a driver can replace a file (the autopad script) while the game
   reads it. */
FILE *host_os_fopen(const char *path, const char *mode);

/* 64-bit seek and position of a FILE: the C library's own names for them. */
#ifdef _WIN32
#define host_fseek _fseeki64
#define host_ftell _ftelli64
#else
#define host_fseek fseeko
#define host_ftell ftello
#endif

/* The frame waits' sleep (host_wait_wall): SDL_DelayNS, or on macOS one that its timer coalescing
   does not make late (host_posix.c). */
#ifdef __APPLE__
void host_os_sleep_ns(unsigned long long ns);
#else
#define host_os_sleep_ns SDL_DelayNS
#endif

#endif
