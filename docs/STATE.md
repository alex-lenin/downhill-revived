# State

Goal: readable, **matching** C/C++ decompilation of Downhill Domination (PS2, USA `SCUS_971.77`):
source that rebuilds the retail executable byte for byte. Later it can serve as the base for a PC
port. PCSX2 is the behavioural reference.

Progress: `python tools/report.py` (objdiff report `progress/report.json`, README table).
2026-10-12: game 98.17% and engine 92.61% of code matched, 4,526 of 4,593 functions (the SDK and
libgcc regions are library code and stay assembly). Left: 28 functions in the queue, 7
hand-written, 164 eight-byte remnants that aren't functions.

## Phase 0 — compiler identification (done)
`docs/findings/002-compiler-sn-prodg.md`. Game code: **SN ProDG 3.01** — cc1 "2.95.3 SN BUILD
1.36" + SN assembler `ps2eeas`, **`-O2 -G8 -fopt-stack`**; C++ `-fno-exceptions` (finding 003).
Library regions are identified (finding 003, `config/libmatch.txt`) and not decompiled.

## Phase 1 — project skeleton (done)
`docs/findings/003-build-system.md`. The retail image rebuilds byte-identical from splat asm +
C/C++ (`python tools/configure.py && python -m ninja` → MATCH), checked by `tools/check_image.py`.
Tooling: `tools/queue.py` (work queue + claims), `tools/try_func.py` (per-function trial build,
with a fast compile-only check first), `tools/diff_func.py`, `tools/promote.py` (into `src/` only
if the whole image still matches; refuses TU merges that would turn C back into asm),
`tools/lost_check.py`, `tools/unit_merge.py` (merge existing units into a wider translation
unit), m2c drafts. Guide for contributors: `docs/WORKER.md`.

## Phase 2 — decompilation (finishing; 28 functions left)
Functions are claimed from the queue, decompiled one at a time against the retail bytes and
promoted only when `try_func` prints MATCH and the whole image still matches. Finding 004
covers how the assembler addresses small data, finding 007 what made trials fast.

2026-10-06: `func_00134CC8` and `func_00240358` promoted after exact function and
whole-image checks. The latter required integration into its existing full translation unit.
Finding 009 distinguishes an assigned-function implementation from a matching assembly stub,
and records serialization of report generation with source promotion. A fresh full-image check
matched all eight checked sections.

2026-10-11: six more matches (00137300, 00137B70, 00148F60, 001554C0, 00171B28,
00228BE8), including two translation-unit merges. The rebuilt image matches all eight
checked retail sections.

2026-10-21 (finding 074): the 25 functions no worker matched have behaviour-equivalent C under
`#ifdef NONMATCHING` next to their `INCLUDE_ASM` (18 new single-function units). The default build
keeps the asm and still matches; `python tools/configure.py --nonmatching` builds the C into
build-nonmatching/ (links, not checked; sizes and the moved ranges are in the finding).
func_00220DA0 is now listed in config/handwritten.txt (trapping adds, pmfhi/pmflo).

Open items:
- `engine` region 0x201000–0x243000 (1,202 functions): same toolchain and flags as game code;
  what it is (engine/middleware) is still open.
- Real TU boundaries: single-function units are merged when evidence appears (shared `$gp`
  data, `.rodata` ownership). func_001F24B8 and func_001C8380 have matching C that still needs
  its translation unit worked out before it can be promoted.
- Per-file flags: two small clusters were compiled `-O0 -fno-opt-stack` (0x21E9A8–0x21EAA0,
  0x2212A0–0x221608); rule in WORKER.md.
- Language: the game is C++ (exception tables, vtables, `this`-style calls). Matching is done in
  whatever form is fastest (mostly C, byte-identical to the method form); a later phase turns
  shared structs into classes and merges functions into reconstructed TUs.
- Hand-written assembly (e.g. func_0021B8A8: registers saved to the scratchpad, `$gp` used as
  scratch) stays `.s`.
- `.vutext`/DVP overlays (VU microcode) stay binary for now.

## Phase 3 — PC port (current)
Plan and starting measurements: `docs/findings/015-pc-port-plan.md`. Port code lives in `port/`
and is applied at build time; the matching sources and checks don't change. Host: 32-bit x86
first. Tools: `port/tools/native_census.py` (clang compile census of `src/`, needs LLVM on PATH),
`port/tools/asm_census.py` (inline asm vocabulary).

2026-10-12: 3,245 of 3,796 units compile for i686 unchanged; the rest is MIPS inline asm,
gcc 2.95 extensions (raw-newline strings, `>?`) and ~150 ordinary C errors.

Step 1 done (2026-10-12): all 3,797 units compile with `python port/tools/build.py` (clang
`--target=i686-pc-linux-gnu`, output in `build/port/`). `port/tools/transform.py` rewrites, at
build time only: inline asm to `ps2_asm()` calls (`port/include/port.h`), `<?`/`>?`, EE 64-bit
`long`, raw-newline strings, and what gcc 2.95 let pass but clang rejects: anonymous
`extern struct` objects in C++, globals redeclared with other types in merged units (section-local
aliases bound by asm label), asm labels added after a function's first declaration, implicit `int`
declarations of later `void` functions, block-scope externs disagreeing with the file's
definition. Line numbers are preserved, so errors point at `src/`.
Known for the data step: some units build one object from adjacent globals (e.g. `STR8` in
001BA3B8 lays an 8-byte string out as two `int`s); the PC link must keep such pairs together.
Step 2 done (2026-10-12): `port/runtime/ps2asm.c` runs every rewritten asm statement on an
emulated EE/FPU/VU0 register file. Each template is parsed once per call site and cached. All
462 distinct templates parse, and asm that reads argument registers directly is refused.
`port/patches.py` holds C bodies for the 4 hand-written libvu0 routines that do that, and
operand fixes for 3 statements that address locals through `$sp`. The three VU0 microprograms
(0x0c0, 0x830, 0x978) are decoded from the ELF's microcode and done natively in
`port/runtime/vu0_micro.c` (finding 016). Tests: `python port/tools/test_runtime.py`, 32-bit
clang-cl. Build and tests use SSE float math with no contraction, so results are deterministic.
Steps 3-4 done (2026-10-12): `port/tools/link.py` (with `port/tools/data.py`) converts the splat
data to x86 assembler and links all units, the asm runtime and the data into
the link output. C++ symbols get their retail names back (`func_X`/`D_X`, 909
renamed). The result: 0 duplicates, 0 errors, and 493 undefined symbols, which are 324 SDK, 114 libgcc
in asm, 26 C library and 29 unfinished game/engine/crt0 functions (finding 017,
`build/port/link/census.json`). Still open: objects defined in C whose neighbours are reached
by pointer arithmetic (`STR8`).
Host platform chosen (2026-10-13): 32-bit Windows, clang `--target=i686-pc-windows-itanium`
(`port/tools/pctarget.py`). That's gcc struct/bitfield layout, as on the PS2 (the MSVC ABI lays out
bitfields differently and 149 units use them), and C++ methods use cdecl rather than thiscall, so C callers and
function tables can call them. Linked with lld-link against the MSVC C runtime into
`build/port/link/dhd.exe`. Asm labels and the data get the Win32 `_` prefix; only the C1/D1
constructor/destructor variants take a class-named function's retail name. Census on Windows: 474
undefined (324 SDK, 114 libgcc asm, 29 unfinished, `fabsf`/`floorf` (inline-only in MSVC x86),
gcc 2.95's C++ runtime (`__dynamic_cast`, exception helpers), `main`), 0 duplicates.
Step 5 done (2026-10-13): `port/tools/recomp.py` recompiles every asm-only game, engine, crt0
and libgcc function the link needs into C on the emulated register file (`port/runtime/recomp.h`,
`recomp.c`), with wrappers that have the C signatures; `link.py` runs it to a fixed point (175
functions: the 29 unfinished, 8 they reach, 138 libgcc). The scratchpad is mapped at 0x70000000,
code addresses with no definition are their PS2 address, `libc_shims.c` has `fabsf`/`floorf`.
Census: 319 undefined, all platform code (295 SDK asm, 23 SDK library, `main`), 0 duplicates
(finding 018). The microprograms the hand-written VU0 functions 0021B7E8/0021B8A8 call run in
the VU0 micro interpreter, checked against PCSX2's (finding 021, round 5). Known limit: `jal`
doesn't set `$ra` (the MIPS exception unwinder can't run).
Graphics study done (2026-10-13, finding 019): each view is one VIF1 DMA chain built by the scene
walk (`func_0021B8A8`, VU0) and the particle renderers, run by 32 VU1 programs in two overlays
(13 in A, particles; 19 in B, world and screen passes) that `XGKICK` to the GS; PATH3 carries setup,
flips and loading screens. Planned renderer: a native GS layer (GIF to GPU, texture cache), tested
on PCSX2 GS dumps, then native versions of the 32 VU1 programs at the VIF1 kick, then GPU fast
paths. `port/tools/vudis.py` disassembles the overlays.
Boot, disc and input (plan step 6, in progress, finding 020).
- `port/hle/` replaces the PS2 system natively:
  - the EE kernel (threads as fibers, semaphores, interrupts);
  - crt0;
  - SIF RPC;
  - the disc (`DHD_ISO`) and file I/O;
  - the sound driver's disc reads;
  - DMA (normal and source chains), VIF0/VIF1, and a VU0 micro-mode interpreter, bit-exact
    against the native microprograms.
- PS2 RAM is also mapped at its own addresses from 0x400000 (the data files hold PS2 pointers).
- Calls that pass arguments only in registers are written out in `src` (58 more sites, still
  MATCH).
- Controllers work: `pad.c`; SDL3 or XInput/Sony HID, and the keyboard (finding 059).
- The game boots, loads and relocates its first resources, runs VU0 scene setup, and keeps
  running its first screen without crashing.
- Open: memory card (`mcserv`), multitap (replies "none"), sound.
Native renderer (2026-10-13, finding 021):
- Automatic comparison tooling (finding 024): `port/tools/compare_pcsx2.py`
  now defaults to independent retail PCSX2 boot, with the BIOS referenced in
  place and an ignored-build GPL adapter for frame-indexed movie input/captures.
  `DHD_FRAME_CLOCK=1` provides opt-in virtual HLE timer/SIF/interrupt time; normal
  runs retain host time. Isolated executables, triptychs, contact sheet, scores,
  worst regions and bounded temporal/line corrections are recorded. GS replay
  remains a separate mode. Real 300/310 s comparison: MAE 39.331451/59.452183,
  block SSIM 0.257401/0.240474, N=2 fields. Retail is racing while native remains
  in the track intro: exact game-state/race-start synchronization remains open,
  so these scores include phase/camera timing as well as renderer differences.
- Race comparison (finding 024 round 3): `port/tools/race_compare.py` boots PCSX2 headless
  and the port with the same input, synchronised on the game's race clock (EE 0x0043EEC8,
  60/s, restarted at countdown and GO). It compares countdown, start, +10, +30 and +50 s.
  The HUD timers agree (00:09/00:09, 00:29/00:29, 00:49/00:48). Proof:
  build/port/compare/race-r10/index.html. Top differences: see-through, inside-out riders;
  floor holes at the gate; grey translucent shapes from +30 s; physics diverges from GO.
- Frame pacing (finding 026, 2026-10-14): retail PCSX2 is vblank-locked (1 frame per field at
  boot, in loads and in the race). The port had no INTC_STAT, so sceGsSyncV never waited and the
  race ran at delta 2 (dt 1/30). It now models INTC_STAT (write 1 to clear; a read with
  VBLANK_START clear runs time to the next interrupt) and charges 1024 bus cycles per kernel
  boundary. Port: 0.997 / 1.000 / 1.000 frames per field (retail 0.974 / 1.000 / 1.000). All
  loads complete. Race delta is 1 from GO, rider positions agree to 0.007 at clock 2 and 0.035 at
  clock 10. Boot to countdown is 530 s of wall time (was 1066 s).
- EE float rounding (finding 026, round 17, 2026-10-14): EE code now runs as PCSX2 runs the EE
  FPU: MXCSR toward zero with FTZ/DAZ, ADD/SUB with one guard bit, DIV/SQRT to nearest; native
  units get it through an IR rewrite (`port/tools/eefloat.py`, `port/runtime/ee_native_float.h`).
  Rider positions are bit-exact to race clock 42; the error first exceeds 0.01 between clocks 75
  and 90 (was 8) and 1.0 between 120 and 300 (was by 30). Load-length stamps (animation clock,
  load timer) still differ but don't affect the race so far; CD latency is not modelled.
- Round 18 (finding 026): all riders bit-exact through race clock 15; the first difference is
  rider 1 at clock 16, traced through func_00231CC0 → func_00133408 → func_00130C08 to a contact
  normal whose y (-3.5e-5) differs before the collision transform (func_00215140) and comes from
  the normalize in func_00213098. VU0 cross product, madd and projection are bit-exact with
  retail inputs. Next: the coordinate feeding that normalize.
- Round 19, real-time speed (finding 027, 2026-10-15): race 18-19 → 24.5 fps, menu 14-16 →
  29.5 fps, both steady over two runs.
  - VU1 is 1.45x faster: a cached block lookup, lazy flags and translated delay slots.
  - GS local copies now run on the GPU, and readback is limited to the dirty pages a read needs.
  - Normal play is paced on the virtual frame clock against 59.94 Hz QPC deadlines: delta
    always 1, never faster than real time.
  - Race dumps unchanged.
  - Left in a race frame: VU1 ~40% and a ~13 ms GPU wait at the game's per-frame store-image
    reads (GPU raster serialised after the CPU).
  - The late-race crash in `func_00214BA0` (float as record pointer) is open.
  - Next: overlap the GPU with the CPU, static VU flag analysis, the crash.
- Round 20 (finding 028, 2026-10-15):
  - **Late-race crash fixed.** `func_00131630` stored the wheel probes' end points as `bp[2]`,
    past `ends.before`. clang split the struct, so the store overwrote the query records'
    pointers. A port patch fixes it; three races of 282-298 s ran with no crash.
  - **Closer to retail.** Riders now match retail exactly at GO+0..+30 (before: none did,
    worst error 36 units).
  - **Lens-flare Z reads deferred.** They are guarded in all RAM views and settled on first
    access, with identical bytes. Paced race 23.5 → 35 fps, wall 27.9 (capped at 30). Race dumps
    unchanged.
  - **VU1 flags.** At most ~27% of VU1 time is flag work; an exact elimination is estimated at
    5-10% of VU1 time and was not done.
  - **Next:**
    - VU1, now 60% of the frame;
    - an audit for the same out-of-bounds struct indexing;
    - the menu wait;
    - online play.
- Round 21 (finding 029, 2026-10-15):
  - **Struct-indexing audit.** The clang analyzer's array-bound check over all units found 62
    sites. One is real: `func_00188890` built a task at `&archive[0x12F30]`, retail's
    `w.pair.load`. It is now patched. The other sites index through pointers, globals or
    escaping locals.
  - **VU1 from inside.**
    - VU1 ops already use SSE and are compiled at -O2. VF registers go through memory.
    - Flags are ~30% of VU1 time and the stall model ~3%. The AOT is ~1.8x the interpreter.
    - The blocks no longer carry VU0 paths: AOT code is 3.6 → 2.1 MB, and the run loop
      11.4 → 9.3% of a race.
    - All exact.
  - **Speed.** Paced race 35.4/35.5 fps, wall 28.1/27.4. With `DHD_MTVU=1`: 39.7/40.0, with
    race dumps identical. The worker is 91% busy, so it is the critical path. A VU1/GS split
    would cap near 50 fps.
  - **Next:** a VU1 recompiler pass (whole-program flag liveness, static stalls, registers
    within a block), and a rendering check of `DHD_MTVU=1` before it becomes the default.
 the port and PCSX2 adapter share
  `port/hle/menu_nav.h` and press each menu's button as soon as it shows (memory-card dialog,
  menu 18's Start-then-Cross). Boot to GO: port 657 → 263 s wall (game 288 → 54 s), PCSX2
  ~105 → 48 s. PCSX2 pins the race seed like the port, so the old reference dumps still hold:
  rider positions are identical to GO+10 s.
- Round 22 (finding 030, 2026-10-15): MTVU is now the default; whole VF moves use
  vectors (9cc58bf8, e714d282), with the name-buffer determinism fix a087f597.
  - Longer VU1 blocks and fused branch/E delay slots pass all 1616 PCSX2 cases
    (1584 generated fixtures + 32 race captures), unchanged bench hash, ref22
    race dumps and byte-identical menu/race BMPs. Retail image check: MATCH.
  - Replay bench 1.267 -> 1.210 s by medians. Paced race 43.7/43.9 fps;
    menu 38.9/36.3. Both bounded runs completed without a crash.
  - SSE locals and static timing prototypes missed the 3% bench threshold and
    were dropped. Conservative CFG liveness found no removable complete flag
    updates with sticky status and drained end state preserved.
  - 60 fps remains open. Next: worker-stage wall timing and VU1/GS overlap;
    per-bit flag coalescing needs a more precise analysis.
- Round 23 (finding 030, 2026-10-15): per-program profiling identifies overlay
  B0078's MSCNT continuation 1040 at 19.12% of wall frame time, 516 calls/frame.
  A guarded native geometry prototype was rejected: median 18.054 us versus
  AOT 12.782 us; clipping caused all ten attempts to roll back. No native path
  is retained. All 32 replay GIF/state comparisons match through fallback,
  not proof of the unexercised native loop. Profiler and comparison tools remain.
  Last accepted paced race is 43.7/43.9 fps; 60 fps remains open. Stop for review.
- Round 24 (finding 030, 2026-10-15): unclipped direct-write SIMD loop passes
  the feasibility gate at 3.31x, with matching CPU/memory/flags and 100% native
  vertices. Full clipping prototype reaches only 1.263x program speedup.
  Initial clipping variant runs at 46.0/46.0 paced race fps; live checkpoints
  show 100% target call/vertex coverage, no rollback/fallback or crash.
  All 32 replay GIF/state comparisons match. Production AOT still passes all
  1616 oracle cases, unchanged bench hash and retail image MATCH. Native stays
  benchmark-only: 3x program / 10% frame gain gates fail. Live state/images and
  lens-flare readbacks remain unverified for it. Stop for review; 60 fps is open.
- Cross-unit EE argument audit generates 183 PC-only call reorderings in 78
  units, including casts and asm aliases. Other ABI mismatches remain flagged
  in build/port/abi/report.json. Full build/link and runtime regressions pass;
  the 1,628-case PCSX2 VU1 comparison still has zero differences.
- Profiling removed redundant GS copies, batched ordered primitives, and moved
  scanout to the GPU. VU interpreter specialization/SSE2 retains zero differences
  against 1,628 PCSX2 cases and 6,000 native VU0 cases. The combined build reaches
  59.8-60.0 fields/s and about 14-15 rendered frames/s in steady main-menu
  intervals. Earlier logs measured game-time ticks, not rendered frames; their
  baseline was 27-29 fields/s and 0.6-1.2 ticks/s. VU1 remains the main CPU cost;
  60 actual game updates/s is not yet sustained. Batching and scanout regressions
  pass, including color/depth aliasing and interlaced fields.
- Corrected SN long-long conversion (128 bits, not 64) and sound producer
  prototype widths. Exact RPC-layout regressions pass; live sound names are
  contiguous at +16. The audio owner's unordered-name fallback can be removed.
  The ABI manifest now has 183 repairs; 1,711 other mismatches are flagged.
- GS core: streaming GIF PATH1/2/3, swizzled 4 MB memory in all 13 formats,
  image transfers, CLUTs and registers; GIF DMA, VIF DIRECT and XGKICK are wired.
- Win32/D3D11 presentation and compute rasterisation draw the memory-card warning.
  Correct VBLANK buffer selection, inverted CSR.FIELD parity and field weaving
  fix the apparent FOR/SAVE/SLOT glyph substitutions. Atlas/field regressions pass.
- VU1: 16 KB memories, BASE/OFFSET, MSCNT, EFU/P and instruction/flag pipelines.
  An independent pinned PCSX2 interpreter agrees bit for bit on **1,603 cases
  across all 32 entries**, including 16 synthetic mesh continuations and 19 live
  shell captures. Rounding, WAITQ, flags and exceptional arithmetic are corrected.
  This is bounded interpreter comparison, not a complete determinism sign-off.
- GS regressions cover large triangle edge signs and exact constant Z32. HLSL
  compiles without warnings; shader source stays under port/gs/. Nonconstant
  Z32 precision and other GS parity limitations remain in finding 021.
- EE bus-clock timers and DHD_AUTOPAD enable menu input. Triangle cancels the
  memory-card warning; with the explicit DHD_SKIP_MOVIES=1 debug bypass, Cross
  reaches the main menu. MPEG/IPU playback remains unavailable by default.
- Deferred SIF RPC replies fix the loader's lost wakeup. Port-only source
  transformations fix low scratch pointers and aggregate/mixed argument ABIs.
- Native audio (2026-10-13, finding 022): the IOP driver is Sony's 989snd on libsd (`MOD/989SND.IRX`); `port/hle/sound.c` replaces its two RPC servers. Banks load by LSN and menu sounds play by name through an integer ADPCM/ADSR mixer ticked deterministically from the vsync count, out via WASAPI (`DHD_SND_OFF=1` for headless). Disc command 0x2C now plays mono/paired VAG and stereo VPK with recorded-rate resampling and deterministic pause/status/positions. Live menu stereo streams and WASAPI initialization work; the 0x19 still-playing query is corrected (it previously stopped voices). Runtime and retail VAG/VPK decode checks pass. Open: queued/raw packed streams, full SBlk grain graph/setters, and reverb; speaker audibility and race transitions remain unverified.
- Audio review round 1 (finding 022): final-mix WAV and full-ID command CSV
  capture are implemented; named-play pan no longer becomes pitch, and bank/
  stream volume plus stream-group/master volume operations are tested. A
  380-second scripted run reaches Mt. Midoule race scenes and preloads T1High
  stereo music. The playable 337.97-second game-time WAV exposes severe race
  clipping (about 47% of samples) and 1,687 unresolved bank plays; the race
  bank has 127 sounds/1,125 grains/176 VAGs, exceeding the current 64-sound
  approximation. Audio tests and link pass; the shared runtime suite stops
  at an unrelated GS capture link failure. Complete SBlk routing/gain/ADSR,
  setters, queued streams and reverb remain open; no PCSX2 audio parity or
  speaker audibility is claimed. Round stopped for review.
- Audio review round 2 (finding 022): fixed two DC sources: waveform-order
  pairing selected loop-ending filler as a bank sound, and streams decoded
  past ADPCM END into that filler. Version-1 SBlk names/grains/tone addresses,
  sample-driven 240 Hz programs, bank/tone gains, signed Q15/pan/group curves,
  real ADSR and depth/pitch/unload/group commands are implemented and tested.
  Final 380-second menu-to-race run uses the port worker's frozen float fixes:
  no logged crash, 210.29 seconds of game audio, 0% mixer/PCM clipping and
  race means about -12/-16 samples. Unresolved bank plays 1,687->1; the
  remaining direct request is malformed, rather than an unparsed bank name.
  Partial operations remain (2,270 params, 1,824 wet requests, 88 LFO grains);
  queued streams, ducking and wet reverb are open. Isolated builds/link,
  audio regressions and retail stream-end/decode tests pass.
  Artifacts: build/port/audio-round2-review/. Stopped for review; no PCSX2
  PCM parity or speaker audibility is claimed.
- Audio review round 3 (finding 022): independent retail PCSX2 WAV capture
  reuses the comparison worker's boot/input adapter and references the BIOS in
  place. Register-driven reverb, all exercised LFO targets, bank/stream setters,
  smooth pan, ducking and FIFO continuations are implemented deterministically.
  The room impulse matches 96,000 PCSX2 scalar sample values exactly. Final
  380.38-second game capture reaches race gameplay, emits nonzero wet output,
  clips 0%, and has no missing/partial counts for exercised commands or grains.
  Malformed named play remains one. Native/ref menu RMS differs +3.10 dB and
  race +6.77 dB, with a 7-second menu-audio onset difference and different race
  states/music phases; gain/event parity remains open. Tests and isolated strict
  link pass. Artifacts: build/port/audio-round3-final/ and
  build/port/audio-round3-reference-state/. Stopped for review; next align states
  and track phase, Gaussian interpolation and the shared malformed-name ABI.
- Audio review round 4 (finding 022): missing stream ADSR 080F/1FC0 caused
  about 6 dB of excess music gain. Streams now advance the retail envelope in
  game samples, retain it across queued cues and freeze while paused. An
  isolated tone/stream fixture through PCSX2's actual decoder/mixer/output
  reduces stream RMS error +6.063 -> +0.040 dB; bank/master/pan cases differ
  less than 0.05 dB. An alternating-sample tone still differs +7.852 dB because
  Gaussian interpolation is pending. Tests, private links and fixture pass.
  Sample-indexed component/gain traces find no gain above unity; a ducker
  release regression restores unity. Menu leading silence precedes the next
  game stream request at 63.0463 seconds. The 380.81-second attempt stops
  audio after 257 seconds instead of reproducing the accepted race, so no
  post-fix live race comparison is claimed. Isolated real race music is
  corrected by about half in level; clipping stays 0%. Atomic report writes
  preserve counters across forced kills; the 90-second validation has no
  missing commands/grains, but one partial 2C flag-04 request. The malformed
  producer is identified: recompiled 001D1C58 passes UW(6) to native 00201B00's
  128-bit name argument. Shared ABI code is not edited. Artifacts:
  build/port/audio-round4-oracle/, audio-round4-final/, audio-round4-report-check/.
  Stopped for review; next stable race/frame sync, Gaussian, flag 04 and that ABI.
- Audio review round 5 (finding 022): Gaussian interpolation replaces nearest
  sampling for voices and VAG/VPK streams, with Q12 pitch and read-only ADPCM
  lookahead across blocks, loops and queued cues. All 65,536 windows match
  PCSX2 GetVoiceValues exactly; the actual core-1 tone render is byte-identical.
  Other isolated fixtures differ at most two sample units / 0.0123 dB;
  alternating-tone error +7.852 -> +0.00597 dB. Finite END-prefetch timing and
  flattened core routing remain outside the continuous-tone parity proof.
  Generic typed scalar 128-bit arguments/returns now retain whole GPRs and
  aligned stack quadwords; signature cache v4 and real i386 regression pass.
  The real named-play builder's isolated malformed case changes 1 -> 0.
  Headers worker should sweep other quadword callers; no src edits here.
  Retail 2C flag 04 means whole-cue loop; native behavior and boundary tests
  are implemented. The 90.16-second menu capture has 0% clipping, zero missing
  commands/grains, and partial 2C 1 -> 0. No stable aligned race claim is made.
  Artifacts: build/port/audio-round5-final/, audio-round5-oracle/,
  audio-round5-producer/. Commits c226c3a8 and ae2a5c45. Stopped for review;
  next quadword caller sweep and stable race/frame-sync comparison.
- Audio review round 6 (2026-10-14, finding 022): exact race-state sync and a
  stable long race are unavailable; captured 210 seconds of menu and retail
  sample-clock RPCs. Live music requests differ by -8.775 s and select different
  SHELL cues. Same-request replay exposed/fixed an intro looping after EOF:
  raw ADPCM repeat flags do not override a nonlooping disc RPC. Replay level
  error +2.105 -> +0.580 dB, but 100-ms RMS correlation remains 0.174 (not parity).
  UI correlation 0.99947 / +0.0224 dB. Retail cold startup is about 205 ms later;
  ambient grain/RNG timing remains unisolated. Static END FIFO stops about 16
  source samples early; finite menu tails are zero or <=3 raw units, race
  effects unverified. CPU caching/direct windows/reverb wraps reduce stereo
  music 5.21 -> 2.60 ms per audio second; full synthetic ceiling 94.27 -> 71.35.
  Optimization PCM, tone and reverb oracle remain byte-identical; tests/private
  link pass. Final 100.18-second PCM prefix equals the earlier 210-second run,
  with 0% clipping and zero missing/partial menu operations. Retail replay finds
  550 movie-only missing calls before menu. No ABI sweep or renderer/reset edit.
  Commits 1c3e3b8e, b74a7f72; artifacts build/port/audio-round6-*. Stopped for
  review; next stream readiness, ambient grain/RNG and stable race alignment.
- Audio review round 7 (2026-10-14, finding 022): explicit 32-bit O1 benchmark
  (production HLE currently O2), pinned 3x60-s trials: 24 bank voices + music +
  both wet cores 42.71 -> 29.43 CPU ms/audio second; 48 bank voices + music
  69.01 -> 50.78, still 5.08% of one Ryzen 7600 core. Optimization PCM and all
  ten reverb modes remain exact; O1/O2 tests and private strict link pass.
  The -8.775-s music-request offset precedes the mixer: native skips movies,
  and menu initialization seeds its cue-selection LCG from EE Timer 2.
  Parity aligns captured retail requests, without changing game RNG or ABI.
  Stream ready/playback deadlines use only rendered game samples; host read
  cost/chunking regressions pass. Cold startup service is an explicit timing
  approximation, not full CDVD emulation. Retail R250 state is generated from
  seed 12345, with held tone parameters and corrected random delay/pitch bend/
  sample-hold initialization. Same-request 130-s RMS correlation 0.174 -> 0.9591,
  level error +0.580 -> -0.0273 dB; completion reply mismatches 12 -> 0, clipping
  0%. Menu missing/partial 0; replay missing 551 -> 550 (movie-only calls,
  corrected prior omission of one GetMasterVolume query). Native 100.15-s and
  replay 210-s WAVs in build/port/audio-round7-*. Commits bcff9f52, 583b07c4.
  Stopped for review; next synchronized stable race, grain timing outlier,
  END-prefetch and broader readiness/seek cases. No shared worker edits.
- Audio review round 8 (2026-10-14, finding 022): required CPU benchmark
  uses normal production HLE O2, exactly 24/48 active voices with both ROOM
  wet cores, five 120-audio-second trials on Ryzen 7600. Optimizations reduce
  interleaved medians 26.43 -> 19.01 / 54.82 -> 29.82 CPU ms/audio second;
  final timing-fix code measures 17.84 (1.78% of one core) / 29.04 (2.90%).
  Gaussian/reverb SSE2, phase-local ADSR and held gains retain exact DSP values.
  Four-zero-envelope driver polls fix the 26.333-ms ambient outlier: all 69
  sampled R250 states match, range -1.333 to +3 ms. Static END-prefetch cutoff
  matches PCSX2 at all 16,383 nonzero pitches for one/four blocks (32,766 cases).
  Deterministic failed/canceled-read, seek-offset and split-chunk regressions pass;
  seek-distance/cancellation latency remains an explicit HLE approximation.
  Menu 130-s RMS correlation 0.9591 -> 0.99185, gain error -0.0317 dB; all
  semantic completion replies match, clipping 0%. Fresh 100.08-s normal-O2
  native menu and 210-s request-replay WAVs under build/port/audio-round8-*.
  Missing calls 550 -> 550, exactly untouched FMV 3B-3E/40; menu missing and
  partial operations 0. Commits 5587507c, c4e5d3fe, be71f074. O1/O2 audio
  tests and private strict port link pass. Independent stable retail race
  input/frame alignment is still unavailable; no renderer/ABI/source edits.
  Stopped for review; next stable aligned race and wider retail seek calibration.
- Animation NaNs are fixed: 13 generated callback wrappers now marshal time in
  $f12 and return $f0. Uploaded bone matrices are finite; the title/menu scene
  and logo draw. The dim text was still fading in: a 160 s settled capture shows
  bright menu text (`build/port/main-menu-settled.bmp`). The newer performance measurements above supersede the initial slow run.
- Single Player now confirms the rider and reaches bike inventory through
  START NEW GAME, game-mode and race-type selection. Port fixes cover the
  resource-ID width, 45 more ASCII-looking menu pointer slots, the inventory
  timer thunk and a vtable getter's false 128-bit return. Regression tests pass.
  START reaches SELECT LEVEL (MT. MIDOULE FR): build/port/race-thunks-006.bmp.
  Thirty forwarding callback signatures now retain their input/return ABI, and
  six member-pointer units use SN's original representation. Track loading now
  passes object registration and particle setup after an explicit-this repair
  and 26 audited relocations of pointer fields splat classified as floats.
  ALPINE track/rider data loads. The lighting output argument and animation
  dispatcher's missing float input/return are also repaired; a playable race
  is still under test. GET READY TO RACE draws rider geometry, with an incorrect
  close-up (build/port/alpine-dispatch-004.bmp). Seven data-labelled timer
  thunks now resolve to verified native forwarding targets. PCSX2/Play!/PS2Recomp speed references and the
  initial graphics synchronization audit are recorded in finding 021.
  The camera temporary is aligned and the alpine world now draws during the
  track intro (build/port/alpine-aligned-024.bmp). A subsequent rider-trail
  vector-by-value ABI crash is repaired with indirect inputs and local copies.
  Runtime regressions pass. Depth/texture artifacts remain; playable racing
  and a valid race frame counter are still under test. The indexed rider
  setter now forwards its missing float. Native GS store-image returns the
  race effects' 4x4 depth queries; all-format readback regressions pass.
  CPU uploads now synchronize/update only changed GS pages, with overlap
  regressions passing. DHP3 measures changed scanout buffers during races.
  Two more rider update callers now pass animation handles indirectly; real
  caller regressions verify full contents and both progress/status branches.
- GS mipmapping/filter selection and mip snapshot hazards now have runtime and
  49-pixel native/PCSX2 software parity regressions (zero differences). Synthetic
  and live GS captures/replay stay under build/. Current-field bob and double
  depth are tested on CPU/GPU (576 depth pixels match PCSX2 software replay).
  Verified signed GS fog blending passes all 512 native/PCSX2 cases, including
  equal colors and negative differences. Purple race geometry still needs
  upstream tracing; PATH1/VU entry 0x1040 fully-fogged draws are identified.
  A 120-field waterfall sequence has a fresh PCSX2 software replay comparison
  (`build/port/race-fog2-comparison.png`); 32 new race VU inputs match the oracle.
  The later bad invocation and ASCII-address crash remain open; see finding 021.
- Optional copied graphics worker (`DHD_MTVU=1`) passes ring/fence/FIFO and
  queued VIF regressions. It reaches Mt. Micoule race/tutorial geometry;
  purple polygons, speckle, field combing and a later crash remain. Presentation
  still drains the queue, so this tier stays opt-in and full speed is open.
- Optional VU1 AOT emits 906 straight-line C blocks under build/ only.
  All 3,164 cases across 32 entries agree with PCSX2 and the separately compiled
  interpreter, including full memory and GIF output. Menu comparison: about
  8-11 displayed buffer changes/s interpreted versus 12-13 with AOT; 60 FPS
  remains unmet. Pipeline queues are still dynamic; static analysis is open.
- Full PCSX2 comparison with the corrected live inputs: 1,628 cases across all
  32 VU1 entries, zero differences. Numbered captures support longer runs;
  finding 021 explains the explicit movie bypass and actual playback needs.
- DHD_DUMP snapshots resume the game before CRT logging, avoiding a watchdog
  deadlock; thread/semaphore details print at the next safe kernel boundary.
- Full PC build: 3,797/3,797 compile; link has 23 existing SDK stubs and no
  unresolved symbols. Runtime tests pass (6,000 VU0 comparisons, GS, VU1, VIF1,
  controller/timer/SIF, 845 callback ABI cases, pointer conversion and D3D11 WARP).
  Only port/ and docs/ changed this round;
  matching PS2 sources and the prior eight-section MATCH are untouched.

Shared-header round 1 (2026-10-13, finding 023): `include/animation.h` and
`include/vector.h` serve 24 units. Nineteen animation leaf Handle views,
the dispatcher and affected callers share types/prototypes; aggregate/pointer
and dropped float input/return disagreements are fixed at source. Five port
patch entries are removed. All eight retail sections MATCH; all 24 affected
PC units compile and runtime regressions pass. Three retail-sensitive units
keep local parameter forms/port repairs. Remaining Handle views, x792C
rider/body prefixes and the 00212048 iterator are next.

Shared-header round 2, constructor outputs: 11 units share the typed
00203590/002036C8/002037D0 API; 12-byte locals are now 16-byte Handles,
and pointer-typed resource selectors are integers. All eight retail sections
MATCH; 11/11 touched PC units compile. Finding 023 records the removed port
patch and concurrent-file staging boundary. Remaining Handle views and
rider/body prefixes are in progress.

Round 2 Handle pass: transition/reset/range definitions and five more caller
units share the Handle/API; eight-section MATCH and 6/6 PC units compile.
The dispatcher and three reset-copy callers retain proven retail-sensitive
forms (finding 023). Rider/body render prefixes are next.

Round 2 render-prefix step: include/rider.h now serves 18 render/attachment
units, unifying x792C body nodes, their +40 Matrix4, and +16D0/+16D4 chains.
The rider index and 001AE8A8 matrix output are corrected. Eight-section MATCH;
18/18 touched PC units compile. Spatial iterator layout is next (finding 023).

Round 2 spatial step: include/spatial.h serves 17 units with one 0x70-byte
iterator and shared filter/reference/hit types. Three collision-call PC
permutations are retired; one retail-sensitive caller retains its audited
alias. Eight-section MATCH; 17/17 touched PC units compile. Vector coverage
is the remaining step in this round (finding 023).

Shared-header round 2 complete for review: 60 distinct units touched;
animation output/transition handles, 18 rider/render prefixes and 17 spatial
units are shared, and vector/matrix coverage broadens in 11 units. All eight
retail sections MATCH before each commit; every touched i386 unit compiles.
Finding 023 records every discovered disagreement, retained local forms,
unstaged concurrent-port integration edits, and the next review plan.

Port race follow-up (finding 021): retained the ignored integer slot in the
rider phase setter, widened six further animation lookup locals, and repaired
the speedometer's native double formatting. Protected real-caller fixtures
pass; the original idle case and handle overflow are reproduced by the old
forms. Full normal frozen-input build/link passes and runs 350 seconds without
a logged crash. Rendering remains incorrect: later NPC rates/tracks become
NaN and the race view degenerates to fog color. A protected run still crashes
in the keyframe handler after race entry. Sixteen new suspect VU invocations
match PCSX2 exactly; trace the controller rate's physics/tween producers next.
Later shared-header commits are not included in the frozen-input validation.

## Facts

- `.text` 0x10a000–0x263bc0 (~1.35 MB), stripped. C++ (only 4 TUs with exception info).
- 5,690 functions (splat): game 3,534, engine 1,202, sdk/libc/libm 802, libgcc 147, crt0 5.
- `_gp` = 0x2A40F0. Single PT_LOAD at file offset 0x1000.
- Disc: `SYSTEM.CNF` BOOT2 = `cdrom0:\SCUS_971.77;1`, VER 1.00.

## Environment
Windows (Git Bash or PowerShell); the build runs natively. Toolchain: `python
tools/setup_toolchain.py` (pinned + hashed in `toolchain.lock.json`: SN ProDG 3.01, ps2dev
binutils 2.45.1, SDK 2.4 libs for identification). m2c: clone into `tools/ext/m2c`. The retail ELF
is not in the repo: extract it from your own disc (`7z e "<USA iso>" SCUS_971.77` into `iso/`,
sha256 adfda7b7…f0cf0c).

Shared-header round 3, spatial transforms (finding 023): twelve units share
the four SpatialReference transforms, moving-transform sampler/velocity,
and transposed matrix product. Typed aligned buffers and offset checks
protect the layouts; integer/pointer and false-return disagreements are
fixed. Twelve PC units compile and all eight retail sections MATCH. The
round-2 port patch/manifest handoff is committed with isolated hunks. Rider
overlays and remaining animation callers are next.

Shared-header round 3, rider overlays: sixteen further units use the shared
0x7BD0 Rider stride, physics/body matrices, Handle arrays, component probes
and rider index. Added size/offset checks pass both compilers; all sixteen
PC units compile and all eight retail sections MATCH. Finding 023 records
the evidence and the +10 bone matrix extent correction.

Shared-header round 3, animation callers: twelve units share Handle queries
and dispatcher calls; 0013C400's 12-byte scratch local is fixed. Thirteen
PC dispatcher permutations retire; nine retail-sensitive calls retain
regenerated guards. All twelve PC units compile and all eight sections
MATCH. Source/manifest edits are committed together (finding 023).

Shared-header round 3, blend/tween setters: tween.h and 23 units share
float-output tweens and the rider blend-controller API. 001B1698's 58
reported return disagreements are reduced to three documented discarded-
return exceptions; five more argument permutations retire. All 62 distinct
round-3 PC units compile and all eight retail sections MATCH. Finding 023
records coverage and the next review plan; no runtime visual improvement
is inferred from matching/compile checks.

Shared-header round 4, adjacent setters (finding 023): 27 units share the
001B3170/001AE630 typed blend APIs and full embedded tweens. Sixteen retail
setup-order exceptions retain AST-guarded PC permutations. All eight retail
sections MATCH; 27/27 touched PC units compile with committed patches.py.
Fresh full ABI baseline: 1,634 flags, 162 permutations, 47 text patch operations;
counts unchanged by this batch. Wider Handle views and sound APIs are next.

Shared-header round 4, Handle step (finding 023): ten units share complete
lookup outputs and constructor-proven 254-slot rider/component Handle arrays.
The dispatcher now shares its input type/prototype; seven retail aggregate
leaf exceptions remain. Six port patch entries retire with their source.
ABI flags 1,634 -> 1,630; 162 permutations remain. Full eight-section MATCH,
10/10 touched and 40/40 rider-header PC units compile. Sound is next.

Shared-header round 4, sound step (finding 023): sixteen units share the
named-play API, 64-bit bank name, 128-bit sound name, full RPC payload and
0x40-byte sound emitter. Incorrect bank-ID pointer views are integers.
ABI flags 1,634 -> 1,261; 162 permutations remain; six patch entries retired
this round. Eight-section MATCH and 16/16 touched PC units compile.

Shared-header round 4, queue/census handoff (finding 023): 26 more units
share the queue API and item/info/table records; all 65 false void-return
calls to 00233BA8 are corrected. Total 79 units converted this round.
Fresh full ABI flags 1,634 -> 1,196; 162 permutations remain. Six patch
entries retired by this worker. The rider-time fixture searches include/.
Full retail MATCH, 79/79 touched PC units compile and runtime tests pass,
including 6,000 VU0 comparisons. Particle, node, camera, sound and GS struct
candidates are ranked in 023; round 4 stopped for review.

Shared-header round 5, timers (finding 023): 47 units share the timer pool,
20-byte node, callback/context fields and pointer-returning registrations.
The null-context no-argument callback path and matching C++ release method
remain explicit. ABI flags 1,196 -> 1,161; permutations 162 -> 162.
All eight retail sections MATCH; 47/47 touched native units compile.

Shared-header round 5, sound callbacks (finding 023): 38 units share the
completion callback, 64-bit callback data and 16-byte command-return record.
The asynchronous send/named-play APIs are correctly void; two old PC return
patches (five operations) retire. ABI flags 1,196 -> 1,073; permutations
162 -> 162. Eight-section MATCH and 38/38 native units compile.

2026-10-13 shared-header round 5 particle batch: 27 units share particle
records and index-return spawners; 43 guarded ABI permutations retired.
ABI flags 1,073 -> 1,066, permutations 162 -> 119. Full image MATCH and all
27 touched native units compile. Finding 023 records ten retail scheduling
exceptions and the retained scratch-alignment patch.

2026-10-13 shared-header round 5 scene batch: 19 units use the shared
0x100-byte instance/transform prefix and explicit 64-bit flags. Constructor
and list iteration share it with rider nodes. Full image MATCH, all 19 touched
native units compile; flags 1,066 and permutations 119 unchanged. Finding 023.

Port race round 2 (finding 021): the EE float lead is confirmed. Shared port
helpers clamp recompiled/EE/VU0 macro arithmetic and repair the bounding
sentinel subtraction in 00119AD0. 00117A10 now uses saturating EE float-to-int
conversion before its grid clamp. Generated PC header copies retain SN virtual
layout after EventQueue moved into a shared header. No matching source/header
or retail checker was edited by the port worker.

A frozen full normal build compiles all 3,797 units and links with 23 existing
SDK stubs, no duplicates or unresolved symbols. A 350-second trap run reaches
the race without game FP hits/crashes; a normal 350-second run also logs no
crash. Three-second captures cover 250-340 s. World geometry remains visible,
but rider/bike rendering is still incorrect. Same-packet PCSX2 race-start GS
replay: MAE 0.356352, SSIM 0.998537; this is not independent retail simulation
proof. VU1 AOT has zero differences in 1,584 pinned PCSX2 cases. Standalone
float/header fixtures pass; the final frozen runtime suite stops at the audio
fixture (8,202 expected-PCM failures), outside this worker's edits. Later shared
source changes are outside the frozen validation. Next: trace NPC rate inputs
and the first bad rider bone/camera matrices before VIF upload. Stop for review.

2026-10-13 shared-header round 5 camera batch: 21 units share the rider camera
prefix, 16-byte blend vectors and selected APIs. One unused argument removed;
001B30B8 stores a component pointer. Full image MATCH, all 21 native units
compile; ABI flags 1,066 -> 1,065, permutations 119 unchanged. Finding 023.

2026-10-13 shared-header round 5 complete: 172 distinct source units converted
across timer/callback, sound completion, particle, scene, camera and GS packet
families. ABI flags 1,196 -> 1,065; guarded permutations 162 -> 119. Two patch
entries/five text operations removed; actual text count 35 after an independent
renderer addition. Full image MATCH, touched native compilation and complete
runtime suite pass. Finding 023 records validation, retained exceptions and
next targets. Stopped for review.

2026-10-13 shared-header round 6 rider/NPC priority batch: five units share
interaction/physics types. 00113398 now forwards its float distance into
00113588; its vector scratch is explicitly aligned. Real native gate/control
fixture passes; full image MATCH, 5/5 touched native units compile. ABI flags
1,065 -> 1,059; 119 permutations unchanged. Finding 023; race/bone outcome
still requires independent port testing.

2026-10-13 shared-header round 6 NPC animation priority: three units replace
by-value handle/rate calls with shared pointer APIs. The long selector uses
an explicit native bridge, preserving a required MIPS-only DI schedule.
Native state/handle/rate fixture passes; full image MATCH, 3/3 native units
compile. Flags 1,059 -> 1,051; one patch entry/text operation removed,
119 permutations unchanged. Finding 023.

2026-10-13 shared-header round 6 animation controllers: 14 units share the
0x64 dispatch/fade record and pointer-returning 001AC780 initializer. Full
image MATCH, 14/14 native units compile. ABI flags 1,051 -> 1,025; 119
permutations unchanged, no further patch removed. Finding 023.

2026-10-13 shared-header round 6 object registry: 42 units share object-pointer
storage/lookup and one-argument registration. Twenty-one EE resource-setup
calls require explicit scheduling aliases; native bridges call the canonical
API. Full image MATCH, 42/42 native units compile; flags 1,025 -> 1,003,
119 permutations unchanged. Finding 023.

2026-10-13 shared-header round 6 queue/team APIs: 26 units share SKAT queue
state/handle records and four-team membership output. Both status returns
are int, correcting 46 void-call flags. Full image MATCH, 26/26 native units
compile; ABI flags 1,003 -> 957, 119 permutations unchanged. Finding 023.

2026-10-13 shared-header round 6 rider input priority: 13 units share thirteen
0x50-byte channels and explicitly forward the float input value. Native real
channel-routing/gate fixture passes. Full image MATCH, 13/13 native units
compile. Flags 957 unchanged; permutations 119 -> 117; three more text
operations/one entry retired. Finding 023.

2026-10-13 shared-header round 6 NPC FP priority: six units share the true
float-return proximity API. Wrong void callers leaked x87 stack entries;
real caller/callee fixture passes with status 0 and the restored wrong
prototype fails with stack-fault/invalid status 0x41. Full image MATCH,
6/6 native units compile; flags 957 -> 952, permutations 117 unchanged.
Finding 023; live bone/race causation remains to be traced.

2026-10-13 shared-header round 6 complete: 120 distinct source units converted.
Priority fixes cover NPC x87 return-stack leaks, animation handle/selector
slots, interaction distance and input float forwarding. Real-path fixtures
pass; restoring the wrong NPC float prototype fails with invalid/stack-fault
bits. ABI flags 1,065 -> 952; permutations 119 -> 113. Two patch entries/four
text operations removed; actual text count 36 after five independent additions.
Full image MATCH, 120/120 native units compile, fresh full audit/manifest and
complete runtime suite pass. Finding 023 records exceptions and priority
handoff; live race/bone causation remains for port tracing. Stopped for review.

2026-10-13 shared-header round 7 path track: four units share PathTrack and
001DDA50 float return; two void callers fixed. Full image MATCH, 4/4 native
units compile; ABI flags 952 -> 950, permutations 113 unchanged. Finding 023.

2026-10-13 shared-header round 7 complete: 53 units; all 43 floating-return
contract disagreements across 29 callees fixed. Strict direct/implicit/header/
callback/cast guard and 48 reviewed assembly FP contracts added; stale wrappers
retain FP results and software-double widths. Flags 952 -> 914, permutations
113 unchanged; one patch entry/two text operations retired. Full image MATCH,
53/53 native units compile, full audit/manifest and owned-snapshot runtime
suite pass. Other workers' observer diagnostics stay unstaged. Finding 023.
Stopped for review.

2026-10-14 shared-header round 8 complete: 191 matching source units plus five
native-only scheduling exceptions share the path/menu/format/impulse/scalar/
heading/effect/particle-update APIs. FP argument disagreements: 39 C sites,
24 bridge contracts and two formatted-vararg paths -> zero. DI disagreements:
39 C calls/callback refs, one bridge contract and two formatted paths -> zero.
The audio TI fix covers all 13 public fixed-signature TI APIs; no additional
fixed-signature mismatch, and both formatted TI paths are now preserved.
Strict guards cover native definitions, callbacks/tables/casts, reviewed asm
inputs, signature caches and unknown wide vararg bridges. Seven additional
assembly FP forwarding return contracts are reviewed. ABI flags 914 -> 757;
permutations 113 -> 102. Zero patches removed; five entries/15 text operations
added for retail scheduling exceptions, staged with the matching headers/source.
Full image MATCH, 196/196 native units compile, full 3,797-unit ABI audit/manifest,
scalar regressions, runtime suite and linked port pass. Finding 023. Stop for review.

2026-10-14 shared-header round 9 rider blend API: five caller units use shared
retail scheduling aliases/native adapters, retiring 19 permutations and three
void-return flags. Real setter fixture passes. Full image MATCH, 5/5 native
units compile and strict ABI guards pass. Flags 757 -> 754; permutations
102 -> 83. Finding 023.

2026-10-14 shared-header round 9 menu sounds: merged 001EB1D8.cpp shares
zero-argument select/back APIs, correcting 29 unused menu-pointer declarations.
Full image MATCH and native unit compile pass. Flags 754 -> 725; permutations
83 unchanged. Finding 023.

2026-10-14 shared-header round 9 resource name: 14 units share the pointer
getter, correcting 13 unused input declarations and eleven int result views.
Full image MATCH, 14/14 native units compile. Flags 725 -> 712; permutations
83 unchanged. Finding 023.

2026-10-14 shared-header round 9 secondary/body blend APIs: nine caller units
share scheduling adapters, removing fourteen permutations. Full image MATCH,
9/9 native compiles and real three-setter fixture pass. Flags 712 unchanged;
permutations 83 -> 69. Finding 023.

2026-10-14 shared-header round 9 animation selection: nine units share selector
and menu resource APIs; nine permutations and one width-rewrite text operation
retired. Full image MATCH, 9/9 native compiles, NPC/selector fixtures and strict
3,797-unit scalar guards pass. Flags 712 unchanged; permutations 69 -> 60.
Finding 023.

2026-10-14 shared-header round 9 UI style: 27 units share the named 32-byte
rectangle/RGBA record and pointer-return initializer/void apply APIs. Full image
MATCH, 27/27 native compiles and real initializer/apply/canary fixture pass.
Flags 712 -> 696; permutations 60 unchanged. Finding 023.

2026-10-14 shared-header round 9 complete: 60 distinct source units, committed
by API family. Flags 757 -> 696; permutations 102 -> 60; one text operation
retired, zero complete patch entries. All 53 HEAD text operations apply once.
The particle caller trace proves B028's missing float is velocityScale, with
elapsed age/frame count supplied separately; no direct race-clock cause is
established. Finding 023 includes the round-5 onward port test checklist.
Full image MATCH, 60/60 native compiles, full 3,797-unit strict ABI/manifest
check and complete runtime suite pass. Standalone selector-header fixture
integration corrected. Stop for review.

2026-10-14 shared-header round 10 menu items: 60 units share selection/item
append/count APIs, correcting ignored selection setup and void count returns.
Full image MATCH, 60/60 native compiles and real selection/item fixture pass.
Flags 696 -> 676; permutations 60 unchanged. Finding 023.

2026-10-14 shared-header round 10 checker review: 81 false positives retained
with one-line reasons (76 exact SI no-prototype calls, five verified packed
Color calls). Current 595 real declaration disagreements / 60 permutations.
Proof checks reject changed promotions, counts, returns or wider Color storage;
strict float/DI/TI guards and full audit/manifest pass. Full image MATCH.
Finding 023.

2026-10-14 shared-header round 10 collision batches: 15 units share the 40-byte
query/output record and int batch return plus hit/sweep scheduling adapters.
Full image MATCH, 15/15 native compiles. Real flags 595 -> 587, 81 reviewed
false positives; permutations 60 -> 50. Finding 023.

2026-10-14 shared-header round 10 particle spawners: four caller units use shared
scale-first scheduling adapters, retiring all ten spawner permutations. Full
image MATCH, 4/4 native compiles. Real flags 587, reviewed false positives 81;
permutations 50 -> 40. Finding 023.

2026-10-14 shared-header round 10 ground probes: 19 units share the 16-byte
probe record and pointer/pointer/int/float API, retiring six rider/NPC query
permutations. Full image MATCH, 19/19 native compiles and repeated float-return/
output-lane fixture pass. Real flags 587; 81 reviewed false positives;
permutations 40 -> 34. Finding 023.

2026-10-14 shared-header round 10 animation weight api: 3 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 3/3 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
34 -> 29. Finding 023.

2026-10-14 shared-header round 10 rider proximity query apis: 7 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 7/7 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
29 -> 23. Finding 023.

2026-10-14 shared-header round 10 animation track search api: 4 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 4/4 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
23 -> 20. Finding 023.

2026-10-14 shared-header round 10 spatial primitive query apis: 7 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 7/7 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
20 -> 15. Finding 023.

2026-10-14 shared-header round 10 particle emission control apis: 8 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 8/8 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
15 -> 9. Finding 023.

2026-10-14 shared-header round 10 sound fade and pacing apis: 3 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 3/3 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
9 -> 7. Finding 023.

2026-10-14 shared-header round 10 motion interpolation apis: 4 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 4/4 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
7 -> 5. Finding 023.

2026-10-14 shared-header round 10 ui region and color apis: 5 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 5/5 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
5 -> 2. Finding 023.

2026-10-14 shared-header round 10 animation time query api: 2 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 2/2 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
2 -> 1. Finding 023.

2026-10-14 shared-header round 10 four-lane vector scale api: 2 units share
canonical native contracts with explicit EE scheduling adapters. Full image
MATCH, 2/2 native compiles and scoped scalar guards pass.
Real flags 587 -> 587; 81 reviewed false positives; permutations
1 -> 0. Finding 023.

2026-10-14 shared-header round 10 SIF copy: two units share the request record
and byte-count return, correcting eight void calls. Full image MATCH, 2/2
native compiles. Real flags 587 -> 579; 81 reviewed false positives; zero
permutations. Finding 023.

2026-10-14 shared-header round 10 load queue: five units share global queue
processing, correcting eight unused context-pointer declarations. Full image
MATCH, 5/5 native compiles. Real flags 579 -> 571; 81 reviewed false positives;
zero permutations. Finding 023.

2026-10-14 shared-header round 10 complete: 143 source units, API-family commits.
All 60 permutations retired. Flags 696 -> 563: 52 real contracts repaired,
81 original false positives reviewed; macro coverage adds one more reviewed SI
call (82 current). Remaining real flags: 304 returns, 259 counts/variadic.
Track projection shares int status and three-float angles; position/angle
prefix extents are corrected, without claiming a live standalone overflow.
Full image MATCH, full 3,797/3,797 normal native build, strict scalar/manifest
checks and complete runtime suite pass. Zero owned patch changes; all 65
current committed text operations apply once. Finding 023. Stop for review.

Round 11 rider event submission: Real flags 563 -> 556; reviewed false positives 82 -> 82; gameplay callee-range flags 296 -> 289. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 global entry predicate: Real flags 556 -> 549; reviewed false positives 82 -> 82; gameplay callee-range flags 289 -> 282. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 render synchronization: Real flags 549 -> 542; reviewed false positives 82 -> 82; gameplay callee-range flags 282 -> 275. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 track bin search outputs: Real flags 542 -> 536; reviewed false positives 82 -> 82; gameplay callee-range flags 275 -> 269. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 motion key sampling: Real flags 536 -> 531; reviewed false positives 82 -> 82; gameplay callee-range flags 269 -> 264. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 derived menu construction: Real flags 531 -> 525; reviewed false positives 82 -> 82; gameplay callee-range flags 264 -> 258. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

2026-10-14 port round 3: race-state traces show advancing simulation, not
repeated race restart. Canonical DI bridge widths repair the HUD formatter;
shared float/scalar/quadword fixes are incorporated in frozen source head
71a18a1b. Nine native EE exceptional-arithmetic sites use shared finite math
and saturated conversion. A deferred x87 underflow resolves to the virtual
scale result in 00132918; the assembly getter 00261E48 now has a float/this
contract. Fiber FP state, per-frame diagnostics/rearming, header asm-label
prefixing and coprocessor host-mask scopes have execution regressions.
Trap+state run r3-final-trap6 reaches 350 s with no SSE/x87/crash report.
Post-trap full build 3,797/3,797 and frozen runtime suite pass; strict link has
the established 23 SDK stubs, no unresolved symbols or duplicates. Normal quiet
and pedaling runs both finish at 350 s without a crash; pedaling advances the
clock/speed with no race reset and two sampled rider-0 crashes. Native frames
012/014/017/024/030 under build/port/race-proof/r3-final-pedal cover countdown,
GO,+10,+30,+49 s. Race-start GS replay is pixel-identical, not retail EE proof.
Rider/bike shards and outside-boundary physics remain. Bone 6 has one 7.4%
nonorthogonal asynchronous sample during a crash; synchronous source tracing
is needed to distinguish tearing/blending from corruption. Next: source bone
matrices at 0023AE78 and first deformed skin vertex/palette/camera data; VU0
micro arithmetic parity remains open. Finding 021 records sites, hashes,
frames, audio-fixture details and owned commits. Stop for review.

Rider/bike shards fixed (2026-10-14, finding 021 round 6). The cause was host stack quadwords
that weren't 16-byte aligned: lq/sq/lqc2/sqc2 name the aligned quadword on the EE and in
recompiled code. The port build now aligns every aggregate local (build.py), and the inline asm
runtime masks the address like the EE. DHD_QUAD_CHECK went from 64 unaligned programs to 0.
The race renders whole riders from the countdown through +50 s. VU0 micro mode matches PCSX2
on all captured race inputs.

Round 11 global menu refresh: Real flags 525 -> 519; reviewed false positives 82 -> 82; gameplay callee-range flags 258 -> 252. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 empty menu finalizer: Real flags 519 -> 513; reviewed false positives 82 -> 82; gameplay callee-range flags 252 -> 246. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 player slot event state: Real flags 513 -> 507; reviewed false positives 82 -> 82; gameplay callee-range flags 246 -> 240. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 rider AI update: Real flags 507 -> 504; reviewed false positives 82 -> 82; gameplay callee-range flags 240 -> 237. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 RTC output forwarding: Real flags 504 -> 499; reviewed false positives 82 -> 82; gameplay callee-range flags 237 -> 232. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 load transition initialization: Real flags 499 -> 495; reviewed false positives 82 -> 82; gameplay callee-range flags 232 -> 228. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 derived constructor retail return: Real flags 495 -> 495; reviewed false positives 82 -> 82; gameplay callee-range flags 228 -> 228. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 reviewed SI bridge cache contracts: Real flags 495 -> 495; reviewed false positives 82 -> 82; gameplay callee-range flags 228 -> 228. Zero permutations; zero owned patch changes. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 11 shared headers complete (65 units / 12 API families).
Real flags 563 -> 495; reviewed false positives 82 -> 82; gameplay callee-range
flags [0x110000,0x1C0000) 296 -> 228. Zero permutations and scalar guard errors.
RTC output forwarding and constructor this return are restored; shared SI
bridges override stale caches. Full image MATCH, 3,797/3,797 native compile,
strict ABI/manifest audit and full runtime suite pass; port link exits 0 with
23 existing SDK stubs. Zero owned patch changes; all 65 text operations apply
once to committed source. Finding 023. Stop for review.

Round 12 rider and VIF matrix helpers: Real flags 495 -> 489; reviewed false positives 82 -> 82; gameplay callee-range flags 228 -> 228. Particle-range flags 26 -> 20; zero permutations. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 12 particle descriptor and spawn result: Real flags 489 -> 488; reviewed false positives 82 -> 82; gameplay callee-range flags 228 -> 228. Particle-range flags 20 -> 19; zero permutations. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 12 three-section particle spray emitters: Real flags 488 -> 486; reviewed false positives 82 -> 82; gameplay callee-range flags 228 -> 228. Particle-range flags 19 -> 19; zero permutations. Full image MATCH and selected native/scalar checks pass. Finding 023.

Round 25 native geometry accepted (finding 030): B0078/1040 is default-on with
DHD_VU1_NATIVE=0 retaining AOT. Paced race 45.8/45.7 fps versus 42.9/43.0 off.
All four ref22 race dumps match under existing exclusions; menu/race BMPs are
byte-identical on/off and 669 lens-flare readbacks match through the final
checkpoint. Native-off oracle 1616/0, bench hash unchanged, retail image MATCH.
Work continues on representative live profiles, more kernels and worker overlap;
the steady 60 fps target remains open.

Round 25 geometry extensions accepted (finding 030): native 1CB0, B1638,
B17E0 and the B0040 lighting loops lower their live VU ms/frame; total VU cost
is 7.973 ms/frame versus 12.202 AOT-only. Paced race 46.3/46.3 fps, all live
state/image/readback gates match, all 64 original/representative captures
match GIF and persistent state. Native mask defaults to all six paths (63).
The separate GS worker and palette cache remain opt-in prototypes under test.

Round 25 GS worker accepted (finding 030): copied PATH1/2/3 GIF packets run
on a separate GS/D3D worker by default (DHD_GS_WORKER=0 restores one worker).
Paced race 47.0/47.1 versus 46.2/46.2 off. Race dumps, menu/race BMP bytes and
675 virtual-field-keyed Z readbacks match; queue wrap/ownership/fence and VIF
integration tests pass. The gain is small; the late race remains about 43 fps.
Palette-cache measurement and final frame breakdown are still in progress.

Round 25 complete for review (finding 030): clean normal build r25-clean reaches
46.9/47.0 paced race fps (menu 55.2/54.6), versus 42.9/43.0 AOT-only at round start.
Native geometry and the separate GS worker are default-on, with referee switches.
All four unchanged ref22 EE gates, eight menu/race BMPs and 671 Z readbacks match;
64 replay captures match GIF/state, native-off oracle 1616/0 and bench hash unchanged,
retail image MATCH and queue/VIF tests pass. No application crashes.
The palette cache (47.1/46.9) and narrower VU snapshot fence (46.7/46.9) gave no gain
and were removed. Main-thread frame timing is diagnostic-build-only: enabling it
exposed unused, uninitialized Huft union upper halves in func_00240CB8; normal
unprobed builds pass the strict reference without new exclusions.
Steady 60 fps remains unmet (late race about 43). Next: profile specific graphics
wait/readback dependencies and separately canonicalize Huft padding with a reviewed
reference refresh. All requested candidates were evaluated; stop for review.

Round 26 in progress (finding 031): fixed uninitialized Huffman entry bytes in
func_00240CB8 through a port-only patch (a93db02a). Fixed controls are identical
on/off at GO+0/+2/+12/+16; all eight frames and 663 Z readbacks also match the
previous control. Canonical padding replaces 1,057 words without adding
comparison exclusions. The clean full-source O1 build passes the refreshed
control. SSE4.1, VU-only PGO and small staging buffers pass their live gates;
AVX2, game O2, assembly O2 and ThinLTO have other EE differences and are not
accepted. Defaults remain unchanged while paired speed trials run.
Late-window VIF utilization is about 52%, both with exact Z reads and with
the diagnostic bypass; B0078/1040 costs 2.29-2.35 ms/frame. The 43 fps plateau
is not explained by a continuously saturated VIF worker alone.

Round 26 complete for review (finding 031): Huffman table entries now have
fully defined bytes (a93db02a); no comparison exclusions were added. Flip-model
presentation is default-on with DHD_FLIP_MODEL=0 and compatibility fallback.
Final paced race 47.8/47.8 versus 47.0/47.0 discard; late race remains about 44.
Final full runtime build passes four fixed-reference EE gates, eight BMPs and
665 exact Z readbacks. Native-off oracle 1616/0, bench hash unchanged, retail
image MATCH. No application crashes. SSE4.1 passes but has no paced race gain;
PGO and small staging add none. AVX2, game O2, assembly O2 and ThinLTO retain
unexplained EE differences and are rejected; compiler defaults stay SSE2/O1.
A separate, pre-flip SSE2 control had an intermittent menu texture/image
mismatch despite identical executable sections; the repeat passed. This is
unresolved and preserved, not excluded. Late VIF busy time is only about 52%,
so queued bytes alone do not establish a VU throughput limit. Next: GPU/worker
critical-path timestamps and the intermittent menu texture-upload producer.
Steady 60 fps remains unmet. Stop for review.

Rounds 27-28 accepted (findings 032-035): exact address shifts, early depth and
exclusive word stores are default-on (explicit-zero referee switches); four
port-only producer fixes define unwritten bytes; the ROV hardware coverage
bridge (conservative tier 3, compute fallback) is default-on with
DHD_GS_HARDWARE=0 as referee. Six EE moments, all images and 1,136 Z reads
match the r27-defined-native reference. Uncapped fields 6000-7000: about 67.5
fps. Findings 036-037 record high-refresh and native-geometry experiments.

Round 29 (finding 038): cleanup. The uncommitted build31-43 experiments
(zero-area eligibility, texture specialization, bin elision, tight scissor,
hardware sprites, coarse/fast depth, shade-before-ROV, native FMAC clamp,
render seams, private GS clone/snapshot and mesh decoders) are dropped; their
diffs are archived under build/port/archive-r29 (ignored). The round-27/28
accepted renderer, its gates and findings 032-037 are committed. Relinked on
the accepted r28 object basis, the cleaned tree passes gate_hw_renderer.py:
hardware off/on at six moments, the reference at four early moments, all
images and 1,128 exact Z reads. New direction: images need not be
bit-identical any more; only EE-visible game state must stay exact. Next:
fair benchmark (step 2), a native D3D11 renderer behind DHD_RENDER=native
(step 3), interpolated high-refresh presentation behind DHD_INTERP=1 (step 4).

Round 29 step 2 (finding 038): port/tools/bench_race.py times one fixed recorded race
(fields 6000-7000, DHD_FRAME_CLOCK=1, no diagnostics), runs candidates interleaved and keeps
the game window in front (a background window runs much slower; each run records its focused
share). On an idle PC, 3 runs each, all 100% focused: build31 fast preview 68.88 fields/s
(spread 0.53, worst 1% 26.14 ms); cleaned accepted tree 66.08 (spread 0.16, worst 1% 24.29 ms).
A first unfocused attempt on a busy PC (55-68 fields/s, spreads up to 11) is discarded.

Round 29 steps 3-4 (finding 038): the native renderer (`DHD_RENDER=native`,
port/gs/gpu_native.inc) and interpolated pictures (`DHD_INTERP=1`, `DHD_INTERP_HZ=N`) are
committed, off by default; the exact GS backend stays the default and the referee.
`port/tools/gate_native_renderer.py` (exact and native arms on one exe; six EE moments, every
readback record, the reference at four early moments) matches, with and without interpolation.
The last native/exact difference was a port ABI bug: two game units called `fabsf` without a
prototype, so clang read EAX and leaked the CRT's x87 result; port/patches.py fixes both.
Focused benchmark, fields 6000-7000, 3 interleaved runs: cleaned tree 66.09 fields/s unpaced,
native 75.42 (+14.1%, worst 1% 17.9 ms against 25.4). Paced at 59.94 Hz, interpolation shows
119.99 pictures/s at 120 Hz (present p99 8.89 ms, game steady at 59.94) and 143.69 at 144 Hz
(present p99 7.16 ms), but at 144 Hz the game thread stalled in two of three runs (221 ms, 3.3 s;
cause not found). 79% of race vertices pair. Launchers:
build/port/fast-preview/Play-native-120.cmd, Play-native-144.cmd, Play.ps1 -Native -Interp -Hz N.
Next: the 144 Hz stall, a by-eye check at 120/144 Hz, pairing more of the held 20%.

Round 30 (finding 043): sound gaps fixed by pacing against the device clock and WSOLA
time-stretch when the game runs slow (521 gaps -> 1 in a paced 3-run bench). Native renderer:
exact-frame hold 120 -> 20 fields (full exact fields 257 -> 57); ground flicker was the rider
shadow at half and varying strength (alpha stored as a/128 clamped; now a/255 targets with
dual-source blending); textures over GPU-drawn memory in another layout are rebuilt on the GPU
(`native_convert`) instead of decoded from the stale CPU copy. The loading-screen flicker no
longer reproduces in scripted captures; to be confirmed in play. Speed: the VU1 worker is the
heavy-scene bottleneck (VU 11.4 ms, GIF hand-off 2.2, VIF 4.4 a field); only 23% of VU1 runs
are independent of the previous one, so multi-core VU1 needs a per-object design (estimated,
not built). Signalling the graphics queues only to a sleeping side, plus a bulk VIF unpack
path: bench median 71.3 -> 84.0 fields/s (+18%; noisy PC). PC options: dhd.ini settings file
(environment overrides), window modes (windowed size, borderless, exclusive), the native
picture scaled into the window at 4:3 (DHD_ASPECT, DHD_PRESENT_FILTER), WinForms launcher
(port/launcher). Widescreen 16:9 (DHD_WIDESCREEN; Hor+ values mapped from Aced14's PCSX2
patch, applied from our side). Gate passes. Next: resolution scale, 16:9 HUD, FXAA, rumble/remap; then the
per-object VU1 split if heavy scenes still drop below 60.

Round 31 (finding 045): resolution scale for the native renderer. `DHD_SCALE` (dhd.ini
`scale`, launcher "Resolution (native)"): 1x-4x or `window` (smallest scale as tall as the
window); default 2x. Frame and Z buffers are drawn T times larger (vertices keep pixel centres,
scissor and viewport scaled, sprites snapped to whole pixels and 1:1 sprites fetched at the
block corner, from PCSX2's Align/Round Sprite ideas); the 64x64 shadow and blur targets stay
1x (PCSX2's native-scaling idea); texel fetches map between scales, and copies/imports across
scales go through a blit. Only pictures change: the exact backend still answers every game read
at 1x, the gate passes at 1x, 2x and 4x, and 1x pictures are bit-identical to before. Quick
bench (6000-7000, one run each): 1x 72.0, 2x 76.4, 4x 66.7 fields/s. Next: 16:9 HUD, FXAA,
rumble/remap; the per-object VU1 split if heavy scenes still drop below 60.

Round 32 (finding 046): VU1 thread speed. Measuring: per-thread CPU per field in every
bench (`bench threads:`, bench_race "CPU ms per field"), `DHD_PROFILE_THREAD` to sample any port
thread, `DHD_VU1_PROFILE_FIELDS` (per-program VU1 time over a field window, optional steps),
`DHD_VIF_STATS`. Changes: native kernel for the 07F0 mesh loop (3.5x on 07F0/1040), the kernel
signature check cached per code version, VIF UNPACK read straight from the stream for any
format with WL <= CL. Exact: `port/tools/vu1_seq_diff.py` replays 32,000 captured VU1 runs
(three race moments) with the kernels off and on, registers, data memory and GIF packets
identical; a 200,000-stream UNPACK fuzz in test_vif.c matches; the gate passes. Bench (3 runs,
idle PC, medians): 6000-7000 84.2 -> 96.8 fields/s (+15%, worst 1% 18.3 -> 16.4 ms), start
3000-3500 72.3 -> 81.4 (+13%), late heavy 8600-9100 81.1 -> 98.4 (+21%). The game thread still
waits half its time for the graphics pipeline (frame-end sceGsSyncPath, vsync, readbacks): the
EE and VU1 run mostly one after the other. Next: EE/VU overlap (vsync work as a ring marker),
the skinning loop 0468 (MSCAL 0020/0010) as a kernel, 0078's setup and tail.

Round 33 (finding 048): game logic overlapping the drawing, tried and reverted. Deferred
vsync (f80b4d61): no frame-end sceGsSyncPath wait, VBLANK and the picture queued as ring markers
run on the GS thread, a provisional CSR settled at the next drain, sceGsSyncV with an unfenced
FIELD read, at most one field in flight, `DHD_SYNC_STRICT=1` for the old waits and
`DHD_SYNC_STATS=1` for EE waits per call site. Exact (the gate passes, readbacks match, VU1 replay
identical) but not faster (3 runs vs round 32): 6000-7000 +4.0%, 3000-3500 -0.5%, 8600-9100
-5.4%. The EE's wait moves to vsync, and the VU thread, which bounds the frame, gets about 12%
slower while the EE runs beside it. Reverted (7fcb47b9); the launcher keeps the round 32 build.
Next lever: VU1 throughput (finding 046's list).

Round 34 (finding 051): CPU critical path, measured then cut. New: `DHD_TIMELINE` (per
field: EE waits for VU and GS, VU idle, GS idle; per-site EE waits), field-tagged profiler
samples (`profile.py --fields/--callers/--addresses`), `DHD_VU0_STATS`. A field is VU idle (EE
work before the first packet, 0.8-2.5 ms) plus VU busy (8-11 ms); the GS worker was close behind
in 6000-7000. Changes: VIF UNPACK specialised per format (the race's ~170k vectors a field, local
bench 1.27 -> 0.65 ms), GS host-to-local uploads whole pixels per format, VU0 microprograms as
translated blocks (`DHD_VU0_AOT=1`, exact, off: no wall gain, the VU0 work overlaps the VU).
Exact: test_vif fuzz, test_gs upload fuzz, vu0_aot_diff over 104 captures x 9, each with a
failing negative control; the gate matches on 62c2d7d9, also with DHD_VU0_AOT=1. Bench (3
interleaved runs, medians, fields/s): 3000-3500 79.2 -> 81.5, 6000-7000 95.3 -> 110.8 (p99 15.7
-> 13.3 ms), 8600-9100 95.0 -> 100.5, whole 3000-9100 104.7 -> 110.9 (worst 1% 17.4 -> 16.0
ms). Then a native kernel for the skinning loop 0468-0570 (MSCAL 0010/0020, replay 57 -> 19 us a
run; vu1_seq_diff over three captures incl. a new crowded-start one, 36,000 runs, 0 differing;
gate MATCH on 20a4fc6a): against 62c2d7d9, 3 runs, 3000-3500 83.1 -> 88.0, 6000-7000 112.9 ->
114.5, 8600-9100 100.9 -> 105.1, whole 112.0 -> 116.3 fields/s (worst 1% 16.0 -> 15.3 ms).
Then the VU1 FMAC clamp (b36aea01): under the VU MXCSR (toward zero, FTZ, DAZ) an operand only
needs the exponent-255 clamp and the result clamp is an identity; an empty asm keeps the compiler
from folding constant operands in AOT blocks (x * vf0.w), which had let a denormal through.
Exact: test_vu1 fuzz against clamp-op-clamp (negative control fails), every run of the three
captures identical to the previous build (registers, data, GIF), test_vu1_pcsx2 --aot 0
failures, gate MATCH (1,163 readbacks). Against 20a4fc6a, 3 runs: 3000-3500 88.3 -> 102.3,
6000-7000 113.9 -> 132.0, 8600-9100 104.1 -> 120.1, whole 115.6 -> 135.9 fields/s (worst 1%
15.5 -> 13.6 ms). Then (2ab40cf5) the U and O MAC bits, always 0 after that clamp, are no longer
computed: exact (captures identical, test_vu1, pcsx2 oracle, gate MATCH 1,156 readbacks); 3000-3500
103.8 -> 111.6, 6000-7000 132.1 -> 141.3, 8600-9100 121.6 -> 136.1, whole 136.2 -> 149.8. The
round chained: whole race 104.7 -> 149.8 fields/s (+43%), worst 1% 17.4 -> 12.4 ms; at the start
the EE (5.6 ms busy) is close behind the VU (6.5 ms). The kernels take ~90% of their runs
(setup/tail not worth it). VU0 AOT benched again on c4: 149.6 vs 148.3 fields/s, inside the
spread; stays opt-in. Next: the VU thread's remaining work (vif_feed 13%, the kernels' clipping
path 4-7%, run dispatch and flag retirement), the AOT programs, now about half the VU1 time
(2DC8/1040 late, overlay 1's 2E48, 06F0, 0E10, 1430), then what the EE does before its first
packet.

Round 35 (finding 050): the game asks for the disc image itself. On a start without a
usable `iso` in dhd.ini (none, file gone, or its SYSTEM.CNF doesn't name SCUS_971.77), the first
dhd.exe (before the relaunch) shows a message and a file-open dialog. It checks the choice
(rejects other files with the reason, Retry/Cancel; Cancel exits 0) and writes it to dhd.ini,
keeping every other line. A dhd.ini made this way also gets the player defaults
`renderer = native` and `skip_movies = 1`. DHD_ISO in the environment still overrides and is
never questioned. dhd.ini values may be quoted. The gate passes on clean build bf0072f7 (all EE
moments, 1,151 readbacks).
The ImGui settings overlay was dropped before any code: settings and the
coming online lobbies should live in the game's own menus. Finding 050 maps the front end:
- the switch on `D_0029DED8` in func_001EB4D0;
- the hand-written screens, text helpers, input latch and sounds;
- the SharedMenu popups (AUDIO / OTHER, the pause menu);
- the font built into the executable.

It shows that a PC SETTINGS page in OPTIONS, new online screens and an in-race entry can be added
from port/ (patches.py and host functions), with estimates per piece and a game-styled overlay as
the fallback. Launcher.ps1 stays for now. Next: A+B+C of finding 050 (OPTIONS item, SharedMenu
page, live display changes).

Round 36 (finding 050): OPTIONS has a tenth row, PC SETTINGS, which opens a page drawn
and driven by the game's own popup menu code (the AUDIO / OTHER page's): the game's font, panel,
cursor, legend and sounds; left/right change a value, Cross keeps, Triangle puts back what the
page opened with. Rows: Resolution, Display, Window Size, Picture, Scaling, Widescreen, Skip
Movies, Vibration (no renderer choice). Each change is saved to dhd.ini at once (other lines
kept). Display, window size, picture shape, scaling and vibration apply at once; resolution,
widescreen and skip movies at the next start, marked "*". Variables set in the environment show
but don't change. Hooks: port/patches.py (func_001F0160) and port/hle/pc_settings.c; no src/
change. Also this round:
- dhd.exe links as a Windows program: a double-click opens no console; the log goes to the
  tools' pipes as before, else the parent console, else dhd.log beside dhd.exe;
- the DualShock 2 motors drive an XInput pad (dhd.ini `vibration`);
- Launcher.ps1 has no renderer row (always native; DHD_RENDER=exact stays for developers).
Tested on clean build 449ce49b (screenshots in build/port/r36): the page opens, changes save,
and after a restart scale 3, sharp scaling, 1280x960 and 16:9 are in effect and shown; Triangle
reverted a change. The native renderer gate passes on 449ce49b (all EE moments, 1,146
readbacks). Found, not fixed: OPTIONS > CONTROLS crashes the port (already on round 35's
build; a PS2 string address used as a pointer, finding 050).

Round 38 (finding 052): frame pacing and input latency. Just-in-time frames: the picture
is handed on at once and the presenter holds it to a fixed 59.94 Hz grid; after the VBLANK the EE
waits until the next due time minus its recent worst frame (+1.5 ms) and then re-reads the pad
(`pad_refresh`), so the pad is younger when the picture shows; a late picture moves the grid
instead of being followed by a short one. Waits use a high-resolution waitable timer then a 1 ms
QPC spin. Present: flip discard, frame-latency waitable with latency 1, sync interval 0 with
tearing allowed (G-SYNC friendly). `DHD_LATENCY=0` keeps the old order, `DHD_TEARING=0` drops the
tearing flag, `DHD_PACING_STATS=1` prints the pad-to-present age. Measured with PresentMon over
20 s of a race, borderless: present sd 0.12–0.14 ms (min 15.6, max 17.7, long/short pairs) before,
0.03–0.08 ms after (min 16.4, no short frames); pad-to-present 17.1 ms before, 13–16.6 ms after.
The native renderer gate: arms and readbacks match (EE state unchanged); the canonical reference
mismatch is the same 65 words as round 37's gates (finding 050). Test build:
build/port/r38/play (local).

Round 38, front end (finding 050): the memory card works. port/hle/mc.c answers the
game's libmc at the mcserv RPC (0x80000400) from `saves\card1` beside dhd.exe (dhd.ini
`memory_card = 0` turns it off; the race/gate tools do). Tested on clean build 1499a7c4: save,
overwrite, the load at boot, and a round trip (DIFFICULTY HARDCORE saved, restart, HARDCORE).
Movies stay skipped: the first crash (func_0023C1D0's five arguments) is fixed in patches.py,
but playback needs an IPU / MPEG-2 decoder and the libmpeg path checked (finding 050 lists
what). The native renderer gate's reference is now `build/port/compare/r38-reviewed` (from
1499a7c4, after the 65 words were each traced to an intended change); the gate passes on HEAD.

Round 39 (finding 053): the first public release, "Downhill Revived".
- Reverb: the ps2sdk preset table (AFL-2.0) is out of the source. The mixer reads the presets from
  the player's disc, Sony's libsd module `MOD/LIBSD.IRX`, found by the table's shape. The values
  equal the old table; the race start writes the same registers.
- Build: the plain `build_clean.py` is the release build. From a fresh clone with only the
  player's split executable it is as fast as the old `--recomp-basis` builds: VU1 AOT and the
  native kernels come from `DHD_BUILD_VU_AOT=1`, not from the basis. Bench 127.2 vs 123.5
  fields/s, same revision.
- Gate: the plain build differs from r38-reviewed only in 54 words of dead main-thread stack.
  The gate's reference is now `build/port/compare/r39-plain`, from the plain build; the gate
  passes on b7904561. `link.py` no longer embeds the builder's path.
- Release zip: `port/tools/package_release.py` makes `DownhillRevived-<version>-win.zip` with
  dhd.exe, README.txt, LICENSE.txt and THIRD-PARTY-NOTICES.txt. It refuses an exe with an
  absolute path or a server host.
  `package_online.py` is retired.
- 0.1.0 unzipped into a new folder: disc prompt, menus, race.
- Game data out of dhd.exe (finding 053, section 4): the data image (.vutext-.sdata, 238 KB) is
  read at start from SCUS_971.77 on the player's disc, checked by SHA-1
  (`port/hle/game_data.c`); dhd.exe keeps only the 4,419 words the port changes (`fixups.s`,
  checked against the ELF at link time). The VU AOT blocks and native kernels copy their
  microcode from it (`ps2_vu_aot_load`). A changed or missing file is named in the disc prompt.
  `port/tools/scan_retail.py`: no 64-byte run of the ELF in dhd.exe except 14 compiled from
  `src/` (string literals, a CRC table); `package_release.py` enforces it. The zip ships no
  dhd.ini; the game writes it on the first start. Gate passes against r39-plain unchanged.
- Contributors record their own gate reference: `gate_native_renderer.py --record`.
- Online out of the box (finding 053, section 5): the official server
  `wss://dhd-online.downhill-revived.workers.dev` is the built-in default when dhd.ini's
  `server =` is empty and `DHD_NET_SERVER` unset; either still overrides it. `package_release.py`
  allows that host only. From the 0.1.0 zip: a public race created, listed by a second instance,
  joined with its code by a third.

Round 40, online track (finding 054): 0.2.0's start gates, version gate and updater.
- Start gates: online, every rider starts on the start point of its global id, the same on every
  PC. Cause: `func_0011AEC0` swaps each human with a random rider, a draw that differs per PC
  while each PC's human is its own slot 0. Puppets stood on AI riders' gates and jumped at GO. The
  swap is skipped online (`port/patches.py`, `hle_puppet_keep_grid`); the draw is kept.
- One version: `port/include/version.h` (`DHD_VERSION "0.2.0"`), used by the hello
  (`clientVersion`), the online version string, the updater and `package_release.py`.
- Server: `MIN_VERSION` in `online/server/wrangler.jsonc` (0.1.0 for now); older hellos get
  `client_outdated` (4009), "A new version is out. Update to play online." 0.1.0 shows that in
  its online footer; 0.2.0 adds Update Now. 34 server tests. Deployed to the official server
  with `MIN_VERSION` 0.1.0 (version d8f6e3c1).
- Updater (`port/hle/update.c`): the first dhd.exe asks GitHub's latest release (or `update_url`)
  at start, at most 6 s. If newer, it asks (`auto_update = ask | on | off`), downloads,
  checks the sha256, swaps dhd.exe (old one as dhd.exe.old, deleted next start) and restarts.
  It never touches dhd.ini or saves. `package_release.py` writes `latest.json` beside the zip.
- Release steps: finding 054, section 6 (publish zip + latest.json as v0.2.0, then
  `MIN_VERSION` 0.2.0 and deploy).

Round 40, movies (finding 055): the startup movies and OPTIONS > VIDEO CLIPS play, with picture
and sound in sync.
- The PS2 player can't run on the PC: it needs libmpeg on the IPU, a toIPU DMA chain the game
  builds itself, PATH3 uploads and 989snd's external stream.
- Its entry points (func_00223BB0 start, func_00223C60 each frame, with keep 0 = skip) are patched
  to `port/hle/movie.c`:
  - a thread reads MOV/*.PSS from the disc image;
  - libmpeg2 0.5.1 decodes the video (GPL-2+, `port/third_party/libmpeg2`, credited in
    THIRD-PARTY-NOTICES);
  - the PCM goes into the mixer;
  - picture and sound follow the mixer's sample count;
  - the presenter shows the picture centred at 4:3 (exact and native paths);
  - the game gets the PS2 player's flags.
- VIDEO CLIPS crashed on raw PS2 string addresses in its clip table; host_movie_start maps them.
- A movie never ends in the frame it started (its callers would wait forever).
- `skip_movies` (PC SETTINGS > Skip Movies, live) is the player's choice again, default 0. The
  race, gate and bench tools still set DHD_SKIP_MOVIES=1; DHD_MOVIES is gone.
- Tested on clean builds of a5c87e5c and 36d79fb1 (screenshots and WAVs in
  build/port/r40movies/final):
  - the four startup movies at real time;
  - CREDITS and INTRO from VIDEO CLIPS, each skipped with Cross back to the list.
- scan_retail and package_release pass. libmpeg2's default intra matrix is stored as uint16_t,
  so the exe holds no copy of the ELF's copy of that MPEG-2 table.
- Native renderer gate on 36d79fb1 (the gate tools force DHD_SKIP_MOVIES=1):
  - the arms match each other at every moment, with 1,153 readbacks;
  - against r39-plain only 2 words differ, 0x004F4324 (0x10 -> 0x24) and 0x004F4454 (0 -> 0x10);
  - the widescreen and menu work's own gate run (build/port/r40ui/gate1) shows the same two
    words, so they come from those changes, not from the movies;
  - so the gate fails on main until those two words are traced and a reference recorded.

Round 40, widescreen HUD, defaults, EXIT GAME (finding 056):
- Widescreen (`port/hle/widescreen.c`) is Hor+ for any shape. `widescreen = 1` takes the
  monitor's shape; `W:H` takes that shape. The process is DPI aware.
- The game's 2D code maps X (`port/patches_widescreen.py`):
  - menus, popups, the pause menu and results stay 4:3 in the middle, with the 3D background
    widened around them;
  - the race HUD's text and its 3D pieces (speedometer, track bar, place) are anchored to their
    side of the screen, in shape.
- Full-screen 2D pictures are shown 4:3 by the native renderer: the memory card warning, the
  publisher cards, and the startup and race loading cards. The test is VU1 drawing nothing, or
  a loading state (title before its 3D, race loading) with few VU1 primitives, 8 fields in a row.
  It is never applied during a race.
- `DHD_ASPECT=wide` is widescreen's shape. `DHD_SCREENSHOT_WINDOW=WxH` writes screenshots as a
  window shows them.
- Player defaults, where dhd.ini doesn't say (only with a dhd.ini; the first start writes one,
  and its template lists them): native renderer, scale 2, borderless, widescreen 1, DHD_ASPECT
  wide, skip_movies 0. Explicit lines and DHD_* variables win. The race, gate and bench tools set
  `DHD_SETTINGS=none`, so they keep the old defaults.
- EXIT GAME: a fifth main-menu row (`port/hle/exit_menu.c`, `port/patches_exit.py`) asks "Quit
  Downhill Revived?" (No / Yes) in the game's popup. Yes closes the window: exit code 0, both
  dhd.exe end.
- Gate (30222647): the arms match each other. Against r39-plain the same 2 words differ,
  0x4F4324 and 0x4F4454: text packet leftovers in `D_004F3AD0` from the fifth row's "EXIT GAME"
  glyphs (finding 056, Gate). The reference is to be re-recorded once at integration.
- `link.py` links gdi32 (the updater's dialog font); the clean build of ebde3667 had failed to
  link.

Round 40, texture packs (finding 057): the native renderer loads PCSX2 texture
replacement packs unchanged (`port/gs/texpack.c`; PCSX2's names reimplemented, XXH3 from
xxHash 0.8.3 vendored as `port/gs/xxhash.h`). Packs are the players' own; none is in the repo.
- On the user's pack (6,181 PNGs, 4x upscales): every texture the renderer decodes in the scripted
  run is in it, 161 in the menus and 110 in the race (271 of 271).
- PNGs load on two loader threads (WIC) with mip chains; the original texture draws until the
  replacement is ready. A budget (`texture_cache_mb`, default half the card's memory, 512-2048 MB)
  releases the least recently drawn. The race peaked at 738 MB of graphics memory; process memory
  grew by 10 MB.
- dhd.ini `texture_pack =`, `textures = on|off` (PC SETTINGS > Texture Pack, at the next start),
  `texture_cache_mb`; a `textures\SCUS-97177\replacements` folder beside dhd.exe is found alone.
- No pack or `textures = off`: pictures byte-identical to 076de351; the gate passes on the build
  from that base (also with the pack on: replacements change no game state).
- Speed, on a busy PC (sanity checks, not benchmarks): paced, pack on, menus to race start
  59.56 fields/s, worst 1% 30 ms, max 69 ms. The hitches fall on menu screen changes where
  40-55 new textures appear at once. Not yet compared with the pack off on an idle PC. Unpaced
  race with all replacements loaded: 116 vs 132 fields/s off.
- Left: a pack-off/on paced comparison on an idle PC (are the screen-change hitches the pack's?);
  texture dumping; region names and lod.x > 0 draws (finding 057).

Round 41, release 0.2.0 (finding 053, section 6): built with `build_clean.py` on 22e70d54
(dhd.exe 2253ddf9...). Gate reference re-recorded as `build/port/compare/r40-plain` from that
build (it differs from r39-plain only in the 2 EXIT GAME words) and made the default; the gate
passes on it. `DownhillRevived-0.2.0-win.zip` (sha256 a14684e0...6a0c) and `latest.json` made;
retail scan clean, icon resource present. The unzipped release: defaults written on the first
start, movies play, EXIT GAME exits with 0, a race starts. Updater dry run (a 0.1.9-marked
copy, a local latest.json): the real 0.2.0 zip downloaded, checked, installed and started,
dhd.ini and saves untouched.

Round 42, release 0.2.1 (finding 053, section 7): window and message box titles say "Downhill
Revived"; version 0.2.1. Clean build c6a4cc4f, gate passes against r40-plain, zip (sha256
c6840bba...d753) and latest.json made. The 0.2.0 release updated itself to 0.2.1 from a local
latest.json (Update now).

Round 43, release 0.2.2 (finding 053, section 8): version 0.2.2 (c4e2c707) with the online
after-race fix, the sharp font and keyboard / SDL3 pads. Clean build (SDL3 built by sdl3.py in
the fresh worktree), gate passes against r40-plain, zip (sha256 4d400a85...178c) and
latest.json made, retail scan clean. Smoke: menus, PC SETTINGS with the Font row, a race. The
0.2.0 release updated itself to 0.2.2. Online: 0.2.0 and 0.2.2 can't share a room (the lobby
version carries the exe hash), and nothing on the wire or in the simulation changed.

Round 42, sharp font (finding 058): the game's text is readable at 2x-4x.
- `port/gs/font_sharp.c` takes the text engine's font texture (the TEX0 in `D_004F35B0`'s
  commands), as the game uploaded it from the disc. In memory it makes a magnified copy:
  cubic B-spline magnification, then the edges cut as a distance field's are when drawn.
- The native renderer draws the copy as a texture pack replacement, with mipmaps. A pack's own
  font wins.
- The menus' 512x512 font lives in a depth buffer's top bytes. It is now drawn from the CPU copy
  rather than rebuilt on the GPU.
- Nothing is written to disk. No third-party code (xBRZ was considered, not used).
- dhd.ini `font = sharp|original` (default sharp); PC SETTINGS > Font, at the next start.
- With `font = original`, the menu screenshots are byte-identical to before, apart from the new
  row.
- Before/after crops: `build/port/r41font/shots/`.
- Gate on the clean build of 29f9d938: match against r40-plain, with the sharp font on (the
  default). scan_retail.py and package_release.py pass.

Round 42, keyboard and generic controllers (finding 059): a whole race plays on the keyboard,
and non-XInput pads work through SDL3.
- Keyboard (`port/hle/keyboard.c`): arrows are the d-pad in menus and, while the game's riding
  controls are on (`D_0029E290`), tilt the left stick, ramped to full in 100 ms (linear in the
  game's own steering, after its dead zone and squaring); Space/X Cross, S/Backspace Triangle,
  Esc back or pause, Enter START, Tab SELECT, Z/C Square/Circle, A/D L1/R1, Q/E L2/R2, W/F R3/L3,
  letters by place (AZERTY/QWERTZ). Read only while the game's window is in front and no
  Alt/Ctrl/Win is held; merged with player 1's controller.
- Controllers (`port/hle/gamepad.c`): SDL 3.4.18, static, built by `port/tools/sdl3.py` from the
  pinned, hash-checked source (CMake; `docs/BUILDING.md`). It replaces the direct XInput and Sony
  HID paths, which remain the fallback (`DHD_SDL=0`); vibration on every pad with motors.
- Checked: test_keyboard; SDL enumerates this PC's DualSense; a DHD_KEYS race (fake keys) gives
  the rider steering 22, 43, 65, 85, 107, 127 over 100 ms; the gate passes. Real key presses,
  generic/Switch/8BitDo pads and actual vibration need someone at the PC.

Round 44, progressive field jitter (finding 060): measured on twelve consecutive 2x widescreen
main-menu fields. OFY alternates 0x7900/0x7908 with fixed raw 2D Y; DISPLAY/DBY stay constant.
The logo and 3D background both bob by two output pixels. Native coordinate conversion removes
the half-line OFY component before sprite rounding on screen-sized INT+FFMD targets, leaving
exact rendering/readbacks untouched. Main-menu and OPTIONS captures now have zero alternating
displacement; every legend crop is byte-identical. Eleven consecutive race fields also remove
the HUD/world's two-pixel bob, preserving the original integer phase without filtering.
Clean build 00c53232 (all 3,797 units compile) passes the unchanged default r40-plain gate:
all EE moments match, 1,168 readback records / Z reads match through virtual GS field 9051.
Proof and lossless diffs: build/port/r44jitter/. Exact rendering stays unchanged; no benchmarks.

Round 45, pickup flare (finding 061): both native and exact sustained an oversized pickup glow.
The port data converter relocated a packed duration word (0x00780000 -> 0x08780000), making
particle template 16's duration 2168 instead of 120. Its intensity envelope stayed bright;
this was game-visible particle data, not native GS blending or occlusion. The converter now
preserves the timing pair at +0x18 of all 77 scale records (25 image words corrected).
- Scripted retail PCSX2 now actually hits the same pickup: it flashes for two fields, then
  shows fragments. The clean fixed build preserves that brief flash and removes the prolonged
  glow in native and exact. Native's real-sun +240 capture is pixel-identical; non-pickup +960
  changes only 44 pixels. No FPS benchmarks. Proof: build/port/r45pickup/.
- Clean build 830c9150: all 3,797 units compile; exe build/port/clean/830c9150/dhd.exe.
  Unchanged default r40-plain gate: **FAIL only for 29 intended words** (25 timing pairs and
  four derived slopes), identical at every early checkpoint; all 29 match independent retail
  RAM. The exact/native arms MATCH at all six EE moments through GO+75 seconds, and all 1,128
  Z readbacks MATCH through GS field 9049. Reference and checker exclusions remain unchanged;
  finding 061 enumerates the words. No src/ changes.

Round 46, systematic data-fixup audit (finding 062): audited all 4,419 historical
entries (4,394 after round 45). Confirmed 44 additional corruptions: 24 copied
SDK instructions, 13 TLB fields, six text words, one inflate extra-bit word.
Numeric schemas also remove two harmless identity entries; 4,348 fixups remain.
The five synthetic schema/pointer regressions pass. Clean build f0308534:
all 3,797 units compile; executable build/port/clean/f0308534/dhd.exe.
Against r40-plain, exactly 73 intended words differ at every early checkpoint
in both arms (44 new corrections + round 45's 29); every word matches independent
retail RAM. Recorded r46-plain from this clean build and made it the default.
The final default gate MATCHES all frozen-reference and paired EE checks through
GO+75 seconds, with 1,096 Z readbacks matching through GS field 9017. No check
exclusions changed.

Round 47, release 0.2.3 prepared (finding 053 section 9): version-only commit
bde3cf92 includes findings 060-062; its clean build compiles all 3,797 units.
Default r46-plain gate MATCH: zero unexcluded differences in all 14 word
comparisons, 1,121 matching Z readbacks through GS field 9042. Release zip and
latest.json are in build/release; clean retail scan and icon verified. Extracted
zip reaches Alpine; real 0.2.2 updater installs/restarts 0.2.3 with ini/save
unchanged. Temporary test folders deleted.

Round 48, visual sweep **partial / open** (finding 063): added `port/tools/visual_sweep.py`,
a sequential retail/native/exact capture runner with settled-menu checks, five-field race
sequences, preserved retries, aspect-correct contact sheets, offline coverage reporting and
an instance guard. Ten front-end screens have retail/native captures; five also have exact
captures. No clear new player-visible rendering bug was established and no game/renderer fix
was made. Shop fade and lost cursor presses exposed capture-route issues; corrected routes
still need live verification. Proof: build/port/r48sweep/. - Clean build 5760505b: all 3,797
units compile, link succeeds; executable build/port/clean/5760505b/dhd.exe. Later changes
are tools/docs only. - Default r46-plain gate **INCOMPLETE**: stopped the agent's own
exact-arm process when a player-owned game appeared; the player's process was untouched. The
interrupted attempt also requested native widescreen in error; the retry helper uses normal
gate state plus scale 2 and remains blocked by that game. No comparison exclusions or
reference recording changed; no MATCH claimed. - Remaining OPTIONS subpages, track
selection/all tracks and effects, pause, results/loading/replay and split screen are
explicitly open in finding 063. Resume on an idle PC with `--retry-blocked`, verify
corrected routes visually, and run the unchanged default gate on the clean executable.

Round 48 continuation (2026-10-18, idle PC; **in progress**): reviewed all selected
retail/native images for AUDIO/OTHER, VIDEO CLIPS, load/save/replay-load dialogs, rider
statistics, career selection and track confirmation, plus ordinary pause. No clear
player-visible renderer defect established. The built-in unlock input now works on both arms
(mask 0x035FFFFF); unlocked pause, NEW LEVEL list, Japan and FOG EDITOR are reviewed as
exploratory setup evidence. A targeted read using PCSX2's EEmem debugger export found Always
Stoked survives a level reload; the next routes toggle it off through input and require
three zero ordinary flag words before arming race-clock capture. Default comparisons remain
unchanged. - Clean runtime **044170e8**: 3,797 units compile with zero errors, link
succeeds; `build/port/clean/044170e8/dhd.exe`. Plain default **r46-plain gate MATCH**:
eight reference and six paired EE checks, 1,124 readbacks/Z reads through GS field 9045;
native used, no widescreen on either arm. No exclusions or reference changed. Later commits
are tools/docs; concurrent changes excluded. - Full-pedal retail: no finish by ten game
minutes (checkpoint 19/28 at clock 27797). Native finishes at tick 23551, 28/28 checkpoints.
Results pictures now have native evidence: results and live-input replay pictures reviewed;
retail finish/replay parity remains open. The initial periodic request was ignored in race
mode. A dedicated opt-in periodic capture branch and finished-byte detector are committed in
044170e8; clean build/gate pass, live verification is next. - Two-player widescreen race:
all 60 retail/native screenshots reviewed through GO+3000; both riders move, both
HUDs/viewports render, no clear new defect. The separate close-rider combat probe now has
all 80 screenshots reviewed; both arms show PUNCH! 50 PTS and the contact effect. Every
readable saved frame in the initial 18 front-end cases is reviewed (incomplete trailing
files excluded). Dense boot probes establish a low-severity native-only loading-card
defect: exact retains the full card while native alternates black/partial output. Earlier GS
history is queued to isolate its cause; no speculative fix. Native finish/results/replay:
all 375 screenshots reviewed. Remaining tracks, effect parity and menu branches remain open
until captured and reviewed. Live jobs/evidence are local under build/port/r48sweep/. See
finding 063 for intermediate build IDs, successful navigation, failed-route evidence and
current limitations.

Online forces 60; replay/ghost paths are disabled above 60. Clock/helper checks pass at
60/61/75/90/100/120/144. Clean physics build `85229f7d` passes the default r46-plain gate (14 EE
MATCH, 1,116 readback/Z records). Read-only diagnostic build `6a063c72` also passes (14 / 1,118) and
exactly reproduces the 31-second rider traces. Final code `734a8cc2` additionally preserves Verlet
prev.w; its final default r46-plain gate passes 14 EE comparisons and 1,110 readback/Z records
through field 9031. Forced crash lasts 30 fields at both rates; pickup emitter visibility is 113-116
/ 120 fields in the short pair. Full-pedal unsteered 60 finishes at 23551 fields; 120/144 wedge and
do not finish by 30000. Separate periodic-bail fixture finishes at 24087 / 24683 fields (60 / 144,
+2.474%). Early 144 speed ratio is 1.01323, maximum path separation 7.78125 world units; AI finish
variation and particle episode screening failures remain unresolved. Final busy-host unpaced 144
throughput was about 104.48 real updates/s; 144 real updates/draws per game second is verified with
the virtual clock. See finding 067 for the full fix inventory, failed acceptance evidence, private
artifact paths and next investigations. Source/matching checks unchanged; no equivalence claim, no
merge/push/publication.

Round 52 continuation (finding 067, existing offline-unlock branch; acceptance still failed):
contact traces identify per-update force clearing and a stable grounded wall equilibrium with no
retail recovery timer. Preserving the force-clear window per legacy field brings dense equal-time
five-second player speed ratios to 0.997961 / 0.999356 (120 / 144). The clean position-progress
recovery refinement finishes at 432.733 / 402.400 seconds versus 392.517 at 60 (+10.246% / +2.518%).
It removes indefinite grounded stalls; 120 still fails acceptance. This is an explicit human
recovery-policy addition, not contact-equivalent physics. AI choice retries are now field-gated;
per-rider route/finish equivalence remains unestablished. The isolated canonical 128-particle burst
passes: initial particle records are identical at all three rates, and every lifetime cohort retires
within one selected update of 60 (1.966667 / 1.975000 / 1.972222 seconds for the last particle).
Exact rational update-count comparisons, not rounded timestamps, enforce this bound. Clean 07ea5e18
passes the unchanged default r46-plain gate (14 EE MATCH, 1,159 readback/Z records through field
9080). All nine AI finish at all rates, but routes/speeds remain divergent. Forced crash is 30
fields. A later partial-grip conversion passed an isolated solver test but regressed 144 to 433.817
seconds; it was rejected. Retained runtime is exactly 07ea5e18. Fresh retail SN build matches all
eight checked sections. Sound unit checks pass; no listening/PCM equivalence or uncontended
paced-144 result is claimed. Final clean 167fa3e2 passes the default gate again (14 EE MATCH, 1,106
readback/Z records through field 9027). It differs from full-race-tested 07ea5e18 only in two COFF
timestamp bytes; every loaded section is byte-identical. See finding 067 for per-fix evidence and
remaining acceptance work.

Round 52 chaos-aware measurement step complete (**equivalence acceptance FAILED**).
Finding 067 records the full method, all-slot drift, censored finish distributions,
provenance and limitations. Twelve common-60-prefix three-second windows cover
three seeds and pedal/steering-prefix scripts; ten starts are supported, while
any already-active ragdoll excludes its whole window. Snapshot comparison keeps
raw data and canonicalizes only proven unused Euler w lanes. Observations are
correlated and mixed signs do not prove zero bias.
- Corrected force-window interval ownership (exact end boundaries belonged to
  the preceding interval). Independent rational regression fails before/passes
  after at 61/75/90/100/120/144. GO quarter-second AI drift changes
  -1.5955% -> -0.6913% at 120; -5.3882% -> -4.5516% at 144: still biased.
- Three-seed full-pedal player finishes: 60 [392.517, DNF>500, DNF>500],
  120 [399.150, 411.617, 389.000], 144 [426.650, 380.267, 410.000] seconds.
  Censoring differs, so distributions are NOT established equivalent.
- Consumed-input steering first reproduces every 60 riding row (2,296,200
  values). Its valid full-route result is 318.900 s at 60, DNF>500 at 120/144.
  Earlier planned-timestamp exports remain rejected as full-route fixtures.
- Pair relaxation remains removed; partial grip and passing floating-RNG
  experiments are opt-in, OFF by default, not established bias fixes.
  The grounded human watchdog remains explicit policy, provably inert at 60.
- Final clean bba533d2 passes default r46-plain: 14 EE MATCH, 1,127 readback/Z
  records through field 9048. Disabled-trial controls equal the measured default
  policy in 379,800 values. Final particle probe passes; injected crash lasts
  30 fields at all rates, but a later natural crash differs at 144. Sound HLE
  tests pass (no PCM/listening claim). Retail check: all eight sections MATCH.
  Morning launchers/README remain on gated167fa3e2: mixed results are not better.
  altered gate comparisons/references, merge, push or publication. All evidence
  stays private. Next: isolate the residual early AI/contact bias with the new
  matched-prefix tool; retime active-ragdoll history before using such starts.

Round 49, settings persistence (finding 064; **done**): fresh release 0.2.3, first-start
disc chooser and generated ini, no DHD tool environment. SendInput changes and EXIT
GAME/restart confirm original audio/stereo, four control schemes and difficulty reset; all
ten PC SETTINGS rows persist. Preferences now have an independent versioned ini record,
accepted-page hooks, boot/load restore and validation, without invoking a career save.
Enabling vibration also exposed and fixed an unimplemented SDK direct actuator DMA path.
Focused C round-trip/career-isolation/malformed-input/tool-bypass tests pass, including a
32-bit run. Clean build feae225f compiles all 3,797 units and links;
`build/port/clean/feae225f/dhd.exe`. Default r46-plain gate MATCH: all 14 EE comparisons and
1,143 matching Z reads through field 9,064, unchanged checker and exclusions. Full settings
/ EXIT GAME / restart PASS: all original options, all four live vibration flags, and all ten
actual PC page row strings retained; both quits exit 0. First start without a career save
PASS; preference changes create no career files. Real Super Career event/save/load PASS:
rider/track records load byte-identically. A later music edit leaves the career file SHA
unchanged and survives both boot loading and manual LOAD GAME DATA despite the older value
on the card. Manual saves do include music (correcting finding 050); the bug was the missing
automatic options write.

## Online play (plan: docs/ONLINE-PLAN.md)
2026-10-16 step 1, server (finding 039): `online/server/` is the Cloudflare Workers + Durable
Objects server (lobbies, join codes, public list, lockstep input relay, desync detection);
protocol v1 is `docs/online-protocol.md`, which the client's `port/net/` codes against. 25 tests
pass; a local 4-player 600-field lockstep simulation relays every input (median 2.5 ms one way).
Not deployed yet: `online/server/README.md` has the steps (`wrangler login`, `wrangler deploy`).
Next: the client network module, then a first internet race.

Online research (finding 040, code reading only, nothing run): a 4-player online race can use
the game's own 4-player split screen. The port must answer the multitap RPCs
(`sceMtapGetConnection(0)` = 1) and open and refresh pads in port 0 slots 1-3; player i reads
pad 2i. 3-4 players race with no AI, other level files and no trackside actors. Each PC must
run all four views EE-side; the local full-screen view is a renderer crop of its quarter. The
finding lists the setup block `D_004B25A0`, the determinism hazards, an integration recipe and
experiments E1-E6 for the game-side worker.

2026-10-16 step 2, client (finding 042): `port/net/` is the game-side network module (lockstep
core, lobby client, WinHTTP WebSocket, XXH64 state hashes, `net.h` API) for protocol v1. Unit,
loopback (4 clients in one process) and end-to-end tests pass, the last both on `wrangler dev`
and on the deployed server: 4 processes, 600 fields, same state everywhere, drop to AI on the
same field, desync reported at 419. Not wired into the game yet. Next: integration in `pad.c`
(4 multitap pads) and `host.c`, with a field clock that slips while waiting for inputs.

2026-10-16 step 3, lockstep in the game (finding 041): 4 multitap pads, lockstep per field, state
hashes, launcher and friends' zip. Live it ran 7200 fields in step, but the race state parted at
field 1020 (cause not found). Dropped the same day in favour of state sync.

2026-10-16 step 3b, state sync: the puppet rider (finding 044). Each PC runs its own one-player
race; the other players, and on guests the host's AI riders, are puppets: AI slots with input
and physics off, written from snapshots sent every 3 ticks through the relay (STATE frames),
shown 100 ms behind with interpolation. Crashes start from the snapshot. Hits become EVENT frames
that the victim's PC applies. A replay on one PC matched (0.013 units mean, crashes included),
and a live 2-instance race through the relay ran with every rider following its owner. Next:
finish times and results, power-ups and lap in the snapshot, then a 4-PC test.

2026-10-16 step 4, a race friends can play (finding 047). Each puppet wears its player's rider
and bike, the AI riders are the same on every PC, every PC has the same grid, START skips the
flyby, and the countdown starts on a moment the host names on the server's clock (GO skew
41-58 ms). From there the pacing keeps to that clock: a PC that falls behind catches up (two
fields a frame). Snapshots carry checkpoints and the attack power-ups. Hits between humans work
both ways: proven by a scripted punch, with "PUNCHED BY DRUD!" on the victim's screen and its
puppet knocked aside on the attacker's. Each PC reports its finish, the host publishes the
order, and the end screen waits for it. A player who leaves becomes the host's AI rider.
4 players were tested as 2 games + 2 nettool bots replaying a recorded rider. The game window
now has its own message thread, so dragging it doesn't freeze the game. A port crash when a
rider breaks a trackside object is fixed (`func_00192D50`, still matching). The friends' zip,
`build/dhd-online.zip` (`port/tools/package_online.py`), has no server address: players paste
it into the launcher. Lag at the start of a race is 0.03-0.25 s, mostly 0.1-0.2 s. Two games
on one PC run slower than real time, so later lag on this PC isn't meaningful. Next: a real
4-PC race; check the game's own results screen against the host's order, the host leaving,
and a kick or throw by a puppet.

Round 49, CPU speed toward 144 (finding 066; validated improvements; 6.9 ms target unmet):
- Graphics FIFO observations use acquire loads; interlocked publications and
  sleep/wake barriers stay. Long queue stress medians 3.602 -> 2.462 s on a busy
  shared PC; order/hash, queued VIF fuzz and negative controls pass as expected.
- Four guarded native VU1 loops: geometry 1430 and 2DC8, particles 0E10 and
  projected particle quads 2E48. Exact arithmetic, flag/Q timing, old-value
  within-pair stores and raw moves retained. Default mask 4095; 255 isolates
  the pre-existing kernels. Unsupported inputs decline before direct writes.
- Target-call replay gains: 1430 4.5-5.2x, 2DC8 1.6-2.8x, 0E10 4.7-5.1x,
  2E48 4.6-5.6x. Three interleaved single-thread runs; busy-PC conditions,
  aggregate variation and rejected experiments recorded in the finding.
- Batched raw thread-cycle medians (old/all kernels) fall 12.54%, 15.47%,
  21.65% across three 12,000-call captures, including unchanged restore/check
  overhead. One counter pair per batch, 50 native replays per input, AB/BA/AB.
  This measures thread work, not live race FPS or the 6.9 ms deadline.
- All eight retail sections MATCH, no src/ changes. Integrated sequence:
  36,000 checks; forced variants: 19,060; independent PCSX2: 1,584 standard
  cases plus all 1,744 real target calls (positive new-kernel coverage).
  All pass; queue/VIF/GS/sequence/oracle negative controls fail as expected.
- Clean queue-only e65990ca and successive kernel commits e36d1fa8,
  8d544317, 7c996d9c pass default r46-plain renderer gate (14 EE comparisons,
  respectively 1,116 / 1,113 / 1,131 / 1,136 matching Z reads). Four-kernel
  clean 451bab30 also MATCHES: 14 EE / 1,090 Z reads through field 9011.
  Final report commit will be clean-built/gated again; no reference/exclusion change.
  heavy CPU load, with before/during/after process evidence and focused window.
  The historical 149.8 fields/s / 12.35 ms worst 1% is not a fresh baseline.

The later 0078 setup and fused setup+loop prototypes pass real/variant replay comparisons but are
discarded: whole-thread cycle medians regress in two of three captures. No extra setup dispatch or
default-mask change is shipped.

Round 49 follow-up: audited the 1430 native signature and added its short exit
and common GIF tail. Altering either tail to read MAC flags makes the old
kernel differ on 255/467 inputs; the stricter guard declines and matches all
467. Retail coverage remains active: 36,000 sequence runs, 19,060 forced
variants, and the 1,584-case PCSX2 suite all pass. See finding 066.

The tightened-signature runtime also passes all 1,744 real PCSX2 target calls with positive
coverage; rebuilt oracle negative control fails as expected. Clean 62ad9972 default renderer gate
MATCHES (14 EE / 1,102 Z reads through 9023). Repeated batched CPU-cycle medians fall 11.68%,
16.10%, 21.23%. A fresh guarded race campaign excludes the earlier exploratory/overlapped runs.

Baseline fields/s 153.410 / 152.231 -> optimized 172.718 / 158.455; worst 1% 12.238 / 12.141 ms ->
10.283 / 10.925 ms. Both whole-window averages exceed 144, but start windows remain below 144 and
the 6.9 ms tail deadline is unmet. All accepted runs (including a late-window regression) remain
reported. A single exploratory VU0 AOT sample is neutral; its default stays off. Final report
revision requires its own clean build and unchanged r46-plain gate; the final artifact manifest and
handoff carry its exact commit/result. No merge or push. Further unproven prototypes are not
retained.

GS follow-up to round 49 (finding 066): GS sampling showed queue consumers
mostly polling PAUSE; retain the spin policy without a valid latency comparison.
GS GIF staging is 7-12% of samples. Gather complete qwords when non-aliased and
no partial is pending; preserve callback have=4/storage and all scalar tails.
Single-thread GS-model replay raw CPU cycles fall 7.18-14.04% across six real
captures, three interleaved runs; a second replication series falls 8.32-10.67%.
18,000 synthetic packet comparisons, 48 aliases, five empty/invalid cases, and
1,174,599 real captured qwords match complete state and ordered callback bytes.
Both staging and callback-state negative controls fail. Updated GS suite
passes 3,000 / 6,657,394 pixel fuzz; pixel negative control fails as expected.
Live B/F figures above predate this addition; no new live FPS claim.
Clean d246fd05 before this addition MATCHES 14 EE / 1,109 Z reads through
9030. The final source revision requires a new clean build/default gate.

Post-GS integration: split FIFO/provenance/fences and VIF tests pass again; 200,000 / 16,140,614
UNPACK vectors have zero differences. All eight retail sections MATCH. Final GS source commit is
clean-built/gated for handoff.

Round 51 continuation (finding 066): clean 060383e5 gate MATCHES 14 EE / 1,101 Z reads; the VU0-AOT
arm independently MATCHES 14 EE / 1,108 Z reads. Fresh AOT differential: 3,432 cases; PCSX2: 936
interpreted and 936 AOT cases, zero differences. AOT remains opt-in pending a valid live benefit.
The three hottest VU0 entries 0000/02E0/05E8 now have whole-entry native kernels, guarded by
complete disc-loaded signatures on every call. Three million raw-bit CPU/data cases, 274
mutated-word guards and 792 PCSX2 cases match; all three separate result-corruption controls fail.
Single-thread median batch cycles versus AOT improve 3.32x / 5.00x / 4.59x. These are shared-PC
replay gains, not new live FPS. Final integrated clean build/gate follows. Slow-field analysis
retains every accepted outlier: the crowded start dominates the worst percent; 6285/9004 recur, and
no single modulo-eight phase explains the tail. Cause attribution beyond the measured waits remains
unproved.

Round 51 VU0 extension: 07B8 squared-length and 0808 direction-transform entries also use
whole-program native kernels (2.02x / 1.94x fewer batch cycles than AOT). Integrated five-kernel
proof: five million raw-bit CPU/data cases, 304 changed-word guards, 1,320 PCSX2 cases with positive
execution, all MATCH; all five result-corruption controls fail immediately. Full clean build and
default renderer gate are the remaining integration checks. See finding 066.

Round 51 integration done: clean 73ec1bb6 (3,797 fresh units, zero errors) passes default r46-plain,
14 EE / 1,118 Z reads through 9039, with positive execution of all five VU0 kernels. The actual
clean runtime object, with disc-loaded guards, repeats 3.21x / 5.36x / 5.41x / 2.05x / 2.10x lower
per-entry replay cycles than AOT. Current VU1 36,000-call replay and 1,584-case PCSX2 suite,
split/VIF tests and all eight retail sections MATCH. VU0 AOT remains opt-in. Combined CPU+FPS test
build follows on a throwaway branch; its artifact README records its exact source and gate.

2026-10-18 round-52 launch-isolation step complete (finding 067), acceptance still FAILED.
Per-update countdown/GO controller, crank and solver traces identify reaction-interval quantization,
thresholded pedal kicks and a straddling force-window defect; no isolated correction meets the
target. The held-cap three-seed AI means are 313.346914 / 325.804321 / 325.333951 s at 60/120/144
(27 finishes each). Cap/overlap experiments are OFF by default. Validated steering 60 replay matches
2,296,200 values; higher-rate full-route steering is explicitly censored, with supported short
windows recorded. Final clean b8a1641b passes default r46-plain (14 EE and 1,152 readback/Z records
MATCH) and complete cold countdown/GO trace coverage. Morning launchers remain on clean167fa3e2.
Next: actual collision-time force clears / rollback in 00131998, distinguishing committed and
tentative integration before another proposed gameplay fix.

Offline input-edge continuation complete (finding 067): the popup cache used whole-field timestamps
and retained one pressed mask for two/three high-rate updates. Its port-only stamp now uses the real
game frame; input/vibration/ action-lock timers keep legacy field units. 60 remains on the original
clock. Windows SendInput and SDL integration suites pass at 60/120/144: 60 cursor presses each move
once, all 18 punch taps each start one attack, confirms consume once, held buttons/sticks have no
neutral holes, keyboard ramp reaches full lock. DHD_NAV Down/Up each consume once at all rates.
Ten-second SDL menu holds preserve the original single move (before 60 and after 144). Clean
43b5c068 passes default r46-plain: 14 EE comparisons and 1,143 readback/Z records MATCH through GS
field 9064. FPS morning launchers point at this clean executable. This completes input acceptance
only; existing high-rate physics-equivalence failures remain open. Combined test integration is
merge-only and recorded in its private build/launcher evidence; no main merge, push or publication.

Current EE samples and a fresh guarded baseline identify generic inline-asm dispatch and VU/GS
pipeline work as remaining priorities. The baseline is 176.413 updates/s, worst-1% 10.474 ms; this
is one exploratory run, not a new campaign median. A matrix-template prototype uses 77.75% fewer
thread cycles (4.49x throughput); disabled VU0 timing removal also benefits short native entries.
Shared-PC microbenchmarks are explicitly noisy and are not converted to FPS claims. The combined
local prototype passes unchanged r46-plain: 14 EE and 1,147 readback/Z records MATCH. The existing
PS2 image recheck MATCHES all eight sections; src/ and all production files are unchanged. A
ten-minute guarded campaign obtained no quiet prototype timing slot, so no fresh frame A/B or
steady-144 claim is made. Finding 069 ranks five opportunities with estimates, risks and effort,
records rejected work, and identifies the remaining proof steps. This review does not change the
open high-rate physics acceptance work.

Win 4 (conditional VU0 timing) passes five million full-state raw-bit cases, 304 instruction guards,
1,320 PCSX2 cases and five failing corruption controls. Stats-on coverage also matches. The
unchanged r46-plain gate MATCHES 14 EE / 1,139 readbacks through field 9060. Guarded
production-object timing is queued; final retention requires measured benefit. Next: measured
inline-asm call census and specialization, then adaptive waits, lane-aware VU1 clamps, GS frontend.
Final clean builds/race A/B, combined-tip merge and morning launchers are still outstanding.

2026-10-18 revised user instruction cancels all performance timing. The owned controller is stopped.
Win 1 specialization passes 1.4 million full-state cases, 448 callback cases and seven failing
negative controls. Win 3 adaptive consumer waits passes exact full GS/VRAM/callback replay and
corruption controls for both policies, plus VIF differential tests. CPU gains and wake latency are
unmeasured. Finding 069 supersedes the earlier measured-benefit retention requirement. Next: VU1
clamps, GS frontend, combined-tip merge, final clean build/gate/launchers.

Finding 069 win 2 completed: four proven all-lane ACC clamps removed, compiler barriers retained.
All 36,000 real full-state/GIF replays MATCH; the corrupted implementation fails 748/2,000 cases. No
timings or extra oracle rounds.

Finding 069 win 5 completed: raw-register-keyed exact-GS metadata cache. Decoder and ordered
primitive replay/full GS+VRAM checks MATCH; corruption control fails. All five changes are included,
with no final timing claims. Final combined-tip merge, one clean build, one unchanged r46-plain gate
and launchers follow.

Adaptive offline timing continuation in progress (finding 067): clean runtime 04d7bc06 builds all
3,797 units with zero errors, ordinary flags, no recomp basis and 23 existing SDK stubs. High-rate
paced steps follow filtered elapsed wall time, bounded by the selected interval and 1/60 second; the
60/online clock paths stay original. Clock/Verlet and regular/variable-step input regressions pass;
the fixed-step negative control fails the new wall-time assertion. Baseline full-AI races and a
verified-PID forced-frame control are recorded. Final 120 all-AI race passes the clock target:
opening 0.794817 -> 1.000476, whole race 0.916123 -> 1.000027 game/wall. AI GO and nine-rider mean
finish improve in this fixture (323.598148 -> 321.374074 s). The 144/forced-after runs, final
r46-plain gates and combined integration are pending.

Finding 069 integration correction: first clean gate failed 33 canonical EE words despite matching
exact/native arms/readbacks. Inlined ps2_asm temporaries changed host-stack residue in legacy
quadword W slots; generic mode reproduced it. An out-of-line fast dispatcher restores the binding
frame and early full EE MATCH. Assembly differentials/negative controls pass again. Corrected
runtime relink reuses unchanged freshly compiled game objects; final gate must repeat.

Finding 069 implementation handoff complete: all five optimizations included,
combined tip 7b74859c merged. Accepted code 4cbe3c71 fixes the shared binding
frame regression; full-state differentials and all corruption controls pass.
Fresh 3,797-unit compilation and corrected runtime link succeed; unchanged
r46-plain MATCHES all 14 EE comparisons and 1,133 Z readbacks through field
checks are unchanged. Play-60/120/144 and README are in
push or release. Imported high-rate AI/physics acceptance remains open (067).

Round 50, keyboard text and lobby (finding 065, in progress): online name/code
fields accept layout-translated WM_CHAR typing, Backspace, Enter, Esc and
validated clipboard paste while suppressing keyboard pad mappings during entry.
Focused field regressions pass. Live SendInput and lobby work remain pending;
no clean-build or renderer-gate result claimed yet.

Round 50 continuation (finding 065): physical SendInput typing/paste, Enter,
Backspace and Esc verified in two real instances, including a loaded French
AZERTY layout. Retail rider and outfit/bike-colour previews work; readiness
counts 2 -> 1 -> 0 and host start work. Both PCs receive identical selected
outfit/bike records, and retain the online grid fix. Part 1 clean c22335b2
compiled all 3,797 units and linked. Camera handoff polish, after-race control
and final lobby clean build/default renderer gate are still being validated.

Round 50 lobby implementation complete, acceptance checks pending (finding 065):
retail rider/shop/outfit previews, host level selection, explicit READY, room
occupancy/readiness/waiting count, change and leave are implemented and exercised.
Two real instances race with identical selected triples and the preserved grid.
Physical results/NEW LEVEL input returns both to offline play. The shop-camera
handoff now restores the stock shell backdrop. Final plain clean build and
unchanged r46-plain gate are next; no final MATCH claimed yet.

Round 50 complete, keyboard text and retail online lobby (finding 065): name/code
accept physical typing, Backspace/Enter/Esc/paste with text-only key capture.
Two real local instances passed SendInput and French AZERTY checks, retail 3D
rider/outfit/bike-colour selection, host track selection, waiting counts,
ready/change/leave/rejoin, identical setup triples and preserved grid. Actual
results/NEW LEVEL input returned both to offline play. Protocol/server unchanged;
raise MIN_VERSION at release, no deployment. Text/net tests and 34 server tests
pass. Plain clean 719ea8a2: 3,797 units, zero errors, successful link;
build/port/clean/719ea8a2/dhd.exe (3fe65eb385707ef7...). Unchanged default r46-plain
gate MATCH: 14 EE comparisons, 1,094 readbacks through GS field 9,015, no failures.
Proof/captures in build/port/r65/; detailed paths and decisions in finding 065.

Round 48 guarded handoff (2026-10-18 03:36, finding 063): **partial / open**. Full
incidental review now includes pause/unlocked cases (3,302 readable files, 909 newly
distinct RGB images plus exact pixel aliases) and Career Freeride (245 readable, two
truncated files excluded). Japan energy pickup and sun/dust are visible, but the exploratory
route stalls at a fence. Native Japan loading also has a brief mosaic candidate. Boot
loading is confirmed native-only against exact; root/fix remains open. Local tasks retain
Russia as deferred-guard and the remaining work pending; captures under build/port/r48sweep.
Remaining menus, 23 track tours, actual all-track near-finish, weather/water/snow parity and
retail results/replay are explicitly incomplete. No player-visible fix or game-state change
claimed. Pinned own clean 044170e8 and plain unchanged r46-plain gate MATCH remain valid:
3,797 units, zero errors, 14 EE comparisons, 1,124 readbacks/Z checks, no failures;
build/port/clean/044170e8/dhd.exe. Later concurrent runtime changes are outside this
validation.

Release 0.2.4 prepared, not published (2026-10-18, finding 053 section 10): R 8a6f6e18,
version header only over 9177a74d. Plain clean build: 3,797 units, zero errors, successful
link. Unchanged default r46-plain gate MATCH: 14 EE comparisons, 1,087 readbacks/Z reads
through GS field 9,008, failures empty. Release zip + latest.json in build/release, retail
scan clean and icon present. Extracted-zip physical-key smoke, music preference EXIT
GAME/restart, typed join code, two-instance local online selection/READY/race and real 0.2.3
-> 0.2.4 updater preservation checks PASS. Test temp folders deleted; only own PIDs stopped.
Round 51, sweep fixes (finding 068; complete, branch fix/sweep-visuals): loading-card root isolated
in a field-1 exact GS history. PSMT4HL font uploads change only alpha, but native invalidated whole
colour pages and imported stale CPU RGB. Masked GPU alpha uploads now preserve RGB, the other nibble
and untouched pixels. Identical-stream baseline/ordering-only/after replay isolates this cause;
PCSX2 replay and independent retail show the complete card. All 41 fresh before/ after boot fields
reviewed; native now keeps it. Thirteen synthetic GPU pixel checks pass; all eight matching-source
retail sections MATCH. Clean b34e28c0: 3,797 units, zero errors, successful link. Fog Editor, dense
Japan transition, final unchanged r46-plain gate and launcher are complete; details follow.

Round 51 continuation: Fog Editor grey-fill candidate cleared. Fresh retail, native, exact and PCSX2
GS replay agree across all five sliders, including a Right input on RED. Matching 00167370 calls
00191078, whose explicit neutral RGB 90 -> 150 Gouraud bar matches the traced GS vertices. No colour
change is made. See finding 068 for the source, packet and picture proof paths.

Round 51 Japan visual acceptance: 301 consecutive fields per exact/baseline native/corrected native
arm, selected track 12 read without writes. The PSMT4HL font upload at 6408 precisely triggers the
baseline's Italy mosaic. The same alpha preservation fix keeps Japan's card; identical-stream
before/after native, PCSX2 replay and dense independent retail confirm it. Retail's relevant
539-field window is complete; 34 earlier uncaptured reset fields are explicitly excluded from
coverage, without changing the strict helper. No extra runtime change. All three requested
candidates now have outcomes.

Round 51 final acceptance: unchanged r46-plain renderer gate MATCH, 14 EE comparisons and 1,114
identical readbacks/Z reads through GS field 9,035; failures empty. Both final captures are field
7,715. Native scale 2 is used, without widescreen in either gate arm. No reference, exclusion or
checker change. All eight retail ELF sections MATCH; 13 synthetic pixel cases PASS.

0.2.5 code cleanup (branch cleanup/0.2.5 over 99d04692; behaviour unchanged): the frame-rate,
CPU-speed, performance-review and loading-screen work is kept; rejected trials (grip, pedal cap, AI
jitter, force overlap), investigation probes/traces (particle probe, launch and solver traces,
DHD_INPUT_TRACE, DHD_RACE_PAD_RECORD), statistics counters, stop-at-finish helpers, the
DHD_TEST_SDL_PAD virtual pad and the one-off measurement scripts are removed (still in git history
at 99d04692). Kept check for the high-rate path: `port/tools/test_fps_race.py` (seed 81257, unpaced
DHD_FPS=144, DHD_FPS_TRACE rider state and step rows compared to a reference run). Kept diagnostic
switches: DHD_ASM_GENERIC (ps2asm_fast.h), DHD_VU0_STATS, DHD_VU_INTERPRETER, DHD_VU0_NATIVE
(vu0_interp.c), DHD_FIFO_ADAPTIVE (graphics_queue.c), DHD_FPS_TRACE/DHD_FPS_STOP (fps.c).

Release 0.2.5 prepared, not published (2026-10-18, finding 053 section 11): R 18abe6f0, includes
findings 066-069 and the recorded code cleanup. Plain clean build: 3,797 units, zero errors,
successful link, no recompilation basis. Unchanged default r46-plain gate MATCH: 14 EE comparisons,
1,151 matching readbacks/Z reads through GS field 9,072, failures empty. Zip and latest.json in
build/release; retail scan clean, icon present, archived exe equals clean R. Extracted-zip
first-start chooser/title/menus/race, music preference EXIT GAME/ restart and physical room-code
typing PASS. Ini-selected 144 race runs; this smoke does not resolve finding 067's high-rate
AI/physics-equivalence limits. Two-instance local-only Wrangler MIN_VERSION:0.2.4 online race PASS;
reciprocal rider views and snapshot exchange confirmed. Downloaded published 0.2.4 -> local real
0.2.5 updater PASS; dhd.ini/save byte-identical, installed exe equals R. Docs-only preparation
commit; no push, release, deployment or server/site changes.

Online lobby navigation repair (2026-10-18, finding 065): online modals restore
view/camera/projection, picker completion/cancellation uses stock title cleanup, and
leaving/disconnecting clears selection requests and restores offline menu setup/cursor. Targeted
shell/host compilation passed. Live navigation smoke was unavailable (local harness input did not
advance the memory-card screen); no renderer/exactness work requested. Clean build and executable
are in the task handoff and build/port/clean manifest. 

Level-start repairs (2026-10-18, finding 072): host/guest flyby input uses the controller after
online setup; only phases 2..4 count as GO. Accepted retail restart/level actions replace phase-drop
inference; guest following clears on acceptance/initialization and has a 30-second/1,800-field
give-up. Results use actual menus/rows, including early phase-2 results. Two-client normal start,
pause/results TRY AGAIN/RE-START/NEW LEVEL, same/changed track and countdown restart PASS; offline
physical skip PASS. Goggles and pre-load level chooser now keep 4:3 proportions in widescreen; final
native 60-fps gate MATCH, all 14 EE checks and 1,123 readbacks/Z reads through GS field 9,044. No
push/release. 
