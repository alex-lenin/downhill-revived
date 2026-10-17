# Frame critical path investigation

## Objective and starting evidence (2026-10-15)

Target: sustained 59.94 rendered game frames/s, about 16.683 ms/frame, with
the existing state, image, readback and retail-image checks unchanged.
Finding 031's accepted flip build averages 47.8 fps and falls to about 44 fps
late in the race. The late workload therefore needs roughly 6 ms/frame removed;
improving only a menu or the aggregate race average does not establish success.

The readback bypass barely changed late throughput. Its 17 ms of observed
waiting is consequently not 17 ms of removable work. VIF busy time is about
52% of wall time and GS busy time about 30%; neither proves GPU utilization.
CPU stages can be idle because they depend on earlier GPU output, or the GPU
can be idle because CPU submission is intermittent. Overlapping waits must
not be added to worker execution time as independent frame costs.

## Measurement design

- Record GPU timestamp intervals with a valid timestamp-disjoint frequency,
  together with CPU QPC spans, field identifiers and dispatch/upload counts.
- Poll a ring of old queries with DONOTFLUSH. Do not wait for query results or
  inject extra Flush calls. Explicitly report missing/overflow/disjoint samples.
- A GPU timestamp interval spanning many submissions includes starvation gaps;
  it is not pure GPU busy time. Sample individual raster dispatch intervals to
  distinguish that case from expensive shader execution.
- Measure instrumentation overhead against the frozen accepted executable.
- Retain legacy wall-window statistics, but also compare a common virtual-field
  interval: different-speed runs otherwise reach different scenery at the same
  wall time. Final proof must include frame-time distribution and late gameplay,
  not just an overall mean. Keep benchmark runs free of concurrent builds.

## Source-derived candidates, not measured results

1. `port/gs/raster.hlsl` computes depth, then texture sampling, fog and alpha
   before rejecting a fragment against Z. Earlier Z rejection could avoid work
   on occluded pixels without changing the interpolation arithmetic. It must
   preserve all alpha-failure/write-mask behavior for surviving pixels. Restrict
   the candidate to batches for which `independent_pixels()` proves no
   cross-pixel memory dependencies; unsafe cases retain the original ordering.
   The texture source is an immutable snapshot during a dispatch. A default-off
   prototype and adversarial GPU regressions are being prepared.
2. `write_pixel()` uses an atomic compare/exchange loop even for 32-bit words
   exclusively owned by one pixel thread. A future guarded ordinary store could
   avoid atomics where the same independence proof applies. Packed subword
   formats and aliased surfaces must retain atomic behavior. Not implemented.
3. Per-pixel interpolation uses double precision for nonconstant Z. Replacing
   that with ordinary float is not acceptable: it loses integer depth precision.
   Any specialization must retain exact results and demonstrate a measured gain.
4. `address()` computes unsigned division/remainder by page widths/heights
   selected from powers of two (64/128 and 32/64/128). Explicit masks and
   shifts are mathematically identical, including unsigned arithmetic wrap.
   Inspect compiled shader instructions before assuming the compiler already
   reduces these operations. A separate candidate would preserve all format
   selection, column/block swizzle, base-pointer addition and final wrap bits.

No candidate is accepted yet. No approximation or exclusion change is authorized
by this investigation.

## Initial late-race measurement

The first instrumented run uses the NVIDIA RTX 3070 (DXGI vendor 10DE,
device 2484). Wall 120-150 s contains 1,329 frames in 29.995 s, 44.31 fps.
GPU render intervals average 22.517 ms; 41 detailed frames have a mean
raster-dispatch sum of 20.326 ms (median 20.272), approximately 325 dispatches
and 22,148 primitives per frame. No VRAM uploads occur in this interval.
Scanout averages 0.0135 ms and back-buffer copy 0.0061 ms. CPU Present
averages 0.072 ms. The main-thread frame profiler records only 0.008166 s
total pacing time over the entire 30-second interval.

Detailed sampled frames are about 2.95% longer than neighboring coarse frames,
roughly 0.66 ms per sampled frame. Even subtracting that whole difference
from raster time leaves about 19.7 ms, above the 16.683 ms target. These
measurements support shader work as the next target, rather than more VU
compiler trials or removing readback waits. They do not imply CPU costs
disappear after the GPU gets faster.

Artifacts are ignored under `build/port/r27` and
`build/port/speed/r27-instrumented`. A control with this same executable and
profiling disabled completed: common virtual fields 6000-7000 take 22.329517 s
with profiling off and 22.329048 s with profiling on (44.7838/44.7847 fps).
There is no measurable aggregate overhead in this single pair; this is not
a statistical bound. External field sampling uses 100 ms intervals with
interpolation. The frozen r26 executable takes 22.336691 s for that interval.
Its original object response file is unavailable, so only the same-executable
pair isolates instrumentation overhead. Bounded runs are intentionally ended
with taskkill; that termination is not an application crash.

## Shader prototype checks

FXC disassembly of the original main shader contains 69 unsigned divide
instructions, including dynamic page addressing. The first mask/shift rewrite
failed T8/T4 local-copy tests. A direct GPU address probe also observed a
compiled-result mismatch (PSMT8 x=64,y=0: old 0x00808000, candidate 0x00800000).
The rejected source and disassembly remain ignored under
`build/port/tmp/address_probe_rejected_mask.*`; no general compiler-bug claim
is made from this isolated failure.

The corrected transformation omits intra-page modulo entirely: every block
and column expression already selects only the relevant low coordinate bits.
T8 block indexing uses x bits 4-6/y bits 4-5, T4 uses x bits 5-6/y bits 4-6,
16-bit formats use x bits 4-5/y bits 3-5, and 32-bit layouts use x bits 3-5/y
bits 3-4. Column expressions use lower bits. Page selection still uses the
original coordinates with exact power-of-two shifts, preserving the original
unsigned multiply before shifting the buffer width.

Two direct WARP address probes compare all 64 format codes: 4,194,304 low
coordinate cases and 4,194,304 full-uint coordinate/width cases, with zero
old-versus-new mismatches. The corrected shift-only GPU regression suite,
including packed local copies and scanout, passes with zero failures. The
early-depth-only suite also passes, including the new full-VRAM depth/alpha
write-mask oracle. These tests do not replace the live image/state/readback
gates or establish a performance gain.

## First same-workload performance comparison

One frozen candidate executable, with profiling disabled and explicit shader
toggles, ran each arm sequentially for 160 seconds. The comparison interval is
virtual fields 6000-7000, sampled through a cached process handle every 100 ms
and interpolated at the endpoints. This avoids comparing different scenery
when a faster executable advances farther during the same wall-clock window.

| Variant | Seconds for 1,000 fields | FPS | Legacy race average FPS |
| --- | ---: | ---: | ---: |
| Original shaders | 22.295851 | 44.8514 | 47.7833 |
| Address shifts | 20.338235 | 49.1685 | 51.4383 |
| Early depth | 17.275207 | 57.8864 | 58.2825 |
| Address shifts + early depth | 16.868997 | 59.2804 | 59.7233 |

These are first-run measurements, not yet accepted defaults. The combined
candidate improves same-workload throughput by 32.17%, but remains below
59.94 fps and has visible timing dips. All completed arms ended at the intended
bound without a reported crash. Extended state, image and depth validation is
still required. The earlier four `g+0,2,12,16` capture moments are race clock
ticks, not seconds; the new gate also captures ticks 3600 and 4500 (60 and
75 seconds) and compares every selected image and state.

Subsequent throughput trials use `DHD_FRAME_CLOCK=1` to remove the wall-time
limiter while preserving the same virtual frame clock, simulation deltas,
seed and field-indexed input. `DHD_PACE=wall` is not used: it changes the
simulation clock. Present already uses sync interval zero. Capped tests remain
necessary to establish delivered 59.94 Hz performance; uncapped same-field
measurements expose headroom hidden by that cap.

The first uncapped same-field pair takes 22.231043 s with original shaders
(44.9822 fps) and 16.251359 s with address shifts plus early depth (61.5333 fps).
That is a 36.80% throughput gain and about 2.66% average headroom over 59.94 Hz.
It does not establish that every frame meets the deadline. The same frozen
executable and explicit store-off setting are used for both arms.

Adding the guarded exclusive word store takes 15.812537 s (63.2410 fps), a
further 2.775% throughput gain over the two-change variant and 40.59% over
original shaders. All three runs are uncapped. The additional average margin
is useful, but final normal-build correctness and frame-delivery checks are
still pending; this is not a claim about every frame or every race course.

## Exclusive word-store prototype

The next prototype replaces compare/exchange with an ordinary masked word
store only for canonical CT32/CT24/Z32/Z24, and only under the existing host
`independent_pixels` proof. The scissor/stride bounds prevent page self-aliasing;
the swizzle maps each pixel to a distinct word, and the color/depth page sets
are disjoint whenever depth can be written. Ordered primitives for one pixel
remain in one invocation. Packed formats retain atomics, as do all local-copy
calls regardless of stale raster constants. Texture reads use a separate
snapshot resource. No ordering barrier or approximate arithmetic is added.

Expanded WARP tests pass with switches 000, 111 and 001: analytical full-VRAM
checks cover CT24 upper-byte preservation, CT16/CT32 mixed with all four Z
formats, no/partial/full framebuffer masks, alpha failure modes and a
64-primitive overlapping blend/depth sequence. DXBC has two guarded raster
stores with atomic fallbacks; the local-copy shader has no ordinary store and
retains compare/exchange. Live correctness and measured gains remain pending.

## Extended normal-build gate: not accepted yet

The normal-runtime build passes the expanded WARP suite with all three options
unset (temporarily default-on). The six-moment live gate then passes all early
canonical EE checks, all six pairwise race BMPs, four pairwise/canonical menu
BMPs, and 1,130 exact Z readbacks through virtual GS field 9013. However, the
late EE comparison fails under the unchanged exclusions: GO+60 seconds differs
at 0x2D5CC4, and GO+75 seconds differs there and at 0x4C9B94. Defaults were
restored off pending investigation; the speed gain is not yet accepted.

The failed runs, normal executable and result.json remain under
`build/port/r27/gate-final`. No comparisons were weakened or failures discarded.
The second address lies in the sound command buffer D_004C9B00; that identifies
storage, not the cause. A private write-watch diagnostic is being prepared to
identify the producers of both words before deciding on any fix.

## Research

Microsoft's [RWByteAddressBuffer documentation](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/sm5-object-rwbyteaddressbuffer)
describes raw stores and coherence. Its [interlocked operation documentation](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/interlockedadd)
states these operations do not imply a memory barrier. Those facts distinguish
atomicity from visibility; the safety argument depends on exclusive word
ownership, not on removing a barrier. No external implementation was copied.

Microsoft's [timestamp-disjoint query documentation](https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_query_data_timestamp_disjoint)
requires a non-disjoint interval and its reported frequency for reliable GPU
elapsed time. This informs the measurement design; no external code copied.
Microsoft's [HLSL optimization guidance](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-optimize)
discusses skipping work for invisible pixels. Its [unsigned divide instruction](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/udiv--sm4---asm-)
documentation identifies the DXBC operation to check when inspecting page
address arithmetic. These are investigation references, not measured gains.

## Accepted exact shader path after producer audit

The late EE failures above were traced to undefined native producer bytes,
not discarded. Finding 034 records the write watches, four port-only producer
fixes, focused poisoned-stack tests and the complete old-reference delta
audit. With those producers defined, original shader switches 000 and
optimized 111 match all six EE moments (including GO+60/+75 seconds), all six
race images, four menu images and 1,137 exact Z records through field 9020.
Old canonical images also match. The old EE reference FAIL remains preserved;
a separately reviewed native reference refresh passes unchanged checkers.

All three shader options now default on in the runtime: address shifts,
independence-guarded early depth, and exclusive canonical word stores.
Explicit `DHD_GS_ADDRESS_SHIFT=0`, `DHD_GS_EARLY_DEPTH=0` and
`DHD_GS_EXCLUSIVE_WORD_STORE=0` retain their individual original paths. The
standalone HLSL macros still default to zero. No guest arithmetic or packed
pixel atomicity is approximated. `gate_gs_optimization.py` selects the audited
`build/port/compare/r27-defined-native` reference by default and retains the
same imported EE/readback checkers.

The normal default-on build is `build/port/r27/accepted/dhd.exe`, SHA-256
`7cef7b180664fa5e9d378913f8b0363b558e04f1dce5e2ab7e182abbfaf9a411`.
It preserves the four fixed O1/SSE2 game objects, recompiles only the GPU
runtime object from the gated normal basis, and contains no frame-profile
build instrumentation. Its expanded WARP test passes with all three options
unset. Final paired performance measurements on this accepted executable
follow; earlier uncapped 63.24 fps figures are candidate measurements, not a
claim that every accepted frame meets a 59.94 Hz deadline.

## Final accepted-build measurements

Two sequential 160-second runs use the frozen accepted executable above with
all shader overrides absent. The uncapped run enables GPU/VIF/GS diagnostics;
the paced run disables them. Both retain the virtual simulation clock and end
at their intentional bound, with no reported application crash.

| Accepted default build, fields 6000-7000 | Seconds | FPS |
| --- | ---: | ---: |
| Uncapped, exact CPU present endpoints | 15.898491 | 62.8991 |
| Uncapped, 100 ms field-counter interpolation | 15.904685 | 62.8746 |
| Paced, 100 ms field-counter interpolation | 16.684826 | 59.9347 |

The paced legacy race and late-window averages are both 59.935 fps. The earlier
original-shader uncapped control was 44.9822 fps; the accepted result is about
40% faster, but this final run also contains the audited producer fixes and
optional query instrumentation, so it is not a new same-executable ablation.

Average throughput is not a deadline guarantee. Uncapped present-to-present
intervals have p95 17.462 ms and p99 21.649 ms. Of 1,000 intervals, 166 exceed
the 16.683 ms budget. Excluding the 31 detailed-query intervals leaves 969:
p95 17.481 ms, p99 21.663 ms and 165 above budget. Excluding their two adjacent
neighbors on either side still leaves 140 misses among 845 intervals. The
tails therefore persist beyond the query-heavy frames; this does not prove
every normal paced frame meets its deadline, nor cover other courses.

All GPU intervals are non-disjoint. The 31 sampled raster sums average
13.593 ms; the full GPU render interval averages 15.817 ms and the CPU Present
call 0.064 ms. Sampled intervals are about 4.6% longer than neighboring GPU
intervals, so the raster number is instrumented rather than an unperturbed
absolute cost. The common interval averages 323 raster dispatches and 21,929
primitives per frame, with no VRAM uploads counted.

Over the broader 90-145 second worker window (3,526 fields), VIF is 75.49%
busy and uses about 13.02 ms CPU time per field; GS is 42.91% busy and uses
8.20 ms CPU per field. These overlapping worker times are not additive and
their window differs from fields 6000-7000. Both raster and geometry work need
attention for an 8.33 ms / 120 fps target. Finding 033 records the hardware
renderer migration, eventually above PS2 geometry/packet processing.

Reproducible local evidence: `build/port/r27/r27-accepted1-measurements.json`,
`r27-accepted1-profile/optimized-summary.json`, and their retained CSV/logs.
No further tuning or repeated sampling was performed after these two runs.
