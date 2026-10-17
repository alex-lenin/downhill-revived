#ifndef PORT_GS_GPU_H
#define PORT_GS_GPU_H
#include "gs.h"
void gs_gpu_destroy(Gs *g);
void gs_gpu_trace_present(Gs *g, int end, double wall);
void gs_gpu_profile(Gs *g, double seconds);
int gs_gpu_display(Gs *g, unsigned *width, unsigned *height);
int gs_gpu_capture(Gs *g, uint32_t *pixels);
/* Counts describe submissions, not completed GPU work. Unsupported batches use
   the ordered reference backend without changing their memory semantics. */
enum {
    GS_GPU_HW_DISABLED, GS_GPU_HW_UNAVAILABLE, GS_GPU_HW_FORMAT, GS_GPU_HW_ALIAS,
    GS_GPU_HW_PRIMITIVE, GS_GPU_HW_COORDINATE, GS_GPU_HW_DEGENERATE,
    GS_GPU_HW_REASON_COUNT
};
typedef struct {
    uint64_t hardware_batches, hardware_primitives;
    uint64_t reference_batches, reference_primitives;
    uint64_t rejected[GS_GPU_HW_REASON_COUNT];
    int hardware_enabled, hardware_available;
} GsGpuRasterStats;
void gs_gpu_get_raster_stats(const Gs *g, GsGpuRasterStats *out);
#endif
