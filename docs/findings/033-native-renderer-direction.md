# Native renderer direction

Status: architecture decision and investigation, not an implemented hardware
backend. Windows first, with a portable rendering interface. The round 27
compute renderer remains the regression reference and compatibility fallback.

## Why start migration now

Round 27 improves matched uncapped throughput from 44.98 to 63.24 fps on the
RTX 3070, but this remains a compute rasterizer over swizzled GS memory. It
explicitly performs coverage, depth, texture addressing/filtering and blending.
This is useful for establishing behavior, but reaching approximately 60 fps
does not make it an appropriate final performance ceiling for this machine.
An initial 120 fps uncapped throughput target means an 8.33 ms budget, not a
guarantee that the current scene can reach it or that gameplay already supports
120 independent visual frames per second.

The accepted default build's final diagnostic run reaches 62.90 fps over fields
6000-7000. Sampled raster dispatches still cost about 13.59 ms. Over a broader
55-second window, the VIF worker also uses about 13.02 ms CPU time per field
(75.5% wall-time busy). These overlapping measurements cannot be added, and the
worker window is not identical to the GPU comparison interval. They nevertheless
show why the migration must eventually remove CPU-side PS2 geometry processing
as well as pixel-raster work; changing the pixel backend alone does not establish
an 8.33 ms frame budget.

Uncapped benchmarks retain the original virtual tick/delta and deterministic
inputs, but advance virtual time faster than wall time. Normal play must retain
the original simulation rate. Actual high-refresh gameplay requires a separate
render clock and interpolation, with physics, input, audio and replay behavior
checked independently.

## Migration boundary

The long-term renderer should consume game geometry, materials, textures and
camera state. A hardware backend receiving decoded GS primitives is an interim
bridge: it removes work from the compute rasterizer without first rewriting
the game engine, but still interprets PS2 rendering semantics.

Start with one measured hardware path using the existing Direct3D device.
Keep Direct3D resource types and shader compilation inside the Windows backend;
game-facing resource handles and draw descriptions must not expose COM types.
Build the portable interface around the first working path rather than invent
a large untested graphics abstraction. A later backend can implement the same
operations, while the PS2 adapter owns GS register interpretation and fallback
decisions. Keep the matching retail source/build independent of port changes.

Stages:

1. Finish round 27 state/image/depth validation and profile its remaining cost.
2. Prototype hardware rasterization on a bounded primitive/state subset. Compare
   full VRAM and depth against the reference, including shared edges, negative
   coordinates, scissor boundaries, overlapping primitives and masked writes.
3. Introduce versioned decoded GPU textures/render targets. Treat CPU uploads,
   palette changes, render-to-texture, local copies and game depth readbacks as
   explicit coherence events. Do not retain stale decoded textures.
4. Move mesh/material submission above GIF/VU packet generation as the game
   structures become understood. Reuse geometry and resource buffers instead of
   reconstructing PS2 pixel operations each frame.
5. Add independent presentation/interpolation only after rendering and gameplay
   are stable. Benchmark multiple courses, effects-heavy scenes and resolutions.

## Constraints to prove, not approximate away

Native rasterization does not automatically reproduce GS coverage, integer
depth interpolation, alpha-failure actions, channel masks, blend rounding or
framebuffer/depth aliasing. Current lens-flare depth readbacks affect game
behavior, so visual similarity alone is insufficient. Keep unsupported cases
on the reference path until a replacement passes the existing checks; do not
silently change tolerances or comparison exclusions.

Direct3D rasterizer-ordered views are a candidate for preserving ordered
programmable color/depth operations while hardware generates pixel invocations.
Their guarantee applies to overlapping screen pixels; it does not make arbitrary
aliased VRAM addresses safe across different pixels. Query device support and
retain the existing alias/ownership checks. Conservative rasterization could
provide a coverage superset followed by the existing exact edge test, but its
coordinate mapping and degenerate cases need tests before claiming equivalence.
These are prototype options, not yet the selected or verified implementation.

Actual hardware probes on the same RTX 3070 (10DE:2484, same adapter LUID)
successfully created Direct3D 11 devices at feature levels 12_1, 12_0, 11_1 and
11_0. OPTIONS2 reports ROV support and conservative rasterization tier 3 for
the first three, but neither for the current port's requested FL11_0 device.
Thus the adapter has these capabilities; initialization must request a suitable
feature level before querying them. Retain an FL11_0/reference fallback rather
than assuming all Windows GPUs support the proposed fast path. The probes did
not change production initialization. Local source/logs are under
`build/port/r27/query_d3d11_caps_levels.*` and `d3d11-capabilities-levels.txt`.

A standalone synthetic prototype then passes 1,516 checks comparing 12,419,072
readback words on the RTX 3070. It combines hardware conservative rasterization,
a half-pixel mapping for GS integer samples, an exact integer edge rejection,
and ordered per-pixel writes. An independent CPU int64 oracle checks coverage
and a noncommutative accumulated value/count, including one draw containing
overlapping primitives and separate ordered draws. Fixtures include shared
edges, subpixel/negative coordinates, clipping, thin and degenerate triangles.

This establishes limited feasibility, not a game backend or performance gain:
the viewport is 64x64, textures/depth/blending are absent, and each pixel owns
its own output word. Production coordinate ranges, interpolation, aliasing,
resources and synchronization still need validation. The first separate-draw
test exposed a primitive-indexing harness error; failed logs were preserved and
the corrected explicit base-index version passed. Source and full report remain
under `build/port/r28/hardware-probe/`.

The synthetic test is now durable in `port/gs/test_hw_coverage.c` and
`port/gs/test_hw_coverage.hlsl`, with `port/tools/test_hw_coverage.py` as its
standalone build/run entrypoint. A verification run of these durable sources
on the RTX 3070 at FL12_1 reports all 1,516 checks passing, zero failed checks,
and all 12,419,072 readback words compared. The original fixtures and independent
CPU oracle are retained. This remains a coverage/order test, not a production
backend or a performance result.

The runner discovers the compiler through `CLANG` or PATH, passes an absolute
shader path, and writes its binary, logs and status to
`build/port/test-hw-coverage/`. `--build-only` creates no device or GPU work.
Unsupported platforms, runtimes or hardware return status `unsupported` and
exit code 3, separately from PASS, comparison failure and execution errors.
The measured durable result is preserved in that directory's `run.log` and
`summary.json`; the initial prototype failures remain in the ignored prototype
directory.

Old local race traces suggest CT24/Z24 with indexed T8/T4 textures dominate
several captured scenes. That is useful for choosing a bounded initial path,
but those historical draw counts are not a current cost-weighted profile.

## Research

Documentation and architectural references only; no implementation copied:

- [Microsoft rasterizer-ordered views](https://learn.microsoft.com/en-us/windows/win32/direct3d11/rasterizer-order-views): ordered UAV access for overlapping pixel shader invocations and runtime capability query.
- [Microsoft conservative rasterization](https://learn.microsoft.com/en-us/windows/win32/direct3d11/conservative-rasterization): coverage guarantees, uncertainty regions and degenerate-triangle limitations.
- [Microsoft rasterizer state](https://learn.microsoft.com/en-us/windows/win32/api/d3d11_3/ns-d3d11_3-d3d11_rasterizer_desc2): conservative state and UAV-only rasterization requirements.
- [PCSX2 hardware renderer source](https://github.com/PCSX2/pcsx2/blob/master/pcsx2/GS/Renderers/HW/GSRendererHW.cpp): separate texture-cache and blend/depth compatibility handling remains necessary even in a hardware renderer. This is a reference, not a plan to duplicate a general PS2 emulator.
- [Vulkan fragment shader interlock](https://docs.vulkan.org/refpages/latest/refpages/source/VK_EXT_fragment_shader_interlock.html): a potential analogous capability for a future portable backend; support must be queried rather than assumed.
