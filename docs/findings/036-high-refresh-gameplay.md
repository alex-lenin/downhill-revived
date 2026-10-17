# Simulation and rendering at different rates

Status: source investigation and isolated diagnostic tooling. No high-refresh
production path or 120 fps gameplay claim. Matching `src/` is unchanged.

## Clock and submission boundaries

`port/hle/kernel.c::vsync()` advances one NTSC field at 60000/1001 Hz. It
waits for graphics, raises the GS field, polls pads, runs `sound_tick()`,
delivers VBLANK handlers and presents the selected display buffer. The
normal virtual clock is paced against wall time. `DHD_FRAME_CLOCK=1` removes
that pacing; throughput from this mode does not mean real-time gameplay at
the reported rate.

`port/hle/sound.c::sound_tick()` renders exactly 4004/5 samples per field
at 48 kHz. `func_0019EE98` advances the race-start timer with
`func_00238F60()` (the integer game delta). Doubling VBLANK, input polling,
audio ticks or game delta consumption changes game behavior. The simulation
clock must remain 60000/1001 Hz. A nominal two-render schedule is
120000/1001 = 119.880 Hz; exactly 120 presentations per wall second requires
an independent render clock with a slowly changing interpolation fraction.

`port/gs/present.c::gs_vsync()` scans the current GS buffer and calls
`Present(0,0)`. Additional calls there alone do not create new geometry or
new visual states. Its display-buffer-change counter also cannot prove an
interpolated camera/rider pose or physical monitor delivery.

The current game-side split is:

1. `001B4DD0.c::func_001B4DD0`: prepare frame and invoke `func_00215B68`.
2. `001B5130.cpp::Obj::func_001B5130`: wait for render, invoke
   `func_002161B8`, finish other passes, run a virtual callback, increment
   `D_0029E10C` and call `00238C00`/`00238D98` (field synchronization).

The engine functions are in the merged `src/unsorted/00215B68.c` unit.
Both `00215B68` and `002161B8` initialize scratchpad packet headers, global
packet pointers and view/matrix storage, call a per-view callback
`D_0029DA94`, walk geometry, mutate packet cursors and submit DMA. Neither is
a pure render function. The outer finish function must not run twice.

## Stable geometry identities and mutation hazards

`include/scene.h` identifies a 0x100-byte `SceneNode`: resource identity at
+0, position at +0x10, matrix at +0x40 and another matrix at +0xC0.
`002167C0` walks list `D_004D0318`; `00216980` walks `D_004D0338`.
Each list contains stable link pointers to scene object pointers. Object
pointer plus resource identity is a useful candidate identity, but allocation
reuse and spawn/despawn require a generation/reset rule. Pointer equality by
itself is not proof of the same lifetime.

Both walks clear visibility bits in `node->x5`; flagged nodes can invoke
`00216AE0`, and culling/scene emission uses `0021B7E8`/`0021B8A8`.
Additional render state includes `D_0029DAA4`, scratchpad matrices,
packet buffers/cursors and per-view state. Repeating these calls against live
simulation state can consume flags twice, reuse in-flight packets and change
feedback timing. Whole-EE-memory rollback is unsuitable: workers and the game
can legitimately modify that memory concurrently.

Lens-flare store-image reads are a further boundary. Extra render frames must
use separate color/depth/output resources and must not issue or overwrite the
simulation's canonical deferred readbacks. HUD integer clocks and particle
birth/death/RNG advance once per simulation tick. Initially their existing
visual state can be drawn at each presentation; eventual smooth particles
need render-only age/pose evaluation, without a second producer update.

## Isolated diagnostic implementation

`port/tools/render_seam_audit.py` inventories the six selected game/engine
boundaries, with direct named calls and source-line write-review candidates.
It explicitly does not claim alias, callback, macro, assembly or callee purity.

`render_seam_inject.py` wraps four definitions in a private already-transformed
`00215B68.c`: prepare, finish and both scene-list walks. `render_seam_build.py`
checks every frozen object hash, rebuilds only that TU with the normal
O1/SSE2/EE-float/quadword-alignment workflow, adds the separate diagnostic
object and links under ignored `build/`. No `/force` link or shared patch edit.

`render_seam_probe.c` samples the first call of each scope at virtual fields
6500 and 6501 by default. It records thread IDs, stage IDs, before/after scene
node/resource/link pointers and all node words, plus accessible EE RAM word
deltas. It does not insert a fence. Deltas are inclusive observations that
may include concurrent worker writes, not exclusive callee write attribution.
NOACCESS/GUARD pages are explicitly skipped rather than forcing deferred
depth reads; skipped pages are absent evidence, never a relaxed comparator.
No scratchpad/MMIO/VU-memory completeness is claimed. Timing is deliberately
distorted and cannot be used as performance evidence.

The input TU hash and complete linked-object hashes are retained in the local
diagnostic manifest. Baseline gameplay/reference gates remain unchanged.

### Completed two-field observation

`build/port/render-seam/diagnostic3/dhd.exe` (SHA-256
`86ab9102fd44e7f0f8284937eb6523abb83c741a5f1417d41531e9dd5e770b2c`)
uses the accepted hardware baseline's unchanged objects except for the
instrumented TU and additional diagnostic object. The run completed both
fields and was intentionally stopped. No comparator is invoked or relaxed
by this observer. Evidence is `build/port/render-seam/run1/`.

| Scope | Field 6500 changed words/pages | Field 6501 changed words/pages |
| --- | ---: | ---: |
| 00215B68 prepare | 1961 / 75 | 1775 / 75 |
| 002161B8 finish | 260 / 18 | 260 / 18 |
| 002167C0 first scene list | 1516 / 33 | 1353 / 33 |
| 00216980 second scene list | 233 / 13 | 233 / 13 |

All boundaries execute on the same recorded EE thread; no inaccessible
pages were present at these boundaries. Worker quiescence remains unproven.
The lists contain 397 and 18 objects. All 415 object pointers and resource
identities persist between the two fields. No captured node word changes
across an observed scope in this particular pair (visibility bits can already
be clear); roughly fifty nodes change position and roughly thirty change
matrix words between fields. Thus this is useful bounded correspondence
evidence, not a claim that scene walks never mutate nodes or that pointer
identity supplies production lifetime generations.

Observed writes include packet storage `D_00528900`, `D_00580180`,
`D_0061D980` and `D_0061E190`; per-view state within `D_00528360`; resource
or heap pages outside the captured node records; and global
`D_0029E1E8`, `D_0029E1F8`, `D_0029E218`. The latter is a texture residency
age counter: `00219EE8`/`0021B668` increment it and `00218C28` uses it for
LRU selection. The corresponding residency tables `D_00621310`,
`D_00625190`, `D_0062E850` and `D_0062E890` are render state too; changing
their history can affect subsequent canonical texture packet output.

### Implemented snapshot foundation

`port/render_snapshot.h/.c` define the understood native 0x100-byte node
layout without embedding host pointers. Three owned buffers give immutable
committed previous/current frames plus a staging transaction. A failed or
duplicate/over-capacity capture cannot publish a partial frame. Explicit
stable identity and generation are separate from source address. Resource
changes, spawn/despawn, explicit cuts/resets and tick gaps invalidate history.
Exact endpoint copies preserve all node bytes, including currently unresolved
fields. Visibility consumption acts on a caller-owned mutable copy.

`port/tools/test_render_snapshot.py` passes native x86 and x64 builds with
warnings treated as errors. It verifies endpoint byte identity, independence
from live node mutations, render-owned visibility, moved pointer storage,
generation reuse, resource changes, spawn/despawn, transaction rejection and
history resets. This module is not yet called by the game and does not itself
render or interpolate anything. It deliberately does not invent matrixC0,
camera or skeletal semantics. Cross-thread publication must be explicit when
a renderer is connected.

### Concrete context extraction inventory

Before endpoint replay, extract these independently owned fields rather than
restoring global EE memory:

- Scene-node/list copies and per-view visibility; immutable pointed resources
  require their own lifetime and writable-cache audit.
- `D_0029E1A0/A4/A8/AC` matrix arenas, `D_0029E1B0/B4` stacks and scratchpad
  offsets 0x38, 0x68/6C, 0x1D0, 0x2AC/B4/B8, 0x300/290 and 0x400 onward.
- Packet contexts `D_0029E1DC/E0`, saved `D_00528900`, `D_00528980`,
  `D_00580180`, `D_005D7980`, `D_0061D980`, `D_0061F9C0`, continuation
  buffers and DMA state. In-flight packets need lifetime ownership, not only
  replacement cursor values.
- Per-view `D_00528360 + view*0xF0`, `D_006201D0 + view*32`,
  `D_00620290`, lighting/contribution pointer/count `D_0029E1F8/FC`,
  scene callback and `D_0029DAA4` modulation state.
- Texture residency age/tables above and mesh/resource cache writes. Texture
  backing content and canonical GS feedback must remain coherent.

Known transform evidence: `0023CF58/78` return the selected object matrix
at +0x40 and position at +0x10 via `D_0075A9D0[view]`; `0023CFA0/CFE0`
set those same fields. `00216AE0` copies that selected matrix/position into
flagged nodes and combines the old node position. `00216E28` combines the
node matrix with per-view state matrices at +0x70 and +0xB0 before VIF
emission. These are verified offsets/operations, not yet proof of camera
inverse conventions or a world-space interpretation for every node.
Both node matrices +0x40 and +0xC0 initialize to identity in `002182D0`;
the latter's complete semantic role remains unresolved. Instrument the
initializer/resource setter and actual object removal/allocation paths to
obtain lifetime generations; do not increment a generation every frame.

### Explicit context component and private VIF adapter

`port/render_context.h/.c` implement a scoped, explicitly enumerated render
state component: fourteen pointer/scalar globals, 1,000 texture entries,
16x300 residency slots, sixteen texture bins and bank counters, 1,000 texture
use counters and the 16 KB hardware scratchpad. Source sizes come from
`00218A30`, `00218C28` and `002193E8`. This is not an arbitrary EE-memory
save/restore facility. Packet arenas, pointed mesh caches, VIF and GS state
must be isolated by a separately supplied backend ownership token; absent
thread/fence/pin/unpin capabilities or a null token, no bind happens.

`port/tools/test_render_context.py` passes strict x86/x64 native builds using
mock capabilities. It verifies restoration on success and pass refusal,
reentry rejection and refusal before mutation for missing capabilities,
wrong thread or failed entry fence. A failed exit fence returns a fatal
diagnostic status and deliberately retains bound state; a real adapter must
stop its owned process, never resume gameplay. These mocks establish the
component contract, not actual graphics isolation.

The in-progress `port/hle/vif.c` private adapter uses private VIF registers,
`ps2_cpu`, code and data allocations. Private code avoids the canonical AOT
resolution cache, which keys mapped code by the global monotonic version.
The extra execution is serialized after `graphics_wait()` because native
VU counters and trace state remain process globals. It does not rebind
canonical VU1 publication, mapped VIF/MMIO registers, provenance or code
version. The private call scopes MXCSR exactly like the graphics worker and
restores the EE caller's control word. EE OS-thread identity is learned from
the existing synchronous canonical VIF0 path; conflicting callers invalidate
private use. Independent GS sink lifetime/refusal validation is required.
`port/tools/test_vif_private.py` now passes with the production VIF parser and
VU interpreter and an explicitly mocked sink registry. A synthetic VU program
checks TOP/ITOP, double buffering and MSCNT; partial UNPACK and DIRECT are
also exercised. Private-only execution leaves canonical VU memory and code
version unchanged; feeding both instances identically then compares their
complete register/code/data endpoints. The test verifies MXCSR restoration,
thread and stale-sink refusal and a failed context's refusal of further input.
Real sink ownership, queued lifetime and gameplay image equality remain
separate independent acceptance steps. Diagnostic native VU counters may
increase during serialized replay; they are not guest state.

The existing canonical `test_vif.c` standalone parser/upload/MSCNT/DIRECT
regression also passes with zero failures. The private adapter requires the
explicit `DHD_PRIVATE_VIF=1` build option; default standalone targets do not
gain an unfulfilled sink dependency.

Backend review established that GPU storage is already per `Gs`, while
deferred readbacks and their field accounting were process-global. The GS
owner is implementing per-instance deferred state and explicit private sinks.
The first extra pass must refuse readbacks and SIGNAL/FINISH side effects.
No global `Gs` swap or concurrently running extra VU invocation is permitted.

### Prepared real transport endpoint driver

`render_seam_endpoint.py` emits an ignored build-only tap around one complete
`dma.c::transfer_out` invocation, before `kernel_dmac_interrupt`. The selected
VIF1 transfer is synchronous on the EE thread and contains no `kernel_poll`
or EE callback. Thus the interval is bounded so that all GS/VIF input comes
through its recorded feed calls. It is not a whole-frame tap that silently
misses the direct `vif_feed` calls in `sceGsExecStoreImage`.

`render_seam_endpoint_hook.c` fences and clones the initial GS/VIF state,
compares every initial raw VRAM word, copies all transfer input, then replays
it into private VIF/GS instances after canonical completion. It requires at
least 4,096 input words, at least 256 actual raster primitive submissions
and changed VRAM before a nontrivial transport result can pass. It compares
all final raw VRAM words, architectural GS fields and complete VIF/VU state,
and guards canonical VRAM/VIF against mutation by replay. Unsupported private
commands, incomplete input or insufficient scene work refuse the job. The
first result will prove at most a bounded real geometry transport endpoint,
never a full world view, interpolated pose or high-refresh delivery.

`render_seam_endpoint_build.py` freezes accepted link inputs and the relevant
host sources/headers, rebuilds the per-instance GS/HLE units together with
private VIF/raw GPU test hooks and links without `/force`. The driver
compiled successfully as `build/port/render-seam/endpoint-build1/dhd.exe`,
with all affected GS/HLE users rebuilt from frozen sources and headers. The
extended private VIF accessor test also passes: full data-memory copy and
explicit unpack state retain untouched bytes and TOP/DBF state. No transport
game run has occurred; independent sink/per-instance tests and the prioritized
renderer measurements must complete first. Once the endpoint
is verified, extend capture to a complete view and substitute proven immutable
scene/mesh transforms rather than treating packet replay as new visual state.

## Proposed bounded extraction after the diagnostic

Create an opt-in render snapshot layer keyed by scene lifetime/resource plus
per-view identity. Copy immutable previous/current camera transforms, scene
world transforms, skeletal palette inputs and draw/resource identities at the
simulation boundary. Inventory must establish which matrix is world versus
derived view/projection before interpolation. Interpolate translation and
rotation with a proper rigid-transform representation; do not linearly blend
all matrix elements, flags, pointers or resource identifiers. Reset history
on cuts, teleports, lifetime changes and topology/resource changes.

The first extraction should render one world view and its rider from those
snapshots into render-owned packet/matrix/visibility/cache storage, with an
independent output surface. Culling and geometry are freshly evaluated for
the interpolated camera/pose. Reusing old projected triangles without stable
scene/mesh identity is not sufficient. Keep canonical simulation rendering
and depth-feedback semantics initially; suppress all extra-pass audio,
timer, RNG, input and feedback producers. Do not claim complete high-refresh
coverage until riders, world, animation, camera, particles and overlays are
accounted for.

Use previous/current snapshots and a one-tick presentation delay for bounded
interpolation rather than predicting physics. This introduces presentation
latency; measure input-to-display separately. Exact simulation equality is
still required at fixed ticks. A later low-latency camera policy would need
its own explicit behavior and validation.

Acceptance: unchanged EE snapshots at all existing moments; byte-identical
canonical GS images and deferred Z records; no second simulation producer
invocations; distinct midpoint camera/rider transforms and fresh draw IDs;
ordered spawn/cut/teleport behavior; independent render deadlines and audio
sample counts; then actual present completion/frame-time evidence throughout
late gameplay. Endpoints of the render-only path must first reproduce the
canonical images before midpoint visual review. Preserve retail image MATCH.

## Native geometry direction

### Independent decoded-channel footprint proof

The bounded selector-3 mesh decoder described in finding 037 now has an
independent production-VIF check in `port/tools/test_mesh_vif_plan.py`.
The original type-0 inline stream is fed through private VIF up to the
MSCNT boundary. A separate writer expands the decoder's position table,
unsigned position-index stream, RGBA8, signed UV16 and symbolic ROW/COL-W
patch into an independently seeded full data-memory image. No VU program or
GS callback executes in this check.

All 33 cases pass exact comparison of all 16 KB, including untouched words,
and every exported unpack-state field (command/need/have/address/written,
CL/WL/mode/MASK, ROW/COL, TOP/TOPS/ITOP/TPC, short data and bit buffers).
The canonical mapped VU data is unchanged by the private path. The first
case uses local B0078 capture data with the source-proven ROW/COL values;
32 more poison the full data image and inherited ROW/COL. Local artifacts
`build/port/mesh-vif-plan/` include actual/expected data and state, input/source
hashes and provenance for the geometry owner's subsequent complete VU/GIF
check. These are channel-expansion results, not topology, native renderer,
game image, performance or new visual-frame proof.

The opt-in private VIF adapter now also provides `vif_private_upload_mesh`:
it expands the owned decoded channels directly into private VU memory and
updates the exact unpack endpoint, without consuming a second VIF stream.
Entry mode/CL/WL/MASK, program/mask metadata, counts, index bounds and the
two-record COL-W patch are all checked before writing. Input count is
conservatively limited to 151 to avoid wrapping the bounded data region;
unsupported forms or state refuse without mutation. It neither infers
topology nor supplies material/transform state.

The independent test now compares three paths across all 33 cases: original
production VIF, the separate manual expansion and this cached uploader. All
16 KB and the entire unpack endpoint match; invalid counts/masks and repeat
upload under the wrong entry state are refused before mutation. Artifacts
are `build/port/mesh-vif-plan-cached1/`. Finding 037 separately records the
geometry owner's positive complete VU/GIF endpoint proof using equivalent
data (four triangle draws and two ADC skips). No canonical game path uses
the cached uploader yet, and no speedup is measured or claimed.

The accepted runtime already has six guarded SIMD geometry kernels in
`vu0_interp.c`, `vu1_native_geometry.h` and `vu1_native_extra.h`, selected by
entry/VI13 and exact local microcode comparisons. The old rejected B0078
prototype is not a new opportunity. Current VIF worker time must be profiled
by native key, clipping and residual AOT dispatch before choosing another
kernel. Removing 13 ms from an overlapping worker is not automatically
13 ms of frame gain.

The high-payoff architectural bypass is at the object/mesh submission seam
before `0021B8A8`, not after `GsVertex` decoding: retain stable scene/resource,
world/view transforms and mesh identity, decode immutable mesh streams once,
then generate geometry per interpolated view without building/interpreting a
second entire PS2 VIF stream. This needs material/pass/skinning/clipping and
feedback equivalence; it is a staged renderer project, not an already-proven
bounded replacement. First bounded optimization should target the measured
dominant remaining native clipping/AOT span and retain exact state/GIF gates.

## Research

Primary engine documentation consulted; no implementation copied:
[Godot fixed-tick interpolation introduction](https://docs.godotengine.org/en/stable/tutorials/physics/interpolation/physics_interpolation_introduction.html)
and [using interpolation / teleport resets](https://docs.godotengine.org/en/4.4/tutorials/physics/interpolation/using_physics_interpolation.html).
These support separate previous/current transform history and reset semantics;
they do not establish Downhill Domination's render purity or performance.


## Private GS foundation: independent ownership verified

The endpoint prerequisite now gives each `Gs` its own deferred readback list,
issued snapshots, readback field counter and optional trace. Existing canonical
entry points retain their behavior; canonical initialization explicitly selects
its trace, while private instances default to no trace. Releasing an instance
settles only its own callbacks before destroying its backend. The appended
`Gs` fields require consistent recompilation of all clients; the frozen renderer
optimization candidates retain their original, separate ABI basis.

`GraphicsSink` uses a synchronized registry of non-reused opaque keys, actual
owner-thread checks and queued lifetime pins. Each queued packet carries its
explicit target; the canonical global GS is never swapped. Private submission
preflights complete GIF control state and refuses SIGNAL, FINISH and local-to-host
readback before publishing that feed. Destroy drains queued references before the
caller may release the target. This is a transport prerequisite, not proof that
replaying arbitrary game rendering is free of simulation side effects.

Architecture cloning explicitly copies architectural GS fields and coherent
VRAM into a fresh backend, never source callback/backend/readback pointers.
The caller must fence canonical GS, presentation and immediate-context users;
shared D3D device/context access remains serialized. Ordinary source GPU sync
may update CPU cache bookkeeping but does not settle issued deferred requests.
Private GPU initialization suppresses process-level diagnostic trace creation,
so cloning cannot truncate the canonical diagnostic file.

Independent real-sink CPU contracts passed: two targets, ring wrap, copied
payloads, provenance/field separation, unchanged canonical bytes, wrong-thread
and stale handles, forbidden-feed preflight and destruction with pinned queued
work. Real per-instance deferred-readback CPU contracts also passed (ignored
artifact `build/port/r28/real-sink-readback-contracts3`). The independent GPU
foundation run passed the 77 baseline raw-VRAM fixtures and 4,194,304 additional
raw word comparisons: initial clone/source/CPU equality across all 4 MB,
source Z=200 and private Z=300 each matching their CPU oracle, pending canonical
destination canaries unchanged until canonical settlement, private destruction
settling only its own issued bytes, and preservation of a canonical diagnostic
file canary. These tests establish the bounded ownership/clone contracts; they
do not establish an extra gameplay frame or any 120 Hz delivery claim.

Synchronization research used Microsoft's primary documentation for
[SRW locks](https://learn.microsoft.com/en-us/windows/win32/sync/slim-reader-writer--srw--locks)
and [InterlockedDecrement](https://learn.microsoft.com/en-us/windows/win32/api/winnt/nf-winnt-interlockeddecrement).
Registry locks are not held across queue waits or callbacks. No external
implementation code was copied.


## Independent GS isolation contracts

`port/gs/test_hw_sink.c` and `port/tools/test_hw_sink.py` exercise the actual
GS parser and private sink queue, without a GPU. The passing test covers two
sink identities; copied packet ownership across ring wrap; private provenance
and field independence; wrong-thread and stale-handle refusal; and whole-feed
SIGNAL/FINISH/readback rejection before any partial write. A blocked real GS
draw callback proves that destruction waits for queued target work to retire.
All canonical GS bytes remain unchanged during private operations.

The same test uses separate staging callbacks with known issue bytes to check
interleaved per-GS deferred requests, release/reset of one instance while the
canonical request remains pending, independent trace fields, stale-epoch refusal
and explicit settle/retry. A fresh architectural clone matches every value byte
while excluding backend, readback and callback ownership. Local passing result:
`build/port/r28/real-sink-readback-contracts3/`. Staging callbacks in this CPU
contract test are deliberate mocks; they do not establish GPU staging behavior.

`port/tools/test_hw_renderer.py --foundation` separately checks real D3D11
resource ownership. Its 77 existing full-raw-VRAM fixtures pass, followed by
4,194,304 additional raw-word comparisons: a clone initially matches every
source/CPU byte, then independently written source Z=200 and clone Z=300 each
match their full CPU result. Source issue-point Z=100 remains pending through
the clone/fence and later writes; settling it leaves the clone request pending.
Clone destruction settles its own staged Z=300 before releasing resources.
A process diagnostic-file canary also proves that private clone initialization
does not truncate the canonical trace destination. Result:
`build/port/r28/foundation-gpu-clone2/execution-result.json`.

These checks support ownership and transport prerequisites only. They do not
prove a second rendered game state, correct-speed 120 fps gameplay, arbitrary
private command support or end-to-end cached geometry rendering. The endpoint
transport diagnostic must still pass independently before any broader claim.

### Complete-transfer transport driver readiness

`render_seam_endpoint_build.py` rebuilt all affected GS-layout users and
linked an ignored executable without `/force` in 9.15 seconds. That second
freeze is superseded before use: source review found the TTE tag-upper-half
call in `transfer_out` bypassed its `feed` helper. The injector now explicitly
routes that exact call through the copying tap and refuses an unexpected
direct graphics feed. Thus DMA tag VIF codes and payload retain their full
original ordering in the recorded stream. A new freeze is required before
execution. No transport game run has passed yet.

`render_seam_endpoint_run.py` prepares deterministic navigation and owns one
bounded child process, stopping only that process once a diagnostic status
arrives or its deadline expires. Its manifest explicitly records zero new
visual frames. The normal replay must pass before enabling the cached mesh
arm. Neither replay arm implements scene interpolation or a 120 Hz scheduler.
The independent real GS clone/sink tests recorded below establish private
backend ownership; they do not establish this complete-transfer endpoint.

### Integrated in-game endpoint smoke (2026-10-15)

The opt-in scene variant now rebuilds the actual observer-bearing VU runtime,
replays the complete selected DMA into an independent GS, and prepares private
presentation through `gs_present_private`. The two canonical world-node
endpoints are captured at fields 6499/6500; the existing VU0 programs reproduce
their exact transform uniforms on owned seeds. Translation and camera-offset
xyz can then be interpolated while retaining current orientation/projection.
This is a partial geometry prototype over a cloned background/depth image;
it is not a complete interpolated frame or a 120 Hz gameplay scheduler.

`endpoint-run3` proved the nontrivial transport in the game: 8 chunks / 2374
words, 356 submitted GPU primitives and 42 changed raw VRAM words. The private
full VIF state, GS architecture and all raw VRAM matched the canonical endpoint.
All 8192 readable EE pages were unchanged, page protections were unchanged,
and exposed pending-read/backend/callback ownership canaries were unchanged.
No unreadable EE page occurred in this run; opaque staged readback contents
were not individually compared. The matched node was `08796280`, with terminal
resource alias `013DF860`; both field transform endpoints matched the canonical
180/238/808 sequence. The midpoint differed, but its uniform upload was in an
earlier transfer, so this run refused substitution and presented no new pose.

The follow-up uses an owned pre-transfer private VIF fork, allowing an exact
64-byte qwords-0..3 endpoint match before replacement when the matrix upload
was earlier. The focused CPU smoke verifies private fork pointer rebinding,
untouched remaining data memory, unchanged source/canonical context, and
refusal before mutation on a mismatched expected uniform. It passed in 1.86 s.
Live visible interpolation remains under investigation; no 120 FPS claim.

### Direct integration continuation

The camera prototype now associates the actual frame view-projection upload
(`6C110009`, VU1 qwords 9–12) with adjacent field snapshots. Build9/run7
presented a private midpoint with two changed display pixels in a small
56-primitive transfer. This is evidence of the path executing, not a complete
interpolated frame or high-refresh performance.

The initial implementation incorrectly rendered the midpoint over the normal
endpoint's color/depth contents. The endpoint hook now creates separate private
GS/sink/VIF states from the same pre-transfer state for both renders. Build10/
run8 at field 3800, transfer 20, passed its unchanged canonical-state guards and
normal replay comparison, and presented 116 changed pixels (160 changed raw
VRAM words) from an 80-primitive transfer. These are partial-scene results.

The driver now supports `--transfers` to capture consecutive DMA submissions
from one field, reducing repeated clone/setup work and expanding geometry
coverage. Build11/run9 at field 6500 captured 16 transfers (80 chunks,
19,808 words), replayed 1,410 primitives and matched the normal endpoint's full
VRAM, GS and VIF state. Its private camera midpoint changed 196,350 display
pixels and 198,196 raw VRAM words while the canonical EE/VRAM/VIF guards passed.
Both 512x448 images were visually inspected: the gameplay scene remains intact
with a small camera shift. This is a larger partial-scene prototype, still not
a continuous full-frame interpolator or a measured FPS improvement.
`--reuse-vu-from` reuses the prior scene runtime object only when its frozen
runtime dependencies and generated AOT include match and its manifest hash
verifies, avoiding recompiling the large unchanged VU runtime each iteration.

Build12/run10 extends capture to the field boundary before VBLANK (`--transfers
0`). It recorded 326 VIF transfers but correctly refused replay because direct
GIF DMA packets were not yet represented. The next candidate records both VIF1
and GIF DMA, retains their ordering and replays GIF on private PATH3. It does
not claim complete capture of SDK readback calls or full-frame correctness yet.

An opt-in `DHD_GS_GPU_CLONE_COPY=1` snapshot path flushes/uploads the source and
copies its GPU VRAM directly, preserving CPU stale-page metadata for later CPU
reads. It avoids the previous forced GPU readback followed by upload. The
current-source GPU test passes all 77 fixtures (80,740,352 raw words) and the
additional 4,194,304-word clone/ownership checks with this switch enabled.
Artifact: `build/port/render-seam/fast-clone-check/gpu-copy.log`. This validates
the copy path's behavior; no sustained gameplay speed gain is measured yet.

Build14/run11 now passes the field-boundary capture at gameplay field 6500:
326 transfers, 2,103 chunks, 379,368 words and 23,255 submitted primitives.
Normal private replay matches full raw VRAM, VIF and architectural GS state;
canonical EE pages, VIF and VRAM remain unchanged by private execution. The
midpoint matches the seeded camera and one uploaded camera packet, plus four
object-uniform uploads. It changes 209,798 display pixels; normal/midpoint
images were visually inspected including rider, terrain, vegetation and HUD.
Capture starts at the first eligible observed scene transfer, so it still does
not prove every object/animation is interpolated or every course is supported.
It is a one-shot field-rendering prototype, not continuous 120 Hz gameplay.
Next: persistent rolling endpoints, repeated presentation and frame-time
measurement without the one-shot diagnostic's full CPU readbacks/comparisons.

The first bounded continuous mode (`--live-frames`) rolls endpoint ownership
between fields and emits private midpoint presentations without the normal
replay, CPU image readbacks or whole-EE comparisons. This mode remains an
experimental capacity measurement; its two presentations are not yet paced as
120 Hz gameplay. Build15/run12 was stopped after eight fields: repeated full
scene-list scans in the observer caused roughly 7–8 seconds per field.

Build16 indexes node resource identities once per field, then checks only the
matching bucket against live node bytes. Run13 completed ten consecutive
midpoint presentations in 2.500213 seconds. Private replay submission costs
roughly 30–34 ms, cloning around 0.8–1.2 ms; the remaining field time still
requires profiling. This is a substantial diagnostic-overhead reduction, not
a gameplay FPS gain. Build17 adds observer-time accounting for the next run.

Run14 attributes about 190 ms/field to observer work. Invocation-local readable
region reuse reduces that to about 39 ms (build18/run15, ten fields in 0.939928 s).
The next iteration tracks the EE-owned readback-protection generation and
invalidates cached regions on each protection change, instead of querying
unchanged mappings in every observer invocation. It also separates retirement
of the canonical VIF producer from draining the GS queue: private packets remain
ordered/pinned but no longer wait for every preceding private GS packet.

Build19/run16 completes 30 consecutive midpoint presentations in 1.310641 s.
Observer cost settles near 0.2 ms/field, private submission near 12–15 ms,
and clone setup near 0.7–1.1 ms. This is still experimental unpaced throughput,
not 120 Hz delivery or a replacement for the accepted game executable. The
existing CPU scene-transform and real GS sink tests pass after these changes.

Build20 adds reuse of private GPU resources across rolling fields. The targeted
refresh check passes 6,291,456 raw-word comparisons, retains the backend identity,
copies the source image, renders independently afterwards and preserves pending
canonical readback bytes. Build20/run17 completes 60 interpolated presentations
in 2.053293 seconds. The first 30 fields take 1.041482 seconds versus run16's
1.310641 seconds (about 20.5% less elapsed time in these individual trials).
Median clone/refresh setup is 0.223 ms, private replay submission 13.600 ms and
presentation call 0.104 ms. The resulting 29.22 simulation fields/second with
an additional presentation still does not satisfy real-time simulation or
120 FPS. Repeated geometry/raster work now dominates the remaining problem.

Build21 adds a thread-local translated-block lookup cache for explicitly
versioned private VU code. Each capture gets a unique version; private MPG
writes invalidate it, and forks retain a version only for identical copied
code. Unversioned oracle buffers keep their original per-call comparisons.
Run18 passes the 326-transfer full-field replay with exact raw VRAM/VIF/GS and
canonical guards. `DHD_PRIVATE_AOT_CACHE=0` retains the uncached path.

An opt-in coarse-depth bound can reject a provably hidden primitive pixel before
double depth interpolation. Remaining pixels still use the original precise
calculation. Its 77-fixture raw-VRAM test passes. Build22/run19 batches this,
private AOT caching and earlier optional GPU switches: 60 presentations take
2.165856 seconds, slightly slower than run17's 2.053293. This combination is
not promoted. Coarse depth remains off by default and is compiled out when off;
the frozen experiment and logs remain available. Next larger change: capture
decoded VIF memory updates and execution events so private rendering can reuse
the canonical decoding work instead of parsing the stream a second time.

Build23/run20 adds an owned decoded field stream: actual masked VIF memory
writes, MPG changes, VU entry/continuation events, and direct GIF input. Private
VU execution still regenerates XGKICK outputs. Replaying 5,311 events at the
current endpoint passes full raw VRAM, VU CPU/code/data and GS architecture
comparisons; the independent original raw-stream comparison and canonical
ownership/state guards also pass. This proves the selected complete field,
not all fields or interpolated visual correctness. Live throughput is being
measured separately. DHD_RENDER_DECODED=2 selects reference comparison and =1
selects experimental midpoint replay; the accepted executable is unchanged.


Live decoded run21 completed 60 midpoint presentations in 2.260337 seconds.
Run22's optional event timings show roughly 10�13 ms in 1,550�1,625 private
VU runs per field, versus about 0.3 ms data writes and 0.2 ms pose patching.
The user reports concurrent Brave YouTube/Twitch playback during current
experiments, unlike earlier runs; cross-run wall-time differences are therefore
confounded and cannot establish a regression or improvement by themselves.

Build26 removes temporary allocation/copy for contiguous private XGKICK spans
and signals GS queue events only when a waiter is registered. Each wait uses
register-then-recheck to avoid lost wakeups; shutdown still signals explicitly.
The actual threaded GS sink contract test (including queue wrap and destruction)
passes with zero failures. Live profile run23 is pending; no FPS gain claimed.

User direction update: batch optimizations and measure FPS once; defer visual
judgment to user play and fix reported problems, avoiding per-change gates.
Build30 combines native FMAC result/operand specialization and both queue
waiter-aware wakeups. The batch benchmark enables zero-area/sprite hardware,
texture specialization, bin elision, tight scissor and coarse depth together.
Its hook records 600 normal fields and display-buffer changes without private
replay or scene observation. Manual launchers are under build/port/fast-preview;
manual play retains normal simulation pacing, and a previous-build launcher
preserves round28. Batch timing is pending; 120 FPS is not achieved.

Batch FPS run1 completed: 675 fields/display-buffer changes in 12.457948 wall
seconds, 54.182 uncapped display FPS. This is the combined build under the
user's current browser/video workload, not a controlled comparison against
round28 and not correct-speed 120 FPS. The first benchmark implementation
rewrote its result until the runner stopped it; all reported counts and elapsed
time refer to the same interval. The hook now latches the first completed
interval for future builds. No further correctness gate was run, per the user's
request. Manual build30 launchers remain available for user visual evaluation.

Build31 batches a per-pixel FP32 depth upper envelope (coarse-depth mode2),
colour interpolation after early depth rejection, and acquire-only queue cursor
loads in place of locked compare/exchanges. Precise double depth remains the
final calculation; the approximate envelope only rejects pixels bounded behind
the stored depth. The host accounts for coefficient conversion and FP32 operation
rounding over the primitive bounds. Unsupported/uncertain bounds retain the
original path. New shader structure carries four extra plane/error words.
No per-change regression run was added, per user direction. Batch FPS run2 is
pending; shader compilation/runtime results and user visual review are outstanding.

Batch FPS run2 completed 600 fields and 600 display-buffer changes in
10.879240 seconds: 55.150 FPS with the current background workload. Shader
compilation and gameplay execution completed, but this single observation is
not a controlled improvement claim. The manual preview launcher now selects
build31/coarse-depth mode2; round28 rollback remains unchanged. These local
queue/shader changes have not closed the substantial 120 FPS gap. Further
work needs to remove duplicated rendering/geometry cost and establish an
independent presentation pipeline, not merely keep stacking small switches.

Build32 changes primitive and tile SRVs from structured to typed buffers and
adds DHD_GS_STREAM_UPLOADS. The small primitive/state/scanout/copy buffers use
DYNAMIC + WRITE_DISCARD mapping; large tile lists retain default resources.
Primitive field layout stays explicit (14 uint4 records). Discard renaming
lets queued draws retain prior upload contents while the CPU submits later
batches. This changes upload ownership, not scene math or draw order.
Research: Microsoft's D3D11_RESOURCE_MISC_FLAG documentation disallows combining
resource misc flags with CPU access flags, so the dynamic primitive buffer uses
a typed SRV rather than a structured misc flag. No external code was copied:
https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_resource_misc_flag
Batch FPS run3 is pending; no additional per-change gate was run.

Batch FPS run3 completed 600 display changes in 10.877722 seconds (55.158 FPS),
essentially the previous observed throughput. Streaming uploads remain opt-in;
manual preview remains build31. This experiment does not establish a gain.

Next structural work started: a detached geometry executor copies decoded
inputs, VU execution storage and precomputed uniform patches into an owned job;
its worker generates owned GIF output and the owning thread later submits it.
Native VU counters/configuration are thread-local, and private execution bypasses
the canonical provenance callback. No private sink ownership is transferred.
This job API is not yet wired into overlapping field scheduling or presentation,
and has no FPS claim. Build33 compilation is pending; no additional test run.

Build34 integrates detached geometry jobs with a one-field presentation delay.
Field N's owned worker executes while canonical simulation/rendering advances
to N+1. At the next boundary the owner submits the finished private GIF stream,
presents midpoint N then held canonical N, and starts the next owned job. Two
private GS instances alternate; the canonical displayed image is copied on the
GPU before its source changes. Uniform replacements are precomputed on the EE
owner and copied into the job. Normal runs without an endpoint log no longer
activate the experimental observer. Decoded live capture avoids redundant raw
packet allocations. The asynchronous batch run is pending.

Presentation is still paired on the EE owner, not yet independently paced120Hz.
Microsoft's immediate-context/DXGI threading guidance is the reason not to move
Present onto an uncoordinated second thread while GS uses the same context:
https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-render-multi-thread-intro
No external implementation was copied. The current batch deliberately establishes
geometry overlap and chronological image ownership first; it makes no120FPS claim.

Async run1 completed 60 midpoint/canonical pairs in 1.864879 seconds, with
all 60 pairs reporting successful presentation. This is about32 completed
simulation fields/second with two burst presentations, not correct-speed120Hz.
Join/render/presentation remains roughly10�19ms in most logged fields (one
29.8ms spike); moving VU work off the EE thread alone did not remove GS work
from the boundary. Manual preview remains build31.

Next structural target: allow the owned geometry producer to publish private
GS packets while canonical VIF work runs, using serialized multi-producer queue
publication and an EE-controlled pause at every graphics fence. The pause must
prevent any later private GS command from racing EE immediate-context access;
merely waiting on the current queue cursor is insufficient with a live producer.
After overlap capacity is adequate, presentation pacing must still be separated.

Build35 batches delegated private GS publication with explicit fence ownership.
The EE owner registers the actual suspended geometry thread and retains a sink
reference until that thread terminates. Private publications are serialized with
canonical publications. A full graphics_wait closes the private publication gate
under the publication lock before draining both queues, preventing future private
packets from racing EE immediate-context work. Canonical graphics_feed resumes
private publication; joining a pending owned job explicitly resumes it first.
The GS consumer remains the sole background D3D command producer. Private input
validation remains active, and ordinary wrong-thread sink access remains refused.
Async run2 is pending; no separate per-edit test run was added.

Async run2 completed60 pairs in1.902310s, essentially unchanged from run1;
no performance gain is claimed for delegated publication. It remains opt-in.

Build36 adds an opt-in native integer texture cache for T8/T4 textures with
ordinary repeat/clamp modes and dimensions up to1024. A GPU compute pass decodes
requested mip levels into a reusable R32_UINT texture. Raster sampling then
loads decoded RGBA values while retaining the existing integer filtering math.
Cache reuse compares exact texture/mip/alpha registers, all256 palette entries,
and immutable VRAM snapshot revision. Region clamp/repeat and small distant
surfaces retain direct sampling. Private instances own separate mutable caches
and may share only the immutable decode shader. The combined FPS run4 is pending;
no per-edit correctness gate was added, per user direction.

Build36/run4 completed 600 fields/display changes in 12.162689s (49.331 FPS),
below the earlier roughly55 FPS observations. Browser/video load is uncontrolled.
The one-entry whole-snapshot cache is not promoted. Build37 batches eight LRU
entries with source-page invalidation: at snapshot replacement, only entries
whose complete source mip pages intersect accumulated VRAM writes are invalidated.
Exact register/palette keys and integer filtering are retained. Clone refresh
marks all pages dirty, preventing stale private-cache reuse. Resource allocation
is lazy and the decode constant buffer is shared within one backend. One combined
600-field FPS test follows the build; user will review visuals separately.

Build37/run5 completed600 fields/display changes in12.071237s (49.704FPS).
The batched cache remains below earlier roughly55FPS observations and is not
promoted; manual preview stays build31. No120FPS claim. Background workload
remains uncontrolled; source-page/LRU caching alone did not deliver the target.

Build38 batches raster demand elimination: exclusive full32-bit stores skip
the old VRAM load; zero write masks return; destination colour is loaded only
when DATE or a selected blend operand consumes it; double-depth evaluation is
skipped when ZTE disables both Z testing and writing. Existing precision and
filtering are unchanged. Decoded-cache option stays off. One combined600-field
run also records GPU query timings (batch6-gpu.csv) to locate remaining cost.
Query overhead makes its throughput non-identical to earlier unprofiled runs.

Build38/run6 completed600 display changes in11.565989s (51.876FPS), with GPU
queries enabled and browser workload uncontrolled; no isolated speedup claim.
Last600 GPU rows average19.951ms render interval (includes starvation), .012ms
scanout and .007ms copy. Nineteen sampled frames attribute about15.25ms to
hardware raster, mainly texture classes1 (8.136ms) and2 (4.982ms); compute
fallback is only .035ms per sampled frame. Rendering remains the major cost;
next work should target textured hardware shading, not scanout/copy overhead.
Manual preview remains build31; correct-speed120FPS remains unachieved.

Build39/run7 batched zero-weight bilinear-tap pruning, zero-fraction/repeated
mip pruning and MXL-zero LOD bypass. It completed600 display changes in
11.975642s (50.101FPS), with profiling disabled. This did not establish a gain
over prior observations, so its sampling changes were removed from the working
shader, restoring build38 shader bytes. Experiment retained only under ignored
build39 artifacts. No new visual validation; manual preview stays build31.
Further branch-level pruning has not produced the needed improvement. The next
substantive route is separating texture shading from ordered VRAM operations
or native render-target/depth handling, rather than adding per-tap branches.

Build40 experiments with DHD_GS_SHADE_BEFORE_ROV=1 on hardware PS only.
Texture reads use the immutable snapshot, so shading can precede the first
ordered VRAM read. Disables coarse/early depth rejection in that variant,
retaining original late depth test, exact depth math, filtering and ROV writes.
Generic and format-specialized hardware variants both receive the switch;
compute remains unchanged. This trades extra hidden-fragment shading for a
potentially shorter ordered region. Driver scheduling benefit is a hypothesis.
Reference: https://learn.microsoft.com/en-us/windows/win32/direct3d11/rasterizer-order-views
(ROV order guarantees; no external code copied). One combined run8 is pending.

Build40/run8 completed600 display changes in10.122478s (59.274FPS), higher
than recent50-55FPS observations but uncontrolled browser load prevents causal
attribution. Retain opt-in shade-before-ROV for further work. Manual candidate
launcher build/port/fast-preview/Try-shading.cmd selects build40 with original
FRAME_CLOCK=0 pacing and no diagnostics; Play.cmd still selects build31.
Visual correctness remains unreviewed;120FPS not achieved. Next: shorten the
remaining ordered depth/blend/store path and independent120Hz presentation.

Build41 specializes raster VRAM addressing/stores for the existing hardware
admission whitelist: PSMCT32/24 and PSMZ32/24. All use the32-bit storage
swizzle; depth applies block XOR24 (byte XOR6144). Hardware reads mask24-bit
values explicitly. ROV stores preserve masked/high bytes without the generic
packed-pixel CAS path; zero masks return and full-word stores omit old reads.
The hardware alias/format proofs are unchanged. Compute and texture addressing
retain their generic paths. Run9 combines this with shade-before-ROV; no
per-edit tests, and manual visual review remains pending.

Build41/run9 completed600 display changes in10.708867s (56.028FPS).
This does not establish benefit over build40/run8 (59.274FPS), with background
load still uncontrolled. Removed the additional hardware-address specialization
from the working shader by restoring build40 shader bytes. Keep build40 as the
manual shading candidate; Play.cmd remains build31.120FPS remains unmet.
Next structural work: native render-target/depth ownership or staged shading;
further minor addressing substitutions have not delivered the required gain.

Build43 adds opt-in DHD_GS_FAST_DEPTH origin-relative bounded FP32 depth.
CPU bounds gradient conversion, FP32 multiply/add, interval endpoint rounding
and original FP64 interpolation error over each primitive bbox. Shader accepts
only an interval with identical integer floors; adds that offset to exact uint
vertex Z with saturation. All uncertain cases retain original double depth.
It reuses depth payload words8-11, demoting coarse mode2 to bbox coarse mode1
for flagged primitives. Build42 was superseded before execution to make the
output temporary explicit rather than rely on HLSL boolean short-circuiting.
One combined run10 uses build43 plus shade-before-ROV. No correctness promotion
or120FPS claim; manual candidate remains build40 until measured/reviewed.

Build43/run10 completed600 display changes in10.676422s (56.198FPS),
below build40's59.274FPS observation. No demonstrated speedup; bounded depth
remains opt-in and is not added to the manual candidate. Browser load remains
uncontrolled. Build40 remains the strongest recent candidate;120FPS not reached.
The FP64 fast-path hypothesis has not produced a measured improvement in this
run. Larger native target/depth handling remains outstanding.
