# Round 26: waits and broad native performance

## Late-race readback breakdown (diagnostic only)

A temporary QPC probe separated VIF retirement, GS retirement, StoreImage setup,
GPU staging and Map. It was removed before candidate builds. Logs are local in
`build/port/r26-profile`; the source backup and probe script are under ignored
`build/port/tmp`. This diagnostic run is not an exactness-gated release build.
The 30-second late-race interval (QPC seconds 431191 through 431220) contains
1,297 64-byte Z StoreImage calls, about 43.23 per second, or one per rendered
frame at the observed late plateau. No immediate GPU readback maps occur in
that interval.

| Operation | Calls | Total ms | ms/probe |
|---|---:|---:|---:|
| VIF retirement, all callers | 14,271 | 11,903.105 | 9.177 |
| GS retirement, all callers | 14,271 | 67.177 | 0.052 |
| Full fence at deferred settle | 1,298 | 11,745.221 | 9.056 |
| Deferred staging Map | 1,297 | 10,643.551 | 8.206 |
| Stage copy/flush | 1,297 | 417.615 | 0.322 |
| StoreImage pre-setup fence and kernel poll | 1,297 | 40.668 | 0.031 |
| StoreImage setup packet and post-setup fence | 1,297 | 11.025 | 0.009 |

These rows overlap: settle's full fence includes VIF/GS retirement. They must
not be summed as independent frame costs. Map and the settle fence account
for about 17.3 ms/probe. The existing implementation already merges probes
on unchanged captured pages and delays Map until first CPU use. A ring or
batching alone cannot combine multiple probes in this one-probe workload.

The tested GS-owner rendezvous prototype resolved the captured snapshot at a
packet boundary. It preserved the original copy point and passed exactness
gates, but regressed paced race fps to 41.1/41.1 from 47.1/46.9. It was removed
and is not a shipping optimization (details below).

## Research

- Microsoft [Map](https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-map)
  and [copy/readback synchronization](https://learn.microsoft.com/en-us/windows/uwp/graphics-concepts/copying-and-accessing-resource-data):
  copies are queued; mapping before completion can stall. DO_NOT_WAIT reports
  readiness, not permission to return old data. No source code copied.
- LLVM [instrumentation PGO](https://clang.llvm.org/docs/UsersManual.html#profile-guided-optimization).
  The installed clang 23.1.3 has only an x64 profile runtime. For the planned x86
  experiment, compiler-rt profile sources matching clang commit
  `0d261d1ca552c95a8f007e061c787ac7132fbcbc` were downloaded under ignored build/,
  including their Apache-2.0 WITH LLVM-exception licence. No third-party source
  or binaries are added to this repository.

## Probe bypass and consumption timing

The reviewer-requested `DHD_ZPROBE_NOWAIT=1` experiment fills deferred Z results
with zero and skips the settle fence/Map. It is diagnostic only, default-off in
its private executable; all probe code was removed from the working tree.
The existing GS profiler confirms **0.0 ms/s readback time** in both bypass runs.

| Mode | Whole-race fps, two runs | Late race (wall 120-150 s), two runs |
|---|---|---|
| Exact control | 47.1 / 46.9 | 43.395 / 43.255 |
| Fixed zero, no settle wait | 47.3 / 47.3 | 43.327 / 43.347 |

The wait moves: no meaningful late-race gain. Main-thread samples in the first
bypass run show 35.23% in NtWaitForSingleObject, 30.36% in NtDelayExecution and
more driver activity; the exact diagnostic trace had 74.94% in
NtWaitForSingleObject. These are instruction-pointer classifications using the
loaded 32-bit modules, not an unwound driver stack. They do not yet prove which
later D3D call imposes the replacement wait. A stale-depth shipping change is
neither implemented nor justified by this upper-bound result.

A separate exact trace used D3D EVENT queries with DONOTFLUSH, serviced on the GS
owner at a packet boundary before the normal settle fence. All 1,300 late-race
samples agree:

- The first touch is `func_002286F8+0x26A`, address `0x0872DC00`, not an unrelated
  access to a shared guard page.
- Issue field N, consume field N+1: exactly one virtual field later.
- The probe copy is not complete. The latest completed GPU display marker is N;
  display marker N+1 has been submitted but is incomplete. The probe lies after
  the N display marker. This bounds the GPU backlog to the unfinished render
  interval containing the probe, rather than several older frames.
- The VIF and GS workers have already retired commands tagged N+1. All older
  N work was submitted; remaining CPU queues are newer work, averaging 821,933
  VIF bytes (range 703,296-896,880) and 3,938 GS bytes (range 0-24,384).

Thus next-frame use does not guarantee GPU completion: virtual VBLANK advances
on the EE, and the EE reaches flare evaluation while the GPU is still rendering
the prior interval. The queue counters describe partial frames, not a count of
complete frames waiting. The rejected blocking GS rendezvous stopped newer draw
submission: 41.1/41.1 versus 47.1/46.9 fps. Nevertheless all four reference EE
checks, eight BMPs and 664 Z readbacks matched for that rejected experiment.

The small per-page staging prototype separately passes all four EE checkpoints,
eight BMPs and 664 Z readbacks. Paired paced race measurements against the
fixed control were 47.0/47.0 versus 46.9/47.0; it was removed for no gain.

## Deterministic Huffman table construction

The compiler trials exposed actual host-stack bytes copied into guest RAM by
`func_00240CB8` (huft_build). Retail instructions 0x241248/0x241284 store a
16-bit leaf at sp+0xA4; 0x2412B0..0x2412BC copy the entire eight-byte local.
The local's bytes 2-3 and union bytes 6-7 were not initialized. The PCSX2
`fineB-retail/retail-ee/ee-g+31.bin` capture confirms retained union bytes:
the fixed distance table at 0x8CA4D8 has leaf 1 stored as 0x008C0001, while
the native reference at 0x8CA4C8 has 0x088C0001. These are stale pointer
upper halves, not decoder inputs. Other tables retain unrelated stack words.

The port-only patch names/zeros the padding, initializes the entry local, and
clears the whole union before building each leaf (including invalid leaves).
The 16-bit leaf member, eight-byte layout and child-pointer representation are
unchanged. Canonical zeros intentionally replace unspecified retail padding;
no comparison exclusion is added. New controls must be captured with this fix.
`test_huft.py` checks the patched producer in four 32-bit builds (O1/SSE2,
O2/SSE2, O2/SSE4.1, O2/AVX2), 256 stack-poison patterns each, complete and
incomplete trees, all leaf symbols, padding, and full allocated table bytes.
Live decompression, frame/state gates and compiler retrials follow.

### Late-window worker utilization with probe bypass

At wall 120-150 s the fixed-zero diagnostic presents 1,301 frames in 29.995 s
(43.38 fps). VIF execution busy time is 15.568 s (51.9%), worker CPU 16.953 s
(56.5%); GS execution is 9.503 s (31.7%), CPU 11.938 s (39.8%).
VU arithmetic/dispatch costs 7.597 ms/frame, GIF enqueue 1.360 ms/frame.
These counters include instrumentation and are not additive across workers.

| Key (overlay B, MSCNT unless entry=address) | Calls/frame | VU ms/frame |
|---|---:|---:|
| 0078/1040 | 596.46 | 2.3455 |
| 0078/1CB0 | 130.10 | 0.6487 |
| 1638/1040 | 113.96 | 0.5806 |
| 0020/0020 | 10.83 | 0.5488 |
| 17E0/1040 | 91.53 | 0.4832 |
| 0040/1040 | 98.08 | 0.2790 |
| 07F0/1040 | 76.10 | 0.2674 |

B0078/1040 remains the largest VU target, but the worker is not continuously
busy. The queue depth alone does not establish a pure VU throughput limit.
Serial critical sections and GPU submission/completion dependencies remain
possible limits; the exact-path late-window profile agrees: 1,299 frames / 29.993 s
(43.31 fps), VIF busy 51.6% (CPU 56.8%), GS busy 29.7% (CPU 35.3%).
B0078/1040 costs 2.2857 ms/frame, B0078/1CB0 0.6312, B1638/1040
0.5734, B0020/0020 0.5454, B17E0/1040 0.4722. Removing the readback
wait barely changes utilization. More VU optimization can help, but neither
worker is saturated throughout the window.

The fixed control differs from ref22 in exactly 1,057 words at each of GO+0,
+2, +12 and +16 under the unchanged pointer exclusions. 1,055 changes clear
only the upper 16 bits of Huffman padding/leaf slots; two clear the unused
union of invalid (e=99, b=5) entries at 0x8CA544 and 0x8CA5C4. No other
non-excluded state changes were observed. This is a reference refresh for
defined table bytes, not a weakening of the checker.

## CPU requirements and 64-bit scope

SSE4.1 trials require an SSE4.1-capable x86 CPU. AVX2 additionally requires
AVX2 hardware and OS support for saving YMM state; FMA remains disabled.
O2, ThinLTO and PGO alone retain the SSE2 minimum. No AVX2 default is approved.

A 64-bit worker could benefit from 16 architectural XMM registers instead of
eight, but a whole-port conversion is substantially more than changing the
compiler target. Native pointers are embedded in 32-bit guest structures and
callback slots; the current ABI bridges, C++ symbol handling, recompiled
assembly and address mirrors assume i386. A rough planning allowance is
4-8+ engineer-weeks including layout/ABI conversion and deterministic gates,
with low confidence until a pointer inventory is made. Extra registers alone
do not remove GPU synchronization or serial frame dependencies. No 64-bit
implementation was attempted.

Huffman fix live validation: the fixed SSE2 control's native-on/off dumps
match at GO+0/+2/+12/+16, eight menu/race BMPs match, and 663 lens-flare
readbacks match through virtual field 3285. The same eight BMPs and 663
readbacks also match the old r25-clean control. No crashes were reported.
The new canonical reference is `build/port/compare/r26-huft/0`; native-on
control is its sibling `1`. The strict comparator and exclusions are unchanged.

## Compiler retrials against the fixed control

SSE4.1 now passes all four EE checks, eight BMP comparisons and 665 exact
Z readbacks through field 3287. Its previous failure was the Huffman bytes.
AVX2 has no remaining Huffman differences, but still differs in 48 other
words at every checkpoint (including 0x4C9B78 and 0x8A0440..0x8A0730).
Its eight BMPs and 665 readbacks match. It is not accepted or made default.

The clean full-source O1 build passes all live gates (665 readbacks). The
corresponding game-O2 build fails with 38/45/37/38 words at GO+0/+2/+12/+16.
Some are real one-ULP float differences: 0x2FF834 and analogous fields change
117.333328247 to 117.333335876; 0x12ECEA8 changes 4038.383789 to 4038.384033
at GO. Other differences include host pointers and sound-command storage.
Its eight BMPs/readbacks still match, but it is rejected by the authoritative
state gate. The rewrite runs before optimization; native float multiplication
and conversions still depend on runtime rounding, and optimizer folding needs
a separate audit before raising the game's optimization level. The precise
producer of each differing float has not been established in this trial.

Recompiled-assembly O2 is separately rejected: 57/58/57/57 differing words,
mostly 0x8A0450 onward, with an additional sound-buffer word at GO+2.
ThinLTO is rejected with 33/34/33/33 words: four repeated object fields at
0x37EB0C + n*0x380 change from 0xBD8F5C29 to 4, and repeated pointer
storage around 0x4C22CC changes representation. Neither is treated as benign
or excluded. Both still pass the eight image comparisons and exact readbacks.

PGO (VU interpreter/AOT translation unit only) and small staging buffers each
pass all four fixed-control EE checks, eight BMPs and 665 exact Z readbacks
through field 3287. The staging change uses 8 KB resources for touched VRAM
pages; it does not use stale snapshots or move the issue point.

With no game or build running, interleaved native-off replay measurements
(32 captures x 3,000, three runs) are:

| Build | Seconds | Median vs SSE2 |
|---|---|---:|
| SSE2 control | 1.251 / 1.211 / 1.230 | baseline |
| SSE4.1 | 1.145 / 1.139 / 1.153 | 6.9% lower |
| AVX2 | 1.083 / 1.090 / 1.072 | 12.0% lower; live gate failed |
| ThinLTO | 1.208 / 1.204 / 1.222 | 1.8% lower; live gate failed |
| PGO | 1.280 / 1.254 / 1.250 | 2.0% higher |

All hashes are `6DB1972798276582`. SSE4.1, AVX2, ThinLTO and PGO each passed
the native-off PCSX2 VU oracle with 1,616 cases and zero failures. The count
is 1,584 generated instruction cases plus 32 captured programs; 1,584 alone
was the generated subset, not a silent reduction. Game O2/assembly O2 do not
change the VU bench/oracle objects. The retail image still matches all eight
sections. Paced measurements use native geometry on, including for PGO, whose
training also used native geometry; native-off bench cost is not its speed gate.

### First paired paced measurements

All runs use the same fixed control, native geometry on, with no build work
in the measured menu (wall 20-40 s) or race (GO navigation +20..+80 wall s)
windows. Each run lasts 180 s. No application crashes occurred.

| Candidate | Menu fps, two runs | Race fps, two runs |
|---|---|---|
| Fixed SSE2 control | 55.3 / 55.2 | 46.9 / 47.0 |
| SSE4.1 trial | 57.7 / 57.8 | 46.9 / 46.9 |
| VU-only PGO | 56.3 / 56.2 | 46.9 / 47.0 |
| Small staging | 54.4 / 55.5 | 47.0 / 47.0 |

SSE4.1 improves menus and the native-off replay but not the paced race.
PGO and small staging do not establish a race gain; neither becomes default.
A full 27-runtime-object SSE4.1 build with all clean-source O1 game objects
also passes all four EE checks, eight BMPs and 665 Z readbacks. SSE4.1 remains
an explicit trial option: the measured race benefit is zero, so the normal
build keeps its SSE2 minimum.

### Flip-model presentation trial

The old scanout uses DXGI_SWAP_EFFECT_DISCARD. A private opt-in prototype
selects FLIP_DISCARD with the existing two buffers, single sampling, full
back-buffer copy, and Present(0, 0). D3D11 keeps the logical buffer-zero
identity across presents, so the existing explicit CopyResource destination
remains valid. No pixel evaluation, guest timing or readback is approximated.
Microsoft's [flip-model guidance](https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/for-best-performance--use-dxgi-flip-model)
and [DXGI overview](https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi)
were consulted; no source copied. The source probe was removed after building
the private executable. All four EE checkpoints, eight BMPs and 665 Z
readbacks match; logs confirm swap effect 4 in race and menu. Paced race
is 47.8/47.8 versus 47.0/47.0 for the paired control. The final implementation
adds a hardware-discard fallback, retains DHD_FLIP_MODEL=0 as the referee,
and resizes after window transitions. Its final validation is recorded below.

### Second paired paced measurements

| Candidate | Menu fps, two runs | Race fps, two runs | Decision |
|---|---|---|---|
| Fixed SSE2 control | 55.2 / 55.3 | 47.0 / 47.0 | Control |
| AVX2 | 54.5 / 58.2 | 47.0 / 46.9 | Reject: EE differences |
| Game O2 | 55.5 / 55.5 | 47.0 / 47.0 | Reject: EE differences |
| Recompiled assembly O2 | 56.5 / 55.0 | 46.9 / 47.0 | Reject: EE differences |
| ThinLTO | 55.8 / 55.7 | 47.1 / 47.0 | Reject: EE differences |
| Flip presentation prototype | 55.5 / 55.4 | 47.8 / 47.8 | Final validation below |

No trial crashed. AVX2/O2/ThinLTO failures are retained as failures; image
agreement alone does not authorize their use. No comparison exclusions were
added. PGO and staging sources are not enabled in the normal build.

### Intermittent control menu difference

The full-runtime SSE2 control `r26-sse2-full` passed all four EE checkpoints,
four race BMPs and 665 readbacks, but all four menu BMPs differed. Its loaded
PE sections are byte-identical to the earlier passing `r26-huft-game-o1`
executable. A repeat with that same SSE2 executable matched all four menu
BMPs. This is an unresolved run-dependent difference, not evidence that an
ISA flag changed the renderer. The failing logs have 12,540 texture uploads
at field 600, increasing each frame; the passing repeat has 387, remaining
constant. PATH1 counts and the navigation fields match. The differing pixels
are on the menu terrain. This points the next investigation toward texture
submission/cache state upstream of the GPU, rather than screenshot timing.
The precise cause has not been established. The failed capture is preserved;
a passing repeat does not erase it or establish global determinism.

Artifacts: `build/port/compare/r26-sse2-full/1/menu` (failure),
`build/port/compare/r26-sse2-menu-repeat` (passing repeat). The full-runtime
SSE4.1 run passed against the fixed canonical control, not this failed menu.

### Final flip-model build

The normal runtime implementation uses flip discard by default and falls back
to hardware discard if creation is unsupported; the existing WARP fallback
remains. DHD_FLIP_MODEL=0 selects the original presentation path. This adds no
new CPU requirement; flip discard requires Windows 10, while older systems
retain discard. ResizeBuffers is also requested after WM_SIZE even if the GS
scanout dimensions are unchanged.

The full 27-runtime-object SSE2 build with clean O1 game objects
`build/port/r26-final-flip/dhd.exe` passes GO+0/+2/+12/+16 against the fixed
reference, all eight menu/race BMP byte comparisons, and 665 exact Z readbacks
through field 3287. See `build/port/compare/r26-final-flip`. The VU code is
unchanged from the oracle/bench-checked control. Final paced race results are
47.8/47.8 fps, confirming the prototype against the 47.0/47.0 paired discard
control. Final menu averages are 42.5/55.4 (prototype 55.5/55.4); the first run
was slower in menus, so no menu gain is claimed. No application crashes.
The retail image was rechecked after the final build: all eight sections MATCH.

Flip presentation is retained; compiler defaults stay SSE2/O1. The final build
still plateaus around 44 fps late in the race, and steady 60 fps is not achieved.
No stale readbacks, approximation, new exclusions or experimental profiling
code ship. Next: instrument GPU timestamps alongside worker/fence events to
identify the serial critical path, and capture EE texture-cache state plus GS
packets when the intermittent menu upload divergence recurs. Remaining
AVX2/O2/ThinLTO guest-word differences need producer tracing before retrying
those flags. The work stops for review here.
