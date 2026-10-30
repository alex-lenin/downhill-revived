/* Lobby compatibility follows the shared sources, not the platform's native executable.
   The wasm build generates online_build.h; a missing identity must fail compilation.
   Keep the release gate and disc-files check separate (finding 115). */
#include <stddef.h>
#include <stdio.h>
#include "../include/version.h"
#include "online_build.h"

void hle_online_version(char *out, size_t size)
{
    snprintf(out, size, "dhd-port " DHD_VERSION " " DHD_ONLINE_SOURCE_ID);
}
