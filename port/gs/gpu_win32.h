/* Windows device/presentation interop; not part of the portable GS interface. */
#ifndef PORT_GS_GPU_WIN32_H
#define PORT_GS_GPU_WIN32_H
#include "gpu.h"
#include <d3d11.h>
#include <dxgi.h>
int gs_gpu_init(Gs *g, ID3D11Device *device, ID3D11DeviceContext *context);
void gs_gpu_copy_display(Gs *g, ID3D11Texture2D *target);
/* Presentation (present.c): the swap chain's flags for ResizeBuffers, the frame-latency wait
   before drawing a picture, and the present itself (sync interval 0, tearing when allowed). */
UINT gs_swap_chain_flags(void);
void gs_frame_latency_wait(void);
HRESULT gs_present(IDXGISwapChain *chain);
/* The player whose split-screen quarter fills the window, 1-4; 0: the whole picture (present.h). */
extern volatile int gs_native_view;
/* DHD_RENDER=native: a render thread draws and presents each field (gpu_native.inc). */
int gs_gpu_native(const Gs *g);
void gs_native_vsync(Gs *g, IDXGISwapChain *swapchain, unsigned *width, unsigned *height, int resize);
void gs_native_bench(Gs *g, int end);  /* DHD_BENCH window start (0) and end (1) */
void gs_native_lock(Gs *g, int lock);  /* the render thread's context lock (EE thread) */
void gs_native_present_options(Gs *g);  /* DHD_PRESENT_FILTER and DHD_ASPECT read again */
/* The last field's picture at the renderer's resolution (DHD_SCALE), owned by the renderer. */
uint32_t *gs_native_picture(Gs *g, unsigned *width, unsigned *height);
/* The field being recorded shows this RGBA picture instead of the display circuit's (a movie,
   port/hle/movie.c), or the circuit again with NULL. Game thread, before gs_native_vsync. */
void gs_native_movie(Gs *g, const uint32_t *rgba, unsigned width, unsigned height);
#ifdef DHD_GS_TEST_BUILD
/* Diagnostic only: coherent raw GPU image, including unpredicted writes.
   Unlike g->sync this never filters pages or merges into g->vram. */
int gs_gpu_readback_all(Gs *g, uint32_t *image);
#endif
#endif
