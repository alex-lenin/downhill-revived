# Opt-in hardware coverage bridge

Status: accepted production hardware bridge after independent raw-VRAM, live
state/image/depth validation and a measured throughput improvement. Hardware is
default-on on capable devices, with explicit-zero compute reference/fallback. This is a decoded-GS bridge, not game-level mesh submission.

## Bounded production path

`DHD_GS_HARDWARE=1` requests feature level 11_1 and enables a triangle-only
hardware coverage path when rasterizer-ordered views and conservative tier 3
are available. The option is on by default; explicit `0` selects compute. Unsupported devices retain
compute; unavailable hardware does not count as successful hardware validation.
The portable `gpu.h` exposes lifecycle, capture and raster statistics without
Direct3D types. `gpu_win32.h` contains device/presentation interop, and all
Direct3D objects remain private to the Windows backend.

Triangles use the existing normalized primitive buffer in submission order.
The vertex shader casts stored coordinate bits to signed integers, maps GS
12.4 positions to screen coordinates `position/16 + 0.5`, then to a fixed
2048x2048 viewport with power-of-two scale/offset. Eligibility requires each
post-XYOFFSET coordinate in [-65535,65535], each pairwise coordinate difference
at most 65535, and nonzero CPU int64 area. These operations are exactly
representable in float in this domain; no hardware attribute interpolation is
used. Conservative rasterization supplies a coverage superset. The pixel shader
calls the same `draw_pixel` used by compute, retaining its exact integer edges,
double depth plane, texturing/filtering, fog, alpha actions, blending and masks.
Both shader stages compile with strictness and IEEE strictness. Shader-source
sharing is not by itself a proof of identical compiled behavior; complete VRAM
and live state comparisons remain required.

Eligible framebuffer formats are CT32/CT24 and depth Z32/Z24. Scissor/stride
must prove injective pixel-to-word addresses; frame and depth page sets must
be disjoint even when Z writes/tests are disabled. Packed formats, aliasing,
other primitive types and unsupported coordinate bounds use compute for the
whole batch. This restriction matters because ROV order protects overlapping
screen pixels, not different screen pixels accessing the same VRAM word.

The path uses the existing 4 MB raw VRAM UAV and immutable texture snapshot
SRV. No decoded texture or render-target cache is introduced. Existing upload,
snapshot, local-copy, deferred-depth-readback and CPU synchronization rules are
retained. Graphics/compute UAV and shader-resource bindings are explicitly
cleared around transitions. Hardware depth/blending are disabled; GS operations
remain in the shared pixel function.

`gs_gpu_get_raster_stats` reports enabled/available flags, submitted hardware
and reference batch/primitive totals, and per-reason rejected batch counts.
`DHD_PROFILE` emits cumulative hardware counts so a bounded game capture can
prove actual path use even when its process is intentionally stopped. Counts
measure submissions, not GPU completion or an independent correctness proof.

## Initial build evidence

CPU compilation of the changed GPU and presentation units passes. CPU-only FXC
compilation passes all five entry points: compute main, scanout, local copy,
hardware vertex and hardware pixel (strict IEEE flags). The first frozen
candidate is `build/port/r28/hardware-bridge/dhd.exe`, SHA256
`dd655075f9eaa80f4fced69ad17cdb5a935589ef92a5e1c5b3fbb3c55a896c54`.
Its ignored build manifest records every linked object and affected source
hash. Only GPU/presentation objects replace the accepted round 27 object basis;
game O1/SSE2 and runtime precise-float settings are preserved. This is build
evidence, not yet a passing hardware correctness or performance claim.

## Research

Documentation consulted; no external implementation copied:

- [Microsoft conservative rasterization](https://learn.microsoft.com/en-us/windows/win32/direct3d11/conservative-rasterization): coverage superset, snapping and tier guarantees. Tier 3 and nondegenerate exact 12.4 positions bound this prototype.
- [Microsoft rasterizer-ordered views](https://learn.microsoft.com/en-us/windows/win32/direct3d11/rasterizer-order-views): ordered reads/writes for overlapping screen pixels, motivating the strict cross-pixel alias rejection.

The standalone synthetic probe and device-capability findings remain in finding
033. Independent production full-VRAM fixtures and the unchanged native
EE/image/depth comparisons are separate acceptance steps; synthetic coverage
alone does not establish this production path.

## Independent validation

`port/gs/test_hw_renderer.c`, built by `port/tools/test_hw_renderer.py`, tests
the actual production bridge against a separate compute instance and checks
every word of each raw 4 MB GPU VRAM resource, including seeded untouched bytes.
The final fixture run passes 77 comparisons (80,740,352 VRAM words), with zero
failures. Of these, 68 also use an independent CPU oracle: int64 edge/top-left
coverage, exact integer/dyadic color and depth arithmetic, masks and known
texels. Nine rich-shading or conservative alias-rejection cases compare the
retained compute renderer directly, without claiming a separate CPU shading
proof. No bytes or comparison tolerances are excluded.

Positive cases must increase actual hardware primitive submissions. The run
records 76 hardware batches / 277 primitives, 71 positive cases and six
explicit rejection cases, with seven reference batches including a mixed
point-rendering transition. A passing fallback cannot count as a positive
hardware test. Unsupported devices return a distinct unsupported status.

Coverage includes both windings and all 16 subpixel offsets; flat and dyadic
interpolated depth/color; CT32/CT24 and Z32/Z24; masks and alpha-failure actions;
64 ordered blend overlaps; known CT32/T8 texels; and perspective/IIP/fog/blend
triangles using CT32, T8 and T4 with nearest and linear filtering. Coordinates
at positive 65535, negative 65535 and clipped far viewport edges are checked.
Packed color/depth, page aliases, invalid stride, degenerate triangles and
full-2048 stride wrapping verify fallback. The richer filtered/perspective
cases retain full-VRAM compute-versus-hardware comparisons.

Coherence tests cover hardware framebuffer feedback through the shared texture
snapshot; hardware CLUT writes followed by a CPU CLD reload; CPU uploads after
hardware writes; hardware-to-compute-point-to-
hardware transitions; local copies; and deferred depth staging followed by
new writes. Deferred fetch must return the original issue-point bytes and
leave unstaged destination pages untouched. These tests exercise existing
coherence behavior rather than introducing a second resource cache.

The existing expanded `test_gpu.c` WARP suite also passes with the hardware
environment switch absent. Local artifacts are
`build/port/test-hw-renderer-raw/` and `build/port/r28/warp-reference-check/`.
The initial 76-case test compared the CPU VRAM view after normal synchronization;
review caught that this merges only predicted dirty pages and could miss stray
GPU writes outside those pages. That initial result was not treated as complete
raw-resource verification. A private `DHD_GS_TEST_BUILD` hook now copies and maps
all GPU bytes without a page filter; fixtures also assert that the normal CPU
view matches the raw resource. The strengthened 77-case run passes. Initial
artifacts are preserved. With the test macro absent, every production COFF
section and section header remains identical; only two object timestamp bytes
differ. The frozen game executable is unchanged. Proof is in
`build/port/r28/compile/raw-hook-compile-proof.json`.

`port/tools/gate_hw_renderer.py` imports the existing comparison functions and
uses the unchanged native reference/exclusions. One frozen executable runs
hardware off and on, keeping round 27 shader switches at 1/1/1 in both arms.
All six EE moments, six race BMPs, four menu BMPs and canonical early/menu
checks match. All 1,136 selected depth records match through virtual GS field
9019. Actual hardware-on race submissions total 1,861,723 batches / 127,448,631
primitives; menus also exercise 53,063 batches / 9,333,728 primitives. Thus this
is not merely a passing fallback run. Results are preserved in
`build/port/compare/r28-hardware1/result.json`. This is native-vs-native
validation, not a new full-game retail parity claim.

One sequential, bounded 160-second uncapped performance pair uses that same
frozen executable, default round 27 shader settings, identical virtual-clock
navigation and no timestamp diagnostics. External QPC field sampling measures
the same virtual fields 6000 through 7000:

| Hardware bridge | Elapsed seconds | Fields/second |
| --- | ---: | ---: |
| Off | 15.765033 | 63.43152 |
| On | 14.806066 | 67.53988 |

This is a 6.48% throughput increase in one paired measurement. The hardware-on
run records 1,817,914 actual hardware batches / 120,368,373 primitives; both
arms end by intentional bounded termination without a crash. Executable hashes
remain unchanged. Artifacts are
`build/port/r28/r28-hardware-speed1-summary.json` and
`build/port/r27/r28-hardware-speed1-{off,on}/`. Field endpoints are interpolated
from external 100 ms samples, so these are interval throughput estimates, not
per-present frame-time measurements. Wall-clock windows visit different
scenery and are secondary. One pair does not establish all-scene performance,
sustained delivery or the active 120 fps gameplay goal. The bridge was accepted
as the next baseline; default-on device verification is a separate final check.


## Active target and next baseline

The explicit user goal is sustained 120 fps actual gameplay on this PC. The
accepted bridge's 67.54 fps uncapped result is progress, not that outcome.
Simulation/render decoupling and game-level geometry remain parallel work;
production GPU work must target the next measured bottleneck while preserving
ordering, texture invalidation and unchanged exact comparison gates. Existing
round 27 and first bridge frozen artifacts are retained. No proprietary capture
or reference data is added to tracked files.


## Default-on baseline

`build/port/r28/accepted-hardware/dhd.exe` is the frozen default-on baseline,
SHA256 `bc4b624694dc131ecde273f1bb2ede8dd00c061f980dea8cdaaa750caabcf0f2`.
The default-absent raw fixture suite passes, proving actual hardware submissions
(76 batches / 277 primitives); default-absent WARP regression passes with zero
failures through unsupported-device compute fallback. Explicit hardware `0`
remains available. These default checks supplement the prior unchanged live
gate; they do not add a second live-game acceptance claim.

A separate frozen `build/port/r28/profile-hardware/dhd.exe` (SHA256
`63af7fcb2d4c8535f13df5e8116eb43ada3535abaa05efad87926e6944fc344c`)
adds hardware/compute path classifications to the existing sampled GPU timestamp
pairs. Appended CSV columns report each path's count and summed duration; the
original total remains unchanged. Query polling, 32-frame sampling and disjoint
rejection remain unchanged. This allows the next optimization to be chosen from
the measured bridge critical path.


## Bounded follow-up: hardware tile-list elision (candidate)

Inspection found that `flush` still constructs and uploads ordered compute tile
lists for hardware draws, although their pixel entry point reads primitives
directly and never reads the tile-list resource. The candidate computes batch
eligibility first and, only for an actual eligible hardware submission, skips
the two-pass tile-list build and bin-buffer upload. Every compute fallback
continues to construct and upload its full list. Primitive order, batch capacity
and membership limits, ownership proof, constants, snapshots and transfer
coherence remain unchanged. State-binding elision is deliberately separate.

`DHD_GS_HW_BIN_ELISION=1` enables the candidate, currently default-off. Frozen
`build/port/r28/hardware-bin-elision/dhd.exe`, SHA256
`cc1fcf989c423ade245b8c2e0b21e8d52d6b4b1f845b96091f87ed3def8cf1f6`,
contains both arms for exact and performance comparison. The pixel shader is
unchanged. Compilation and the independent 77-case raw-resource suite pass. Live
correctness and performance comparisons remain pending; the candidate stays
default-off. No speedup is claimed from source inspection alone.


## Zero-area eligibility: independently validated, gain provisional

The reason-weighted diagnostic attributes 3.315 ms per sampled frame to
compute batches rejected because one triangle member has zero area. This does
not mean the other triangles can be discarded. `DHD_GS_HW_ZERO_AREA=1` instead
keeps all members, their order and normal dirty/snapshot bookkeeping, allowing
otherwise eligible batches onto hardware. The shared exact pixel function
rejects zero-area members before VRAM access. The candidate remains default-off.

Independent tests now include zero-only repeated, collinear and coincident
vertices, plus mixed valid/zero triangles in different orders with overlapping
blend/depth operations. All 82 cases pass, comparing 85,983,232 raw VRAM words;
99 zero-area members are actually submitted to hardware. The reference keeps
the new eligibility disabled. Artifacts: `build/port/r28/zero-area-fixed-raw1/`.

An unaccepted texture-specialization scaffold initially broke the generic
shader: FXC preprocessing treated the negative macro sentinel as unsigned and
selected the wrong branch. Both compute and hardware then failed five existing
independent texture oracles, including a control with all candidates disabled.
The failed executable and logs are retained. An explicit defined-macro guard
fixes it; generic hardware and compute DXBC are byte-identical to the prior
baseline (168,684 and 162,476 bytes). Proofs are
`build/port/r28/preprocess-guard-proof.txt` and
`build/port/r28/generic-identity-proof.txt`. No failed executable was live-gated.

The corrected frozen executable is
`build/port/r28/hardware-zero-area-fixed/dhd.exe`, SHA256
`fac2b24893d3f49f94240d4aaac47dac0b247dfaddfbd904563e18eb3ae0c8b9`.
With hardware enabled and bin elision/texture specialization fixed at zero,
zero-area off/on passes all six EE moments, selected race/menu images and 1,132
exact depth records through GS field 9015, using unchanged comparisons and
exclusions. Actual zero-area hardware membership totals 2,554,679. Results:
`build/port/compare/r28-zero-area-fixed1/result.json`.

One quiet sequential 160-second uncapped pair measures common fields
6000 through 7000:

| Zero-area eligibility | Elapsed seconds | Fields/second |
| --- | ---: | ---: |
| Off | 15.542570 | 64.33943 |
| On | 15.281660 | 65.43792 |

The paired increase is 1.71%; both arms end intentionally without crashes.
One pair cannot distinguish this small gain confidently from run variation,
so this is a correctness-validated opt-in candidate, not an accepted default
performance improvement. The older 67.54 figure is from a different run and
is not the denominator for this experiment. Artifacts:
`build/port/r28/r28-zero-area-speed1-summary.json` and corresponding
`build/port/r27/r28-zero-area-speed1-{off,on}/` directories. These uncapped
capacity measurements do not establish correct-speed 120 fps gameplay.

The next measured GPU target is texture-format specialization: the diagnostic
attributes about 3.382 ms to T8 and 2.400 ms to T4 hardware draws per sampled
frame. Native VU arithmetic and simulation/render isolation remain separate
candidates. No combined gain is inferred from separate experiments.
