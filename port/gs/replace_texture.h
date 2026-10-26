/* Replacement textures (texture packs, texpack.c; the sharp font, font_sharp.c) are made by the
   renderer that draws them (gpu_rust_native.inc; the browser's gpu_rust_web_native.inc). Both
   modules prepare an RGBA8 picture with its mip levels in memory; the renderer makes the texture. */
#ifndef PORT_GS_REPLACE_TEXTURE_H
#define PORT_GS_REPLACE_TEXTURE_H
#include <stdint.h>

typedef struct ReplaceDevice {
    /* A new texture, w x h with `levels` mip levels, each level's rows following the previous
       level's in pixels (halved sides, down to 1x1): a counted reference to what the renderer
       draws from, or NULL. Any thread (loader threads, the GS worker). */
    void *(*create)(void *context, const uint8_t *pixels, unsigned w, unsigned h, unsigned levels);
    void (*retain)(void *view);
    void (*release)(void *view);
    void *context;
} ReplaceDevice;

#endif
