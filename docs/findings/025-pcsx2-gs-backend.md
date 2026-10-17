# 025: PCSX2's GS as the port's renderer (research)

Proposal (2026-10-14): replace the port's hand-written GS renderer (port/gs) with PCSX2's
GS, hardware renderer (D3D11 or Vulkan), behind `DHD_GS=pcsx2|native`. The repository is
GPLv3 and PCSX2 is GPL-3.0-or-later, so its sources can be vendored with their headers.
Reference tree: build/port/compare-pcsx2-src, PCSX2 v2.8.2
(fd9d310ccbb6b8b62c976da8886a3c8fd3a10ff3), the same one the comparison adapter builds.

## What the GS swap would change, measured first

The swap was expected to fix the countdown's inside-out riders, the missing sky and the
start-gate floor holes. Which GS draws the frame can be measured without building
anything: PCSX2 can replay the port's own GS capture, which is exactly the input a PCSX2
backend would receive.

- Countdown +120, port capture `trace-c120/gs/frame-000.gs` replayed by PCSX2 v2.8.2
  (`compare_pcsx2.py --mode gs-replay --from-dumps`, result in
  build/port/compare/replay-c120/scores.json): **MAE 0.0, block SSIM 1.0, 0 % of pixels
  over threshold** against the port's own render of the same stream. PCSX2's GS draws
  the riders inside out exactly as port/gs does.
- The cause is in the stream: every rider vertex has Z in 0xFFFBD7..0xFFFC00 against
  0x8F3..0x1EF2D9 in retail's GS dump, because about 220 rotation matrices in EE RAM,
  the camera orientation among them, have [3][3] = 0 instead of 1 (finding 024, round 4).
  A hardware write watch puts the bad store in the port's inline-asm interpreter
  (port/runtime/ps2asm.c `run`, an `sq` of a GPR), upstream of any GS.

So for the countdown frame the native GS already equals PCSX2's GS, and the renderer is not
where the countdown artefacts come from. The sky and floor holes are in the same frame
and the replay matches there too, so they are upstream as well; the later artefacts
(+10 s ... +50 s) haven't been replayed this way and would need the same check before a GS
is blamed. A D3D11 replay of the same dump also starts (PCSX2's D3D11 device on the RTX
3070), but the adapter's field counter doesn't advance during dump replay, so its GPU
capture hook saved nothing; the software result above already answers the question, since
both PCSX2 renderers consume the same wrong Z.

What a PCSX2 backend would still bring: PCSX2's coverage of GS features the native
renderer may not have (other games' tricks, upscaling, texture cache behaviour), and a
reference renderer to bisect against without a retail run. Neither fixes items 1-3.

## Options, if the backend is still wanted

PCSX2's GS is 64-bit only: GSVector (SSE4.1/AVX2 with x64 intrinsics), GSLocalMemory's
multi-ISA tables, the JIT'd software renderer (Xbyak x64), the common/ layer and its
dependencies (fmt, d3d12ma, vulkan headers...) are built and tested for x64 and arm64.
The port is 32-bit Windows (PS2 RAM mapped at host 0x08000000, the low view, scratchpad at
0x70000000, recompiled code with 32-bit pointers).

**(a) 64-bit helper process (the workable one).** PCSX2 already runs GS without a CPU:
pcsx2-gsrunner/Main.cpp boots VMManager with a dump file, and pcsx2/GSDumpReplayer.cpp
swaps in a fake R5900 (`GSDumpReplayerCpu`) whose step reads the dump's records
(Transfer with a GIF path index, VSync with the field, ReadFIFO2, Registers) and posts
them to MTGS (`GSDumpReplayerSendPacketToMTGS`, `MTGS::PostVsyncStart`,
`MTGS::InitAndReadFIFO`). The port's capture (port/gs/capture.c) writes exactly those
records already. A helper built from the same tree as the comparison adapter would
replace the file read with a shared-memory ring (one writer, the port; one reader, the
helper), with an event per record batch and per vsync. It presents from its own window;
the port then runs headless (`DHD_HEADLESS` already skips presentation). Returning the
image to the port (shared D3D11 texture) is only needed if the port must keep its own
window.

Determinism holds because nothing flows back into game state except what the port's own
GS model reports: FINISH, SIGNAL, LABEL and CSR vsync bits stay with port/gs, which keeps
consuming the stream for registers and readback, and the helper only draws. The one
real feedback path is GS→EE readback (`ReadFIFO2`, local-to-host transfers): the game
does use it (retail's GS dump at countdown +120 has 24 ReadFIFO2 records in 6 fields;
the port's capture of the same moment has none, since port/gs serves readback itself and
doesn't record it), so the port keeps serving it from its own GS memory, and the helper's
VRAM must be told about the read (or the transfer skipped) so its texture cache stays
consistent.

Cost: a new PCSX2 build target (the adapter's patch/build machinery,
port/tools/prepare_pcsx2_compare.py, already builds pcsx2-dhd-compare.exe from this
tree), the ring and the helper's fake CPU (~300 lines), a port-side writer in place of the
file capture, and a launcher. The whole PCSX2 core is linked into the helper (GS needs
VMManager, MTGS, Host, the settings layer), so "vendoring GS" means vendoring or
building PCSX2 itself; a pinned v2.8.2 checkout plus the patch script is lighter
than copying several hundred files into port/third_party.

**(b) Build GS 32-bit.** Not realistic: GSVector*.h use x64-only intrinsics and 64-bit
GPR assumptions throughout, the SW renderer's draw scanline JIT emits x64, and the
common/ layer and Qt-free host are only maintained for 64-bit; PCSX2 dropped 32-bit
builds in 1.7. It would be a fork that no upstream fix could reach.

**(c) Move the port to 64-bit.** The largest change by far: recompiled game code uses
32-bit pointers to PS2 RAM at fixed host addresses (low view, 0x08000000 base, scratchpad
at 0x70000000) and the ABI checks (abi_check.py) assume i686. It would also not change
the result above.

## Recommendation

(a) is the only reasonable way to put PCSX2's GS behind the port. It is not the fix
for the countdown artefacts. Those come from the port's inline-asm interpreter writing
matrices with a zero [3][3], which is one probe run from being pinned down (log the asm
template and caller at the watch hit; finding 024). Fixing that first, then
replaying the later moments (+10 s ... +50 s) through PCSX2's GS as above, would show
whether any artefact is the native renderer's at all, before building the helper.
