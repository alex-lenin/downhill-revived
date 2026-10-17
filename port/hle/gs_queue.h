#ifndef DHD_GS_QUEUE_H
#define DHD_GS_QUEUE_H
#include <stdint.h>
/* VIF-owned provenance travels with copied GIF data; GS never reads it live. */
typedef struct {
    unsigned entry, top, itop, pc, clip, qword, overlay, program;
    double seconds;
} GraphicsProvenance;
extern GraphicsProvenance graphics_provenance;
void graphics_gif_feed(unsigned path, const uint32_t *words, unsigned count);
int graphics_xgkick(const uint32_t *memory, unsigned qword);
void graphics_gs_wait(void);
#endif
