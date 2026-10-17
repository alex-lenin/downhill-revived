# 027 - Real-time speed: round 19 (2026-10-15)

Goal: a steady 60 fps on the user's PC (RTX 3070, Windows 11), never faster and never slower.
At the start of the round the game ran at about half speed and the speed wobbled. This finding
records the measurements, the costs, what changed and what is left.

## Method

- Wall-clock fps with no capture, framelog or probe: presented frames per second
  (`DHD_PROFILE` off), averaged over menu 33 and over race start +20..+80 s, with the same
  navigation and race pad each time.
- The user was playing another game on the same PC, so each build was measured twice.
- Profiles: separate builds with QPC counters around the VU, VIF, GS, present, kernel and game
  code (the probe exes were never committed). The GPU's busy time comes from Windows' GPU
  counters for dhd.exe.
- VU1 speed: a replay benchmark of 32 VU1 programs captured in the race (`DHD_VU_CAPTURE`),
  3000 times each. Every step had to keep the hash of all results (registers, VU memory, the
  kicked GS data) unchanged: `6DB1972798276582`.
- Determinism: EE dumps at race clocks GO+0, +2, +12 and +16, compared with the dumps from the
  round's first build. Allowed differences, all checked by hand:
  - host pointers (0x40000000-0x42000000 and 0x01000000-0x08000000);
  - the word at 0x2B461C, which differs between two runs of the same exe;
  - 0x4B1EA4..0x4B1EBC: leftovers of native host call frames in dead stack. 0x4B1EA4 holds a
    host code address, which moves with every build.

## Results

| Build | Pacing | Menu 33 fps | Race fps | Notes |
|---|---|---|---|---|
| start of round, run 1 | wall | 16.1 | 19.3 | |
| start of round, run 2 | wall | 14.0 | 17.9 | |
| after VU step 1 | wall | 20.3 | 23.8 | |
| final, run 1 | paced | 29.5 | 24.5 | steady |
| final, run 2 | paced | 29.5 | 24.5 | every 5 s bin equal to run 1 |
| final | wall | 32.5 | 27.6 | crashed 125 s into the race (see below) |

VU1 replay benchmark: 2.1 s at the start, then 1.6 with inlining, 1.53 with lazy flags and 1.45
with delay slots. The plain interpreter (no translation) takes 2.7 s.

Race dumps of the final build are identical to the start-of-round dumps, apart from the
allowed host-frame leftovers. The PCSX2 VU1 comparison (1616 cases, `--aot`) has 0 failures;
the runtime tests pass.

## Where a race frame goes (final build, paced, ~41 ms)

| Cost | Share |
|---|---|
| VU1 microprograms (translated and interpreted) | 38-45% |
| Waiting for the GPU at the game's store-image reads | ~34% (~13 ms) |
| GS on the CPU (GIF, primitive setup, queue) | 9-13% |
| VIF1 unpack | ~6% |
| ps2asm (inline-asm runtime) | 3-4% |
| Game code | 2-3% |
| Sound | ~1% |
| Kernel and other HLE | ~1% |

The GPU is about 53% busy for dhd.exe. The wait is not raster work running slowly: it is GPU
time spent after the CPU work instead of alongside it.

- After drawing the world, the game reads small VRAM rectangles back every frame:
  `func_00228328` → `func_00244228` (`sceGsExecStoreImage`) → `gs_read_image`.
- Each read must wait until the GPU has drawn everything queued before it, so the frame's
  GPU raster time is added to its CPU time.
- An attempt to start GPU work earlier (a context Flush after every dispatch) cut the readback
  wait to 5 ms/s. But the CPU then blocked inside Flush, the race did not get faster and the
  menu fell to 21.5 fps. It was reverted.

## Changes

1. **VU1 (commits 3880d186, 3cfead4a).**
   - The translated-block lookup is cached per PC. It is reset by a code version that VIF MPG
     bumps whenever a word of VU1 code changes.
   - The hot register helpers are inlined.
   - MAC/status flags are retired lazily, only when an instruction reads them or the flag ring
     fills.
   - Branch delay slots are translated as single-pair blocks.
   - The VU state is cleared without the 16 KB VU1 file.
2. **VIF1:** a whole-word UNPACK path for 32-bit-aligned stream data (3880d186).
3. **ABI:** the fourth argument of `func_00202760` (port/patches.py, 3880d186). This changes
   game state legitimately, so the round's dumps are compared against a build that already has
   it.
4. **GS (4876f586).**
   - VRAM-to-VRAM transfers (TRXDIR 2, about two per race frame) used to read all 4 MB back,
     copy on the CPU and upload again. They now run on the GPU (`raster.hlsl` `local_copy`), in
     order with the draws.
   - Rectangles whose source and destination share pages keep the CPU path, which has the GS
     pixel order.
   - Store-image reads and `sync_range` now copy back only the GPU-written pages they touch.
   - `test_gpu` checks GPU copies against the CPU path for these cases: 32-, 24-, 16- and
     16S-bit, 8- and 4-bit, Z to colour, and an overlapping rectangle.
5. **Pacing (c2f254c1).** See the next section.

## Pacing

Before, wall time drove the fields.

- A slow frame spanned 2-3 fields.
- The game's delta (`func_00238D98`, capped at 2) then stepped the race by 1 or 2, so the game
  ran at a varying speed.

Now normal play runs the virtual frame clock: one game frame per field, delta always 1, as
in the PCSX2 comparisons.

- Each field waits for an absolute QPC deadline at 59.94 Hz (`pace()` in `port/hle/kernel.c`).
- The wait is a high-resolution waitable timer down to 0.7 ms before the deadline, then a short
  spin (`host_wait_wall`, `port/hle/host.c`).
- More than two fields behind, the deadlines restart from now. Nothing is skipped and there is
  no catch-up burst.
- The game is therefore never faster than real time and the race step never changes. Below
  60 fps it runs uniformly slow: at 24.5 fps that is 0.41x real time.
- Presentation does not pace the game.

Switches:
- `DHD_FRAME_CLOCK=1`: unpaced, for comparisons.
- `DHD_PACE=wall`: the old wall-time fields. Closer to real time on a slow PC, but the speed
  wobbles.

## Late-race crash (open, needed for online play)

All builds of the round crash, as earlier builds did: 3 of the first 4 races, and later runs
too, 40-125 s into the race. Two players online need it fixed.

- The fault is an access violation (C0000005) in ps2asm `_run` (+0x2600). It is reached from
  the inline asm of `func_00214BA0`, the spatial batch query, at the `lqc2` load from
  `records[k].x0`.
- The base pointer holds a float, for example `C45B3A10` (about -877.8); v0 = `C0D66666`
  (-6.7f). A caller has passed a float, or a structure with a different layout, where the
  query expects its record array.
- Candidate callers on the stack (not proven):
  - `func_00214A18` (a shared-header unit, not edited in this round);
  - `rc_func_0025B1A8`;
  - `func_00134D70`;
  - `func_00131630`.
- The `func_001460D0` caller's Group5D8 layout matches SpatialBatchQuery, so that caller is
  probably not the one.
- Most likely another argument/ABI mismatch of the `func_00202760` kind. Next step: log the
  caller's return address and the argument registers on entry to `func_00214BA0`, and compare
  them with the same race in PCSX2.

## Next candidates (in order)

1. **Overlap GPU raster with the CPU.** Get the GPU's ~13 ms out of the critical path: start GPU
   work in large batches before the store-image read, or answer the read from the GPU's copy
   later if the game doesn't use it at once.
2. **Static VU1 flag analysis**, after PCSX2 microVU's `mVUsetFlags`: compute only the MAC/status
   flags a later instruction reads.
   - Reviewer's estimate: 10-15%.
   - Trap: `fmac_flags` also clamps results (`arithmetic4`), so the clamp must stay when the
     flags go.
   - Dead ends, per the reviewer: tuning the lazy flag ring, longer blocks, deleting stalls.
3. **VIF:** one `unpack_run` per UNPACK format.
4. **The game units at -O2.**
5. **The menu cost:** menu 33 at 29.5 fps on mostly static screens.
6. **MTVU** (VU1 on its own thread): deferred.

## Sources

- PCSX2 microVU (`mVUsetFlags`) for the flag analysis idea (a reviewer's suggestion).
- The PCSX2 VU1 oracle (`port/tools/test_vu1_pcsx2`) to check the VU changes.
- Microsoft documentation for `CREATE_WAITABLE_TIMER_HIGH_RESOLUTION`.
