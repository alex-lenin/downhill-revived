/* Texture replacement packs where there is no image decoder (headless
   Linux): the pack folder is found the same way texpack.c finds it, but
   nothing decodes or uploads. texpack_open returns 0, so no caller may use
   views or finds; texpack_available reports whether a pack folder exists,
   exactly as on Windows before its device is created. Interface: the five
   symbols the shared code uses (texpack.h's device-typed ones with an
   opaque handle). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "gs.h"
#include "../host/host_api.h" /* host_exe_path */

typedef struct TexPackEntry TexPackEntry;

static int is_dir(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

/* The pack's folder: DHD_TEXTURE_PACK (the replacements folder, or a folder
   holding replacements/ or SCUS-97177/replacements/, as PCSX2's textures
   folder does), else textures/SCUS-97177/replacements beside dhd64. */
static int pack_find_root(char root[4096], int quiet)
{
    static const char *const below[] = {"", "/replacements", "/SCUS-97177/replacements"};
    char base[4096], tried[4352];
    const char *named = getenv("DHD_TEXTURE_PACK");
    if (named && *named) {
        snprintf(base, sizeof base, "%s", named);
        size_t n = strlen(base);
        while (n > 3 && (base[n - 1] == '\\' || base[n - 1] == '/')) base[--n] = 0;
        for (int i = 2; i >= 0; i--) {
            snprintf(tried, sizeof tried, "%s%s", base, below[i]);
            if (is_dir(tried)) {
                snprintf(root, 4096, "%s", tried);
                return 1;
            }
        }
        if (!quiet) fprintf(stderr, "textures: pack folder %s not found (texture_pack in dhd.ini)\n", named);
        return 0;
    }
    unsigned n = host_exe_path(base, sizeof base);
    while (n && base[n - 1] != '\\' && base[n - 1] != '/') n--;
    base[n] = 0;
    snprintf(tried, sizeof tried, "%stextures/SCUS-97177/replacements", base);
    if (is_dir(tried)) {
        snprintf(root, 4096, "%s", tried);
        return 1;
    }
    return 0;
}

int texpack_available(void)
{
    char root[4096];
    return pack_find_root(root, 1);
}

/* No image decoder on this host: nothing loads. */
int texpack_open(const void *device, uint64_t video_memory)
{
    (void)device;
    (void)video_memory;
    return 0;
}

TexPackEntry *texpack_find(const Gs *g, unsigned ctxt)
{
    (void)g;
    (void)ctxt;
    return NULL;
}

void *texpack_view(TexPackEntry *e, uint64_t field)
{
    (void)e;
    (void)field;
    return NULL;
}

void texpack_field(uint64_t field) { (void)field; }

void texpack_report(void) {}

int host_service_texpack_available(void) { return texpack_available(); }
