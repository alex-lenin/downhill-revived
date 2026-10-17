# Native geometry CPU investigation toward 120 fps

Status: bounded FMAC result-clamp candidate, compile-time opt-in and default off.
No performance or live correctness acceptance is claimed yet. Matching sources
and comparison exclusions are unchanged.

## Current measurement boundary

The six accepted native kernels remain enabled by mask 63. The rejected old
B0078 prototype is not a new candidate. Finding 032's pre-hardware worker
measurement was about 13.02 ms VIF CPU per field over its broader window;
overlapping worker times cannot be added to GPU time or treated as frame gains.
The first hardware profile's worker exports were empty because its export
deadline outlasted process lifetime. A coordinated replacement profile uses
the existing VU1 per-program CSV and `DHD_VU1_NATIVE_STATS=1`, plus worker exports
ending at host second 130. Cumulative per-key native counters report calls,
target calls, vertices and clipped vertices, not separate clipping durations.

The replacement profile covers 2,674 fields in host seconds 90..130. VIF worker
CPU is 13.364 ms/field and busy time 12.171 ms/field; GS worker CPU is 8.403
ms/field. Timed VU execution excluding GIF callbacks totals 7.828 ms/field,
with GIF queue/copy callbacks another 1.419 ms/field. These are instrumented
overlapping stage measurements, not additive frame costs. The roughly 4.12 ms
difference from VIF CPU includes packet processing, diagnostics and worker
bookkeeping; it is not an exclusive UNPACK attribution.

| Program / continuation | Calls/field | VU ms/field |
| --- | ---: | ---: |
| B0078 / 1040 | 595.78 | 2.60894 |
| B0078 / 1CB0 | 129.95 | 0.71289 |
| B1638 / 1040 | 113.82 | 0.60519 |
| B17E0 / 1040 | 91.44 | 0.53848 |
| B1638 / 1CB0 | 36.58 | 0.19720 |
| B17E0 / 1CB0 | 24.35 | 0.09762 |

The first four keys alone use 4.466 ms/field, supporting investigation of the
shared native arithmetic helper. Sources are the verifier's ignored
`build/port/r27/r28-hw-reasons-profile1/{vu1,vif,gs}.csv`; aggregate key results
are in `build/port/r28/native-geometry-profile.json`. Program timings include
their retained AOT setup/finish and clipping; none isolate the native loop.

## Bounded candidate and call-site audit

`vu1_native_geometry.h::native_result` currently applies the general
`arithmetic4` sanitizer after each native SIMD FMAC. Its callers are exclusively
`native_add`, `native_sub`, `native_mul`, `native_madd`, and the explicit SSE
cross-product subtraction in `native_geometry_loop`. None pass arbitrary raw
register bitcasts as a result. Operand sanitization remains unchanged.

Every production/replay/oracle invocation enters through `ps2_vu1_run`, which
sets MXCSR to toward-zero arithmetic, FTZ/DAZ, and masked host exceptions before
calling the interpreter/native kernels, and restores the caller's mode later.
Thus actual SSE FMAC results have already flushed subnormals to signed zero.
The opt-in `DHD_VU1_NATIVE_FMAC_CLAMP=1` build reuses the result exponent test
for the existing exponent-255 clamp and omits the redundant result-subnormal
path. Raw-result sign/zero/overflow accumulation and `last`/destination mask
remain before sanitization. Signed zero, saturation and operation order remain
part of the exact comparison. The macro defaults to zero; normal builds retain
the accepted implementation.

`test_native_fmac.c` includes the production implementation and compares its
actual helpers against the old general sanitizer plus independent scalar
flag-bit expectations. It covers all destination masks, seeded nonzero sticky
flags, signed zeros, subnormal operands, under/overflow boundaries, cancellation,
infinities and quiet/signalling NaNs of both signs, followed by deterministic
random mixed-lane inputs. Separate multiply/add ordering is retained. The test
does not assume arbitrary bitcasts are valid FMAC results.

The focused production-helper differential passes 4,760,896 cases with zero
failures. Output is retained in `build/port/native-fmac-test.log`; this is a
CPU arithmetic result/flag test, not game/image validation.

Candidate replay against exact AOT passes all original 32 and representative
32 captures: 128 GIF/persistent-state sections match, with positive actual
native calls for all six keys. The printed first checkpoints exercise clipped
vertices for keys 1, 2 and 4; no extra claim is made about per-capture clipping
coverage where the periodic counters do not print the final total. Evidence is
`build/port/r29-native/exact-replay/` and `r29-native-replay.log`.

Six short alternating standalone native-on samples use the 32 representative
captures, 3,000 repetitions each, after compilation and before releasing the
machine for GPU validation. Sum of per-capture VU-only means, microseconds:
baseline 165.176, candidate 159.627, candidate 154.823, baseline 172.088,
baseline 161.159, candidate 155.311. Median sums are 165.176 versus 155.311,
a 1.0635x replay ratio. This is about 5.97% less replay VU time, not measured
whole-game improvement. Individual dispatch families vary; 20C and 2FE medians
regress in this short sample, while 011/260/2C9 improve. No repeat campaign or
live acceptance follows automatically from the aggregate gain.

Source and standalone binary hashes are retained in
`build/port/r29-native/cpu-evidence-manifest.json`. Candidate bench SHA256 is
`8c4d789658dde98d31d1ebc73fa23f36284a6b56eaa9f21695e27c5c41bb521c`.
The opt-in remains off in normal game builds. Live EE/images/deferred Z and a
controlled whole-game performance comparison are still required before any
default change; they are queued behind the separately owned hardware work.

The result-only live candidate is frozen at
`build/port/r29-native/live-candidate/dhd.exe`, SHA256
`57683dd15f830141f76408e0c222d0d096bf984ce022a94ffe012fd402f7616f`.
Its manifest verifies all accepted hardware-basis objects before replacing
only the VU runtime object. This is build evidence, not live acceptance.

## Separate DAZ operand experiment

A separately authorized `DHD_VU1_NATIVE_FMAC_OPERANDS=1` build changes only the
operand sanitizers in `native_add/sub/mul/madd`, with the result-clamp candidate
explicitly zero. These sanitized operands are immediately consumed by SSE
addition, subtraction or multiplication. DAZ supplies their signed-zero
subnormal inputs, while exponent-255 saturation remains explicit. Loads,
bitcopies, integer/raw comparisons, minmax and raw flag inputs do not use this
new helper. The cross-product's manual operand sanitizers also remain unchanged.

The same production-helper differential has a separate operand mode; its old
reference continues to apply the full `arithmetic4` sanitizer before every
operation. Precise SSE2, no FMA and disabled contraction remain required. This
experiment is not bundled with the result-only candidate, and each compile
macro defaults to zero. Focused tests, exact captured replays and frozen builds
may run during a coordinated live correctness gate, but meaningful timing must
wait for an otherwise quiet machine.

Operand-only focused differential passes 4,760,896 cases; all 64 captures /
128 GIF and persistent-state sections match the exact AOT referee, with actual
native dispatches recorded. The separate live candidate is
`build/port/r29-native/live-operands-candidate/dhd.exe`, SHA256
`5bf9b03649bc358aceaaf1583f375bd6d93a6d29be9abc35dd5f7a45bfd24a17`.
Again, only the VU runtime object replaces the verified accepted hardware basis.
The emitted runtime contains separate SSE add/subtract/multiply instructions,
zero fused FMAs and zero x87 arithmetic instructions; precise SSE2 build flags
and the differential's old unfused reference preserve the operation-order
check. Evidence is `operand-focused.log`, `operand-exact-replay/`, the frozen
live manifest and `operand-instruction-audit.json` under `build/port/r29-native`.
This establishes local arithmetic/replay evidence, not live acceptance.
A reserved quiet three-way round gives median representative VU sums of
167.201 us baseline, 155.520 us result-only and 159.078 us operand-only.
The result-only path has the greater aggregate gain in that small round.

A separately frozen combined experiment also passes 4,760,896 pathological/
random cases and all 64 captured GIF/state replays. Its executable is
`build/port/r29-native/live-combined-candidate/dhd.exe`, SHA256
`1683fd2cfd20e9d9614a279dbb287f6e6f5bf3a4d0f5697ec982383e95dba0ab`.
Another reserved quiet nine-process round gives median sums of 177.019 us
baseline, 158.583 us result-only and 157.215 us combined. Combined is only
about 0.87% ahead of result-only in that round; gains are not assumed additive.
These short replay rounds vary and do not establish actual game improvement.
All three runtime candidates remain default off pending one independently
coordinated strict live gate and same-field performance comparison.

The focused tool now compiles the exact production helper text, extracted at
explicit function boundaries, without unrelated generated AOT code. Its
reference remains the old full sanitizer and independent scalar flag checks;
no handwritten duplicate of the candidate arithmetic is substituted.

The clipping program's broad VF inventory touches 28 of VF1..VF31; only
VF18/19/25 are absent. A simple union-based spill elimination would therefore
save little of the full register roundtrip. A more precise control-flow
read-before-def analysis could remove additional work, but no speedup or
complete callee read/write proof is claimed from this broad inventory.

## Concrete selector-3 static mesh foundation

Read-only analysis of the existing GO+60 EE capture finds an understood
type-1 root at `097BA7D0`, with one child at `097BA640`. The child is type 0,
with one material and format selector 3. `0022F600` maps that selector through
scratchpad slot `12C` to overlay-B entry `0948`. The signed descriptor at
resource offset `3C` has low byte -1 (inline chain) and high byte 6, selecting
the RET chain at resource offset `60`. The checked local example contains five
raw position triples and eight indexed input records. No captured vertex,
colour, UV or instruction blob is stored in tracked files.

The chain uploads a scalar position count at VU qword `B4`, V3-32 positions
at `B5`, a raw four-word header at `238`, and three interleaved input streams:
unsigned S8 indices at `239 + 3*i`, unsigned V4-8 RGBA at `23A + 3*i`, and
signed masked V2-16 UV at `23B + 3*i`. Indices use immediate USN bit 14;
the UV stream does not. CL=3/WL=1 establishes this stride. Initial mask
`50505050` supplies UV z/w from ROW. A later mask `BFBFBFBF` plus a masked
S8 command writes only UV w from COL[0] for two specified input records,
before restoring mask `50505050` and issuing MSCNT.

`initScratch` in `00215B68.c` supplies ROW `[0,0,1000,0]` (hex words) and
four COL words `800000`. B0948 converts the full UV vector with ITOF12,
adds UV w into the projected output w and uses FTOI4. Thus the canonical
COL w contribution becomes `8000`, which the existing packed GS parser
recognizes as ADC draw suppression. ADC advances strip history while
suppressing that draw; it must not be treated as an arbitrary strip reset.
The exercised native-output test below confirms this correspondence for the
audited example.

`port/native_mesh_decode.{h,c}` owns a strictly bounded decoder for this
selector-3, one-material, inline-RET form. Unknown commands, unsupported
descriptors, malformed/truncated counts, out-of-range indices and animated
callback flags fail closed. It preserves raw IEEE position words, typed
index/RGBA/UV inputs, the raw VU header, and symbolic COL-w patch metadata.
It owns an exact copy of the original chain. Its FNV hash is diagnostic;
cache reuse additionally checks every byte and the caller's resource identity,
generation and root/terminal addresses. Generation must come from an actual
resource allocation/archive lifetime, not a frame number or pointer guess.

Material IDs are provenance; TEX0/residency patches at resource `+50`, material
LRU changes in `0021B668`, matrices and ROW/COL values remain per pass and are
not cached. This component is unwired and exposes `topology_proven=0`.
No rendering path or geometry bypass may infer triangles from indices alone.

Synthetic owned-storage, raw-bit preservation, mutation/lifetime mismatch and
every-byte truncation tests pass; the existing captured example also decodes
successfully. Evidence is `build/port/r29-native/mesh-decode-test.log`.
The context lane's independently authored `test_mesh_vif_plan.py` passes 33
cases comparing the original production VIF parser with a manual decoded
write plan over all 16 KB, including untouched bytes, and the entire exported
UNPACK state. Trial zero uses the saved VU data with canonical ROW/COL; 32
additional cases poison data and ROW/COL. It does not execute MSCNT.

`test_native_mesh_output.py` then seeds both endpoints from the same saved
VU CPU/code and those independently produced data arrays. B0948 setup returns
the actual continuation **1CB0**, which is executed next. Complete persistent
CPU state (only the two host pointers normalized), all final VU memory, every
GIF byte and recorded draw vertices match. Positive coverage is one XGKICK,
four triangle draws and two ADC skips. The initial harness incorrectly expected
continuation 1040; it failed rather than continuing, and was corrected after
reading the actual control register and shared setup E/delay-slot boundary.
This is a local CPU/GIF proof, not GPU pixels or live readback acceptance.
Evidence is `build/port/mesh-vif-plan/` and
`build/port/r29-native/mesh-native-output.log`.

The private-context lane implemented a guarded cached upload of these owned
channels, bypassing a second VIF parser traversal while preserving the exact
UNPACK endpoint state. Its independent three-way original-VIF/manual-plan/
cached-upload proof passes all 33 full-memory/full-state cases; malformed
entry/count/mask inputs refuse before writes. Evidence is
`build/port/mesh-vif-plan-cached1/`. It remains a private diagnostic with no
canonical rendering integration.
The full B0948/1CB0 output harness was also rerun directly against that
cached uploader's saved data: complete CPU state, final memory, GIF bytes and
recorded draw vertices match, with four triangles and two ADC skips. Evidence
is `build/port/r29-native/mesh-cached-output.log`.
No decode-cache performance gain is claimed until a per-pass application is
wired and measured.

The FMAC candidates' remaining gates are frozen live EE/images/exact deferred-Z
and coordinated whole-game performance. Instruction count or replay speed
alone cannot justify retention. The mesh foundation separately requires an
exercised private per-pass submission before any frame gain can be claimed.

## Private transform and two-field bridge

The world-walk B7E8 path loads the node's original basis into VF14..16, its
first raw qword into VF3, position into VF5, camera offset from scratch `390`
into VF6 and far clip from `2DA0` into VF10 before entry `180`. View/cull
matrix rows reside in VU0 data qwords 1..6. It then loads the normal/light
basis at `2D60..2D80` into VF23..25 and calls `238`. The returned VF4 is
retained; the original node basis is restored into VF1..3 before `808`.
CC44 uploads VF1, VF2, VF3 and VF6 as VU1 qwords 0..3. Scale flag7 bit0 adds
an additional operation sequence and is deliberately unsupported initially.
The type1/flag7-bit4/VI2 cull predicate must also remain intact.

`native_scene_transform.{h,c}` now contains an unwired private helper that
owns copies of the CPU/code/data seed and accepts these explicit node/view
inputs. It runs those exact existing microprograms in order, without
algebraically cancelling the inverse/forward basis operations. Three closed,
branchless microcode ranges are checked against a byte-exact local generated
reference; absent reference refuses all seeds. No instruction data is tracked.
Failure and culling leave the caller's output unchanged. MatrixC0 remains
uninterpreted. The source and capture harness pass all eight saved 180 inputs,
with exact paired 238 boundaries found for four. The others still compare the
four output qwords with the original source-order entry sequence. Owned-seed
repeatability, byte-guard refusal, scale/cull transactional refusal and observer
recursion checks pass. Even a matching captured VU boundary is not sufficient to
claim a canonical SceneNode/CC44 endpoint association without matched capture
provenance.

`render_seam_scene_probe.{h,c}` supplies an opt-in EE-thread observer bridge.
It requires a unique enabled world-list node matching the canonical 180
header/position/basis, follows only unary type1 edges to a type0 terminal,
captures the actual normal basis at 238, and checks both canonical
808 input and private 808 output before exposing an endpoint. Two adjacent
fields are retained as owned copies. Alpha0/1 return exact endpoint qwords;
the midpoint interpolates node position and camera-offset xyz while retaining
current orientation/projection. This is bounded experimental correspondence,
not persistent resource lifetime discovery.
This transform association deliberately allows the hot selector6 path, which
still uses original geometry transport in the first prototype. It does not
broaden the cached decoder/uploader's selector3 contract.

The actual observer bridge passes a CPU integration smoke: a synthetic mapped
EE world list/resource chain seeded with saved VU input executes the real
180/238/808 path on two adjacent fields with node and camera motion. Unique
node/terminal selection, exact endpoint bytes, a distinct moving midpoint and
unchanged output on invalid alpha pass. Evidence is
`build/port/r29-native/scene-transform-test.log`. No GPU or canonical transferred
packet proof is claimed yet; the presentation lane owns that integration.
The private runner omits normal entry capture/observation, avoiding
`DHD_VU0_CAPTURE`'s possible read/sync of the live VU1 register file.

The result-only FMAC object has also been relinked, without recompiling other
objects, onto the exact frozen `fac2b248...` formats basis. The resulting
isolated candidate is `build/port/r29-native/result-on-formats/dhd.exe`, SHA256
`5efbcd66830bbb7bcf02515671e9a0a78277a6c63761f0012671e3e79dda8111`.
Its manifest records every basis object and the sole VU object replacement.
It remains default-off and awaits its own gameplay correctness/performance
comparison on whichever renderer basis the controlled measurements select.

## Research

Intel's primary [FTZ/DAZ documentation](https://www.intel.com/content/www/us/en/docs/dpcpp-cpp-compiler/developer-guide-reference/2025-0/set-the-ftz-and-daz-flags.html)
describes flushing subnormal results in MXCSR FTZ mode; the [Intel architecture
manual](https://cdrdv2-public.intel.com/874240/325462-090-sdm-vol-1-2abcd-3abcd-4.pdf)
documents masked SIMD underflow and MXCSR control. These support the narrow
precondition, not a change to the port's existing arithmetic contract. No
external implementation was copied. Tests still check the actual host behavior.
