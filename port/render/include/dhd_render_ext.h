/* dhd_render.h's additions after the v1 freeze: exports the Rust renderer (crate dhd_render) has
   beyond the frozen v1 header. Each is optional for C, adds no type and
   changes no v1 struct; include it after dhd_render.h. port/web/build_graphics.py reads the
   DHDR_FN declarations of both headers.

   SP10 (docs/findings/090-speed.md): an indexed native picture. */
#ifndef DHD_RENDER_EXT_H
#define DHD_RENDER_EXT_H

#include "dhd_render.h"

#ifdef __cplusplus
extern "C" {
#endif

/* dhdr_native_picture for an indexed picture: each DRAW command's first and count are a range of
   indices[0, index_count) (in place of a range of vertices), and each index names one of the
   picture's vertex_count vertices. The triangles drawn, their order and their vertices' bytes are
   those of the expanded picture (vertices[indices[i]] for every i of the range), so the picture
   is the same. indices stays alive for the call. Returns as dhdr_native_picture. */
DHDR_FN(dhdr_native_picture_indexed) int dhdr_native_picture_indexed(DhdrNative *n, const DhdrPicture *p,
                                                                     const uint32_t *indices, uint32_t index_count,
                                                                     DhdrPictureResult *out);

#ifdef __cplusplus
}
#endif

#endif
