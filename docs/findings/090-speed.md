# 090: Game speed round 1 (x64 to 144 fps)

The changes finding 089's profile ranked. Each task has a section here.

## OP3: VU1 per microprogram

Research only, branch `rr/OP3` (from `render/rust` 84cb6af61). No game code changes are committed.

### Summary

- **One family of overlay-B mesh programs is about 91% of VU1 time in a race.** They all reach the
  `JR vi13` dispatch at 1040, 1CB0 or 12E0, which jumps to one per-vertex loop, kicks one triangle
  strip of 30-42 vertices, and clips through the shared routine at 1948/1CC0. B0078 alone is 38%.
- **(a) Flags.** The AOT path queues 112,000 MAC updates a field. 98.7% are never read. No
  program reads the MAC, status or clip flags that an earlier program left. VU0, the EE and VIF1
  never read VU1's flags. The only readers outside the program itself are the end-of-run
  registers that the state trace snapshots.
  - Flag work is 22% of the vu thread's work: 41% of AOT-block time and 40% of
    `native_clip_run`'s.
- **Cheaper than (a), found on the way: programs that miss their native kernels.**
  - B0040's MSCNT at **12E0** is the same `JR vi13` as 1040, but the kernel dispatch only looks at
    1040/1CB0, so the 0x20C and 0x260 loops run block by block. Allowing 12E0 cut those runs from
    11.3 to 3.0 us and from 6.2 to 1.9 us: -3.7% of VU1, measured. About 1 h plus the
    `vu1_seq_diff` check.
  - Loop **0798** (B06F0, vi13 0x0E0) and loop **3140** (vi13 0x616, B0040/0020/30A0) have no
    kernel at all. They are 7% and 4% of race VU1, and 06F0 is 16-21% in the menus. They are
    siblings of the 07F0 kernel's loop, which runs 3.5x faster than its blocks. 3-5 h each.
- **(b) VIF1.** UNPACK is 0.67 ms a field (12% of the vu thread). Command handling adds 0.3 ms.
  - Four forms are 72% of the unpack time: V2-16 masked, S-8 masked, S-8 and V4-5, all CL 3 WL 1
    and mode 0.
  - Half of all commands write 1-2 vectors, so the per-command setup matters as much as the
    per-vector loop.
  - Specialising these forms could save about 0.25 ms a field. 4-6 h.
- **(c) The GPU candidate is the B mesh family's per-vertex loop, led by 0078's 0170 loop.**
  - It is vertex-parallel apart from a 3-vertex strip window, and nothing reads its output back.
  - Batches are small: about 1,400 kicks of about 36 vertices a field.
  - The blocker is the exact lens-flare Z probe. The CPU still needs exact output for Z-writing
    batches near the probe, and for whole fields at the bottom edge.
  - 100-200 h. It pays off for the browser (VU1 is 38% of its graphics worker), but not for x64
    before (a), the kernels and the fold fix are in.

### Method

**Build.** A local copy of `vu0_interp.c`, `vif.c`, `graphics_queue.c` and `vu1_native_clip.py`,
plus two local headers (`op3_vif.h`, `op3_vu.h`), built with `build_host64.py` and release flags.
The wasm core is unchanged (CL7's `dhd_core.wasm`, sha256 13c7217c...). `DHD_OP3=file` and
`DHD_OP3_FIELDS=a-b,...` turn the counters on for windows of virtual fields:
- **Per run** (one `ps2_vu1_run`), keyed by overlay, MSCAL program, entry address, MSCNT or not,
  and VI13 when the entry is 1040, 1CB0 or 12E0 (the dispatch register). The counters:
  - rdtsc time, and the XGKICK callback's time;
  - pairs run in AOT blocks, in the interpreter or in native kernels, and the native kernels'
    time;
  - for each XGKICK, the GIF packet parsed in VU1 memory: tags, PRE primitives and their types,
    PACKED/REGLIST XYZ registers (vertices), A+D writes and IMAGE qwords;
  - kicks per run.
- **Flag liveness.** Every queued flag update gets an id. When it lands, it becomes the current
  MAC, or enters the 4-deep clip history. Each FM*/FS*/FC* read marks what it reads (FCAND by
  mask group, FSAND by bit group: ZSUO from the MAC, D/I from Q, sticky). Reads of state from an
  earlier program are counted apart. At the program's end the unmarked current MAC and clip
  entries count as "live at end". So **dead = queued - read - live at end**. Native kernels and
  `native_clip_run` commit flags directly; their state is tagged "native", and reads of it are
  counted but not attributed.
- **vu thread totals.** `execute()` per packet, `vif_feed`, UNPACK per form, MPG, PATH2/PATH3
  GIF, and calls of `host_vu1_file` (VU0 reading the VU1 register file) and `host_vu1_snapshot`.

- A hidden desktop, scale 2, `DHD_FPS=144`, `DHD_FRAME_CLOCK=1`, sound off, the full-pedal race,
  seed 81257.
- Windows: two menu stretches (fields 3000-4000 and 5000-6400) and the race (7400-14400, 66 s of
  wall time at about 107 fields/s). `DHD_BENCH=7400-14600` with `DHD_TIMELINE`.
- Three runs:
  - `stats2`, `stats3`: the baseline (stats3 adds VI13 at 12E0);
  - `k12e0`: the same exe with `DHD_OP3_12E0=1`, which lets the native kernels run from entry
    12E0.
  - So the absolute ms are about 1.5x finding 089's quiet numbers: VU1 4.55 ms a field here
    against 2.99 ms in PR1's sampled run.
  - Shares, counts and per-call ratios are what to use. Where this section gives "quiet" ms, it
    scales by PR1's numbers.
  - The counters add 2 rdtsc and a small table lookup per VU1 run, per XGKICK and per UNPACK
    chunk. That is about 0.1 ms a field of UNPACK time, so short commands look worse than they
    are.

**Work kinds inside the code.** The rdtsc counters cannot split flag work from arithmetic.
- That split comes from finding 089's own vu-thread samples (`s8-prof` + `s2-prof`, 27,859
  samples, PR1's exe and PDB; VU code unchanged since).
  - the innermost frame gives the kind (`queue_flags`/`retire_flags`/`fmac_flags4`/`native_result`
    = flags; `operand4` = clamp; `pair_stalls` = stalls; ...);
  - the first `aot_block_N`, native-kernel or `native_clip_run` frame gives the code unit.
- `keyflags.py` splits each block's samples between programs by how often each program ran that
  block start (the per-program block counts the local build dumps).

### Where the vu thread's time goes (race)

Measured ms a field (loaded PC). The quiet equivalents from finding 089 are VU work 3.86, VU1 2.99
and VIF1 0.66.

| part | ms/field | share of wall |
|---|---|---|
| VU1 programs (AOT blocks, native kernels, dispatch) | 4.17 | 44% |
| XGKICK: copying the packet into the GS ring | 0.38 | 4% |
| VIF1 UNPACK | 0.67 | 7% |
| VIF1 commands, MPG, run_micro bookkeeping | 0.30 | 3% |
| PATH2 DIRECT + PATH3 GIF | 0.06 | 1% |
| waiting for packets (`vu_idle`) | 3.73 | 40% |
| field (wall) | 9.38 | |

Menus: 3.6-4.2 ms busy a field (VU1 2.8-3.2), with 0.47-0.62 ms of UNPACK.

**Samples, by unit** (share of the vu thread's work; finding 089's data):
- native kernels 31%;
- AOT blocks 22%;
- VIF1 17%;
- `native_clip_run` 14%;
- run loop and dispatch 6%;
- `retire_flags` (out of line) 3%;
- memmove 2%;
- other 5%.

**By kind:**
- flags 22%;
- VIF 17%;
- upper decode and operation 13%;
- operand clamp 12%;
- lower 5%;
- dispatch 5%;
- register commits 4%;
- stall model 3%;
- Q/P 2%;
- kick 1%.

Inside the AOT blocks: flags 41%, lower 12%, clamp 10%, commits 9%, upper 8%, stalls 8%, Q 4%.

### Per microprogram

**Race, fields 7400-14400** (stats3). Each MSCAL program includes its MSCNT continuations.
- "in native kernels" is the share of the program's time spent in a native loop. That includes
  `native_clip_run`.
- "MAC updates" counts only the AOT and interpreter path.

| program | runs/field | share of VU1 | us/run | in native kernels | XGKICKs/field | vertices/kick | max/run | AOT pairs/field | MAC updates/field | dead | clip updates/field | flag-skip saving ms/field |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| B 0078 | 593.4 | 38.0% | 2.91 | 73% | 559.4 | 35.7 | 92 | 26104 | 13297 | 99.4% | 511 | 0.054 |
| B 0040 | 168.1 | 13.4% | 3.64 | 11% | 131.5 | 35.8 | 44 | 55200 | 38994 | 99.8% | 481 | 0.159 |
| B 1638 | 136.7 | 10.7% | 3.57 | 75% | 124.2 | 33.4 | 63 | 5750 | 3262 | 99.2% | 110 | 0.013 |
| B 06F0 | 191.0 | 7.1% | 1.68 | 0% | 117.0 | 19.9 | 44 | 29874 | 21557 | 99.1% | 0 | 0.088 |
| B 2DC8 | 43.2 | 6.2% | 6.51 | 79% | 53.8 | 29.7 | 89 | 2188 | 1165 | 99.7% | 39 | 0.005 |
| B 0020 (skinning) | 77.1 | 4.8% | 2.86 | 48% | 69.0 | 40.1 | 44 | 10958 | 7024 | 99.7% | 22 | 0.029 |
| B 07F0 | 202.3 | 4.2% | 0.94 | 43% | 151.7 | 31.8 | 44 | 8801 | 5061 | 99.0% | 0 | 0.021 |
| A 29C0 (particles) | 4.0 | 3.4% | 38.82 | 0% | 4.0 | 333.3 | 386 | 24276 | 10932 | 91.6% | 0 | 0.041 |
| A 2E48 (particle quads) | 9.3 | 2.6% | 12.73 | 79% | 57.8 | 72.9 | 512 | 979 | 278 | 93.3% | 9 | 0.001 |
| B 17E0 | 42.0 | 2.2% | 2.35 | 66% | 36.5 | 26.3 | 100 | 1906 | 990 | 98.9% | 31 | 0.004 |
| B 0010 (skinning) | 28.6 | 1.7% | 2.74 | 52% | 25.4 | 39.6 | 44 | 3077 | 1847 | 99.8% | 0 | 0.008 |
| B 1430 | 31.3 | 1.5% | 2.18 | 63% | 25.5 | 41.3 | 44 | 1326 | 561 | 99.0% | 25 | 0.002 |
| A 02D0 | 4.4 | 1.0% | 9.95 | 0% | 4.6 | 36.0 | 292 | 2577 | 1669 | 99.7% | 177 | 0.007 |
| B 30A0 | 20.0 | 0.9% | 2.15 | 0% | 11.3 | 42.0 | 42 | 4364 | 3115 | 99.4% | 0 | 0.013 |
| A 0E10 (sprites) | 8.6 | 0.9% | 4.91 | 74% | 23.8 | 64.2 | 410 | 458 | 111 | 100.0% | 0 | 0.000 |
| B 12F0 | 20.4 | 0.4% | 0.91 | 0% | 0.0 | - | 0 | 1227 | 1063 | 98.1% | 0 | 0.004 |
| A 26D8 | 0.9 | 0.2% | 11.37 | 0% | 16.7 | 0.2 | 36 | 676 | 139 | 98.2% | 0 | 0.001 |
| all | 1611.9 | 100% (4.55 ms) | | 53% | 1431 | 35.7 | | 181338 | 112088 | 98.7% | 1452 | 0.45 |

**Menus.** VU1 is 2.8-3.2 ms a field.
- Fields 3000-4000: B1638 31%, B06F0 21%, B17E0 20%, B07F0 7%, A29C0 6%, B0078 5%, B0020 4.5%.
- Fields 5000-6400: B0078 35%, B1638 25%, B06F0 16%, B17E0 14%, B07F0 5%, A29C0 3%.
- The same programs as in the race, in a different mix. B06F0, which has no kernel, is
  16-21%.

**Per entry, the parts worth naming** (race, share of VU1):
- **B0078's continuations** at 1040 and 1CB0 (dispatch 0x11 to the 0170 loop) are 28.7% and 7.4%.
  - 77% and 73% of their time is in the native kernel and its clipping.
  - The rest is the shared setup at 1AE0 and the GIF tail at 0FF8.
  - 4.9-5.1 clipping-routine runs a call. 38.5 and 42.0 vertices a call.
- **The MSCAL setups that only `B L1AE0`** (0078, 06F0, 07F0, 1638, 17E0, 12F0, 1050, ...) take
  0.7-0.9 us each and together about 5%.
  - 60 pairs and 52 MAC updates a call, 98% dead.
  - 46% of their samples are flag work.
- **B0040's setup** (MSCAL 0040, 11E8: a lighting/matrix loop at 1280) is 3.6%. 546 pairs and 426
  MAC updates a call, no kick.
- **B0040's MSCNT at 12E0** (`JR vi13`):
  - vi13 0x20C (loop 1110): 2.7%, 11.3 us a call;
  - vi13 0x260 (loop 13A0): 2.4%, 6.2 us;
  - vi13 0x616 (loop 3140): 0.6%.
  - The kernels for 0x20C (`native_geometry_20c`) and 0x260 (`native_geometry_260`) exist, but
    `run()` selects them only when the entry is 1040 or 1CB0.
- **B06F0 through 1040 and 1CB0** (vi13 0x0E0, loop 0798-07D8) is 5.8%, plus 1.3% for its setup.
  - 9 pairs a vertex: the matrix transform, DIV, MULq projection, FTOI4 and stores.
  - No flag reads, no clipping. 7.4 MAC updates a vertex, 99.5% dead.
- **Loop 3140 (vi13 0x616)**, through 0040, 0020 and 30A0 at 1040 and 12E0, is 3.8%.
  - 8 pairs a vertex, no perspective divide in the loop. No flags read, no clipping.
- **Skinning tails** (0020 and 0010 at 0590, after `native_skin_468`) are 1.8%. 480 MAC updates a
  call, 99.8% dead.
- **A29C0** is the particle generator, 2 calls of about 39-65 us and 333 sprites each.
  - It runs a recurrence (`MADD vf13, vf13, vf23` with ACC = vf22, `RNEXT` random numbers).
  - It culls with FMAND on the MAC sign and zero bits, and writes a variable number of sprites
    (SQI).
  - Of all VU1 programs it reads the most MAC flags: 909 observed updates a field.

### Flags: who reads them

Over the race (per field):
- **Reads.** 5,313 MAC reads, almost all inside `native_clip_run` (FMAND at 1970) and overlay A.
  457 status reads, all of D/I (FSAND 0x020 after DIV). 9,292 clip reads.
- **No reads of the status ZSUO bits** (from FMAC results) and **no sticky-bit reads** at all.
- **No program ever reads a flag left by an earlier program**: 0 reads of entry-state MAC or
  clip, in all windows.
- **VU0 never reads the VU1 register file** (0 `host_vu1_file` calls). The EE has no access to VU1
  registers, and no game function addresses VU1 memory (0x11008000/0x1100C000 appear nowhere in
  the asm).
- **VIF1 reads only TOP/ITOP/TPC**, never a flag.
- So the end-of-run MAC, status and clip registers matter only to `state_trace`'s VU1 snapshot.
  They must stay exact for that check, but no instruction reads them.
- **MAC updates.** 112,088 queued in the AOT path. 923 observed, all in overlay A: 29C0, 2E48,
  1010, 26D8 and 1240. 560 are live at the end. **98.7% are dead.**
- **Clip updates.** 1,452 a field (CLIP + FCSET) in the AOT path. All are read or overwritten.
  Clipping is real work, not dead work.

Finding 030's conservative pass found no removable complete flag update. That still holds: every
FMAC sets sticky status bits, and the end state is exact. The saving is in how the updates are
made, not in deleting them:
- Today each FMAC computes the MAC (`fmac_flags4`: 2 movemasks and 2 table lookups) and queues a
  16-slot ring entry. That is 6 stores plus the status derivation. `retire_flags` later walks
  the ring.
- Where no MAC/status reader is reachable before the program ends (every overlay-B program
  outside the clip routine), the generated block could do what the native kernels already do:
  - OR the sign and zero masks into two sticky accumulators;
  - keep the last result and its dest mask;
  - build MAC and status once, at the end (the drain) or at a reader.
- With no reader in reach, the 4-cycle flag latency doesn't matter. The final registers, and
  so the oracle's and the state trace's end state, stay identical.

**(a) Estimate.**

| where | today (quiet ms/field) | removable |
|---|---|---|
| AOT blocks: `queue_flags` + `fmac_flags4` inline (41% of 0.83 ms), plus out-of-line `retire_flags` (0.12) | 0.46 | about 0.35-0.40 |
| `native_clip_run`: `fmac_flags4` per FMAC and the clip commits (40% of 0.55 ms) | 0.22 | about 0.12 (MAC is needed only from 1950's SUBA, read at 1970) |
| native kernels: `native_result` sticky accumulation | 0.15 | about 0 (already the cheap form) |
| total | 0.83 | **about 0.45-0.5 ms (12-14% of the vu thread's 3.86 ms)** |

- By program, the AOT part goes mostly to B0040 (0.16), B06F0 (0.09), B0078 (0.05: the 1AE0 setup
  and the tail), A29C0 (0.04), B0020/0010 (0.04) and B07F0 (0.02). See the table's last column.
- Once the missing kernels below are in, 0040's and 06F0's share moves into kernels, which
  already use the cheap flag form.
  - a reachability pass over each overlay in `vu1_aot.py` (readers FM*/FS*-ZSUO, and the 1970
    FMAND for the clip code);
  - a "lazy" `commit_upper` variant and a drain that builds the end flags;
  - the same change in `vu1_native_clip.py`;
  - checks: the PCSX2 VU1 oracle (`test_vu1_pcsx2.py --aot`) and `vu1_seq_diff.py` on three race
    captures.
- **Overlay A keeps the queue** in 29C0's loop, 2E48, 1010, 26D8 and 1240.

### Programs that miss a native kernel (cheaper than (a))

- **12E0 entry (measured).**
  - `DHD_OP3_12E0=1` adds `addr == 0x12e0` to the native-kind selection in `run()`. VI13 0x11
    keeps kind 0 there.
  - B0040@12E0/20C went from 11.28 to 2.96 us a call, and /260 from 6.20 to 1.86 us. The
    kernels took 47% and 42% of those runs.
  - That is -0.167 ms a field on this run (-3.7% of VU1; about -0.11 ms quiet). No "native
    geometry failed" in the log.
  - Exactness rests on the kernels being entry-independent: they start at the loop's first pair
    from the live pipeline state, as at 1040. Before shipping it needs `vu1_seq_diff.py` over
    captures that include 0040's runs. About 1 h.
- **Loop 0798 (B06F0, vi13 0x0E0).**
  - 7.1% of race VU1, 16-21% in the menus.
  - The unlit, unclipped sibling of 07F0's 08B0 loop. That loop's kernel (`native_geometry_800`)
    runs 3.5x faster than its blocks (finding 046).
  - Estimated -4% of race VU1 and -10-14% in menus. 3-5 h.
- **Loop 3140 (vi13 0x616).** 3.8%, similar shape. About -2.5%, 3-4 h.
- **Skinning tail 0590.** 1.8%. Mostly setup and the GIF tail. Low priority.

### (b) VIF1 UNPACK

The race sends 11,000 UNPACKs and 186,000 vectors a field. All are mode 0 and WL <= CL, so all go
through `unpack_stream`; none takes the filling path.

| form (CL/WL) | commands/field | vectors/field | vectors/command | ms/field | share | ns/vector |
|---|---|---|---|---|---|---|
| V2-16 masked (3/1) | 1226 | 43669 | 35.6 | 0.170 | 25.5% | 3.9 |
| S-8 masked (3/1) | 4076 | 9074 | 2.2 | 0.117 | 17.5% | 12.9 |
| S-8 (3/1) | 1223 | 43645 | 35.7 | 0.106 | 16.0% | 2.4 |
| V4-5 (3/1) | 947 | 33162 | 35.0 | 0.090 | 13.5% | 2.7 |
| V3-32 USN (4/4) | 311 | 20586 | 66.1 | 0.047 | 7.1% | 2.3 |
| V3-8 (4/4) | 110 | 11314 | 102.9 | 0.037 | 5.6% | 3.3 |
| V4-32 (4/4) | 1123 | 8881 | 7.9 | 0.033 | 4.9% | 3.7 |
| V4-8 USN (3/1) | 276 | 10483 | 38.0 | 0.031 | 4.6% | 2.9 |
| V4-32 (3/1), one vector | 1223 | 1223 | 1.0 | 0.021 | 3.2% | 17.5 |
| S-32 (4/4), one vector | 323 | 323 | 1.0 | 0.006 | 0.9% | 18.4 |
| 10 more forms | 53 | 3100 | | 0.007 | 1.0% | |

The ns/vector figures include the counters' two rdtsc and a key lookup per transfer chunk, roughly
10 ns each (about 0.1 ms a field), so the 1-2-vector rows are inflated by up to half.

Worth specialising, in order:
1. **The masked forms with WL = 1** (V2-16 masked, S-8 masked).
   - With WL 1 every vector uses mask row 0, so the mask becomes one constant per-lane select
     between data, row, column and the old value.
   - That is a 4-lane blend instead of the per-field `switch`. About 1 ns a vector.
2. **Short commands** (S-8 masked at 2.2 vectors, V4-32 and S-32 with one vector: 5,600 commands a
   field).
   - Skip `stream_form`'s 16-entry mask table and the row/column copies when the count is small or
     the command is unmasked.
   - Decode the mask once per STMASK rather than per UNPACK.
3. **S-8, V4-5 and V4-8 at stride 3** (CL 3 WL 1). Widen 4-8 vectors per step with SSE (byte to
   lane broadcast, the 5:5:5:1 shifts).
4. V3-32 and V3-8 at CL 4 WL 4 are already near memory speed. Leave them.

**Estimate.** About 0.25 ms a field off the vu thread: 40% of UNPACK, plus some of the 0.3 ms of
command handling. 4-6 h, with `test_vif.c`'s 200,000-stream fuzz as the check. x64 and browser.

### XGKICK pattern and batches

- **The mesh family kicks once per batch,** at the GIF tail 0FF8. The packet is one tag with
  PRE = triangle strip and NLOOP = vertices, and the run toggles the TOPS double buffer. 1.0-1.3
  kicks a run.
  - About 2-5% of runs (more in 2DC8: 1.3-1.95 kicks a run) have extra kicks from the clipping
    routine. It emits each clipped polygon as a triangle fan. Fans are 4% of the mesh family's
    primitives.
- **Batch sizes.** 30-42 vertices a kick on average. 44 is the usual maximum; a few batches reach
  52-100. The kernels decline over 64, and for 07F0 over 341.
- **Overlay A.**
  - 2E48: 6.2 kicks a run of about 73 sprites, 454 a run.
  - 0E10: 2.8 kicks, 178 sprites a run.
  - 29C0: 1 kick of 333 sprites.
  - 26D8: 18 kicks of register-only packets.
- **Per field:** 1,431 kicks, 51,100 vertices and 155,000 qwords in the race. The copy into the GS
  ring is 0.38 ms (finding 089: 0.9% of samples are the kick itself).

### (c) A GPU path for one program

**Candidate: the B mesh family's per-vertex loop, with 0078/0170 first.**
- 0078's 0170 loop is 36% of race VU1 by itself. With 1638's 1708, 2DC8's 2F00, 17E0, 1430 and
  the three loops without a kernel (0798, 3140, 1110/13A0 via 12E0), the family is 91%.
- Each loop is the same pipeline:
  - 3 input qwords a vertex (from the UNPACKs above);
  - a 4x4 transform, DIV/Q, the perspective multiply;
  - a CLIP judgement against w times the guard band;
  - FTOI4 to XYZ2 with the ADC (no-kick) bit, plus ST/Q and colour;
  - 3 output qwords.

**Dependencies.**
- **Vertex-parallel, except for a sliding window.** Within a batch, no vertex reads data written
  while an earlier vertex was processed.
  - The software pipeline carries the previous vertices' positions and clip codes (`v[10]`,
    `v[28]`, `v[29]` and the 24-bit clip shift register).
  - The backface test (a cross product with the previous two positions) and the clip test
    (FCAND 0x02FBEF: the current judgement and the two before it) are functions of 3
    consecutive vertices.
  - So: a pass per vertex, then a pass per strip triangle.
- **Clipping is the sequential part.** When the window test fails, `native_clip_run` (1948/1CC0)
  clips the triangle and writes and kicks a fan.
  - That happens 4.9-21 times a batch (0078: 4.9, 1638: 11, 2DC8: 19-21). It is 14% of the vu
    thread's samples.
  - On a GPU this is a second compute pass with an append buffer. To keep the native renderer's
    blend order, its output needs a prefix sum per batch, or the fans drawn after their strip
    with a per-batch indirect draw.
- **Flags:** only the clip judgements (vertex-local plus the window) feed the output. MAC and
  status have no reader, as shown above.
- **Nothing the CPU reads.** No EE, VU0 or VIF reads of VU1 output, registers or flags.
  Everything goes to the GS.
  - The GIF tag and A+D register writes (TEX0 and the like, 0.3-4 a run) come from the
    per-batch header, not from the transform. They can stay on the CPU and feed the GS state
    machine as now.

**Precision.** The native renderer could run the transform in plain IEEE floats on the GPU.
Positions would differ from the VU's toward-zero, clamped results by about an ulp, below a
pixel. But two consumers need exact VU1 output:
- the exact renderer (`DHD_RENDER=exact`), which must keep the CPU path;
- the **lens-flare Z probe**, the one GS result the game reads (native_core.inc). It needs exact
  Z for every Z-writing primitive inside the watch rectangle, a replay log for camera cuts, and
  whole exact frames while the probe is near the bottom edge.
  - So a GPU path still needs, per batch, a conservative screen bound on the CPU: a plain-float
    transform of the batch's bounding box, cheap next to exact VU1.
  - It also needs the exact CPU VU1 for every batch that may touch the rectangle, and for whole
    fields in the band.

**Feasibility and gain, roughly.**
- **Work.** WGSL kernels for about 8 loop variants and the clipping routine, a batch upload from
  VU memory (the unpacked data, 3 qwords a vertex), and indirect draws. Then GS-state plumbing
  around VU1-produced primitives (they no longer pass `gs_write`/`native_record`), the Z-probe
  bound and fallback, and the exact-mode split. 100-200 h, high risk.
- **x64.** It would take about 2.5 of the vu thread's 3.9 ms, and part of the GS thread's
  `gs_write`/`vertex_kick`/`native_record` for these primitives.
  - The vu thread is not the x64 limit until finding 089's item 1 (the alias fold) lands.
  - (a), the missing kernels and (b) together take about 0.9-1.0 ms off it for about 25-35 h.
- **Browser.** VU1 is 7.2 ms of the graphics worker's 16.3 ms (finding 089). There a GPU path is
  the one item big enough for 120 fps. For 60 fps the cheap items here, plus 089's worker
  split, are enough.
- **Bad candidates.** A29C0 (a recurrence with random numbers, MAC-flag culling and variable
  output) and overlay A's particle programs: small shares, sequential or already native.

### Raw data

- `runs/{stats1,stats2,stats3,k12e0}/`: game.log, op3.txt (the counters), summary_w*.txt
  (`op3stats.py`), table_w*.md (`final_table.py`), keyflags_w2.txt and timeline.csv. stats1 has
  only the first menu window (its race window ended at the bench exit).
- `pr1_vu_samples.{txt,json}`: finding 089's vu-thread samples by kind, unit and block.
- `tools/`:
  - `run.sh`: the hidden-desktop run;
  - `vuprof.py`, `keyflags.py`, `op3stats.py`, `final_table.py`;
  - `patch_vif.py`, `patch2.py`, `patch3.py`: how the local build was patched.
- `op3-instrumentation.diff`, `op3_vif.h`, `op3_vu.h`: the local counters.

Sources: findings 029 and 030 (the earlier flag-liveness pass, the AOT design), 046 and 051 (the
native kernels, `DHD_VU1_PROFILE`, `DHD_VIF_STATS`), 089 (the profile and its samples);
port/gs/native_core.inc (the Z probe); PCSX2 microVU's `mVUsetFlags` (flag liveness that skips
unread flag instances, the model for (a); no code taken).

## OP2: VU0's code-signature compare and the per-field getenvs

Finding 089's item 6, and the facts behind the cheap part of its item 2. Branch `rr/OP2`.

### The VU0 signature compare

`ps2_vu0_run` (port/runtime/vu0_interp.c) runs five vcallms entries through native routines (finding
066: 0000 cull, 02E0 bounds, 05E8 basis, 07B8 length, 0808 direction) only where VU0 micro memory
holds the exact words they were written from. It checked that with a `memcmp` of 192, 528, 376, 80
and 40 bytes on every call of those entries. In the core module that `memcmp` is wasi-libc's byte
loop (`w2c_core_memcmp`: one `i32_load8_u` per byte, each with the guest-memory alias fold on x64),
so a call cost several hundred byte loads: 4.5% of the x64 EE thread and 1.7% of the browser game
worker in finding 089. The two shorter compares were inlined by clang; the three long ones were
calls.

Now each routine's answer is kept with the code version it was computed for, as VU1's
`native_code_matches` and `aot_find` already do with `ps2_vu1_code_version`:
- `uint64_t ps2_vu0_code_version` (vu0_interp.c, declared in ps2asm.h).
- `vu0_native_code(cpu, which, at, words, bytes)`: for the game's micro memory
  (`GADDR(cpu->vu0_code) == 0x11000000`) the `memcmp` runs once per version and routine; any other
  code buffer (the replay and oracle tests: vu0_native_diff.c, test_vu0_native.py, test_ps2asm.c's
  static buffer) is compared on every call, as before.
- `DHD_VU0_CODE_CHECK=1` also compares on every call and prints
  `vu0: micro memory at XXXX changed without VIF0 MPG` if the kept answer was wrong (then uses the
  fresh one). Off, it costs one static test.

**Every path that writes VU0 micro memory, and how it invalidates:**
1. **VIF0 MPG** (port/hle/vif.c `vif_feed`, compiled into the core as vif0.c's
   `module_vif0_feed`, reached from dma.c's channel-0 transfers). It already compared each word
   before writing it; a changed word now also bumps `ps2_vu0_code_version` (`if (!unit)`, next to
   VIF1's `ps2_vu1_code_version++`). This is how the game loads its VU0 package (0x26CF80).
2. **The signature words themselves** (`ps2_vu_aot_load`, which fills `vu0_*_words` from the
   image's .vutext at image load): it bumps the version too, so an answer computed before the
   words were there (all zero; a zero first word never matches) is not kept.
3. **EE stores to 0x11000000-0x11000FFF.** The hardware allows them, and nothing in the core could
   see them, but the game does not make them: every `lui 0x1100` in the game's code builds a VIF
   code (FLUSH 0x11000000 in DMA packets: 00215B68, 00216BB8, 00218B80, 002249E0, 00224B88, 00226588,
   002286F8, 0022E560, 0022E9A0, 002370F0, 00237918, 00242F10) except 0021CB98, which stores
   to VU0 *data* memory (0x11004000); the one data word 0x11000000 (00283604) sits in a table of
   hardware address ranges (next to 0x1000C000, 0x1000E000, 0x12000000). The inline asm (ps2_asm) and the VU0 macro mode never address
   micro memory. `DHD_VU0_CODE_CHECK=1` is the check for this claim.
4. **Nothing else**: DMA from or to the scratchpad copies RAM and SPR only; VU1, the GS and the
   host never write VU0 memory; the state trace and the recorders only read it; memory.c zeroes it
   once when it commits the page, before the first vcallms.

The generator comment in port/tools/vu1_aot.py ("VU0 code has no version counter") is updated; its
output is unchanged.

### The getenvs read every field

Finding 089 found `common_getenv_nolock` at 0.5% of the x64 EE thread, called through
`w2c_env_host_getenv`: the core's `getenv` is the `host_getenv` import (runtime/wasi_adapter.c),
which on x64 is the CRT's `getenv` (a case-insensitive scan of the whole environment) plus the
bridge's value interning, and in the browser a call out to JavaScript. Getenvs that ran on every
field or every pad read, uncached:
- online.c `choose_view` (every field): DHD_VIEW; `pads_read` online: DHD_NET_SCRIPTED.
- kernel.c `vsync` (every field): DHD_SCREENSHOT.

These now go through `hle_env(&cache, name)` (port/hle/hle.h): the value is read once and again only
after the core changed its environment: every `host_putenv` of the core is followed by
`hle_env_changed()` (online.c's `set_env`, DHD_NAV and DHD_RACE_SEED at an online race's start and
end; online_menu.c's `save`, DHD_NET_NAME; pc_settings.c's rows; widescreen.c's aspect). Of the
cached variables only DHD_NAV changes at run time. The host's own writers (present.c's
DHD_MONITOR_MODE and window size, host.c's settings and DHD_ISO before the core starts) set none of
the cached variables. The returned pointers stay valid: the x64 bridge and the browser's `intern`
keep every value they hand out.

Left as they are: host.c's `host_pad_vibrate` reads DHD_VIBRATION on each motor change (the PC
settings page changes it live; host side, not every field), present.c's capture settings (render
thread), and the reads already behind a static.

### VU0 blocks in the core module (finding 089 item 2, cheap part): not switched on

The facts:
- The core compiles vu0_interp.c with `-DPS2_VU0_NATIVE` only (build_wasm.py): the five native
  routines and their word arrays (vu0_native_words.py), not `-DPS2_VU_AOT`. The `aot0_*` blocks
  and the whole AOT dispatch in `run()` are inside `#ifdef PS2_VU_AOT`, so the core does not even
  contain them. `PS2_VU_AOT` pulls in all of vu1_aot.inc: 718 VU1 blocks (55,000 of its 62,000
  lines) plus the VU1 native kernels (vu1_native_clip.inc, vu1_native_geometry.h,
  vu1_native_extra.h), dead code in the core, where VU1 never runs. The VU0 part is 53 blocks,
  about 6,200 lines.
- Where the blocks are compiled (the host's VU copy, the old i686 build) they are off by default:
  `DHD_VU0_AOT=1` turns them on. Findings 051 and 066 proved them exact on i686 (vu0_aot_diff.c:
  104 captures, 13 entries, 936 and later 3,432 full-state comparisons; the race gates MATCH with
  `DHD_VU0_AOT=1`) but kept them opt-in because the EE was not the limit there (EE busy -0.2 to
  -0.35 ms a field, wall time unchanged).
- `aot0_find` compares each block's words on every dispatch with `memcmp`; in the core that is the
  same byte loop as above, for every block of every interpreted program.
- They were never checked in the wasm core: the core's float operations are wasm's (the exact VU
  rounding comes from the float layer, not MXCSR), and vu0_aot_diff.c has no wasm build.

So it is not only a build switch, and nothing was changed. What switching them on would take:
1. vu1_aot.py writes a VU0-only include (the 53 `aot0_*` blocks, `aot0_find`, the word fills),
   and vu0_interp.c gets a `PS2_VU0_AOT` mode that compiles the VU0 half of the AOT dispatch in
   `run()` without the VU1 kernels; build_wasm.py builds the core with it. 2-3 h.
2. `aot0_find` keeps its answer per pc and `ps2_vu0_code_version`, as `aot_find` does for VU1
   (the version from this section makes that possible). 1 h.
3. Exactness in the core: vu0_aot_diff.c run against the core's own build (a wasm or wasm2c
   harness over the 104 captures and seeded variants), then a state-trace race compare with the
   default on. 3-5 h.
4. The default: on in the core, `DHD_VU0_AOT=0` to turn it off. With a short timing pair.
Estimate 6-10 h in all, as finding 089 says. Expected gain: the blocks remove the interpreter's
decode and dispatch for 71% of VU0 time, not the float work; from the i686 numbers and the share
here, about 0.3-0.8 ms of the x64 EE's 7.2-8.0 ms, and somewhat more in the browser.

### Checks

Builds, after merging `render/rust` 2979bc0b2 (CL6 and OP3 in): `build_wasm.py -j 2` (offset scan
clean), `package_release.py --wasm build/wasm/dhd_core.wasm --jobs 2` (dhd.exe sha256 8b28d259...),
`port/web/build.py --renderer rust --no-threads` and the threaded default, each followed by
`check_web_layout.py build/web`: OK.

**State trace** (`state_trace_run.py race`, 5000 fields, GO at 3214, the same input as the base;
the new build also with `DHD_VU0_CODE_CHECK=1`, which never fired):
- Before the CL6 merge, against the CL7 release exe (8a824262...): all 5000 rows match.
- After it, against render/rust 18c8f153e built the same way (992bbcb3...): 2,743 rows differed
  in `ram` (fields 25-27, 143-1112, 1444-3213; from GO on they match) and 6 in `cpu` (143-148).
  Every other section (scratch, hw, vu0, gs_stream, gs_regs, gs_vram, sound) matched all 5000.
  Dumps (fields 25, 143, 2000) and `DHD_STATE_WATCH` on the words found two host-layout values
  in game memory, the kind state_trace_mask.txt already lists:
  - 0x0093C628-0x0093C637: the t0 and t1 slots of func_00254D68's (sprintf) vararg spill, next to
    the already masked 0x0093C620. func_00208900's sound-bank message passes one argument, so
    t0/t1 still hold what strcat (func_002542C4, copying through func_00254544) left from
    appending the module's `";1"` literal: base 0x70 / 0x088AEE38, this build
    0x0101010101010101 / 0x3263666D7100313B (the word loop's constant, and `";1\0qmfc2"`: the
    word read past the NUL into the next .rodata bytes, an asm template). The literal is the
    tail of the merged string `"...LIBSD.IRX;1"`, followed by `"qmfc2..."` in both builds, at
    0x70104A45 in the base and 0x70104A58 in this build (read from the cores' data segments):
    func_00254544 (strcpy) copies word by word when source and destination are both 8-byte
    aligned and byte by byte otherwise, so the registers it leaves follow the literal's alignment.
  - 0x00760A10: a SIF RPC word that func_0025CF88's HLE call sets to an entry of sif.c's .bss
    `pending` and sif_poll clears: 0x7028DC44 against 0x7028DE34, the .bss moved by 0x1F0.
  A build without the second commit (pc_settings.c/widescreen.c) stores the same values as this
  one: they follow the data layout of commit 1 plus CL6, not a behaviour change. The five words are
  now in state_trace_mask.txt with their reasons. With the mask, rerun: `ram` and every other
  section match all 5000 rows. Left: `cpu` in fields 143-148, the same strcat leftovers still in
  t0/t1 at the end of those fields (a later sound-bank load); the mask covers RAM words only, and
  the registers are rewritten before field 149 with nothing else differing.
- After merging render/rust b739ef49f (OP1 in), against OP1's exe from the same port/ and src/
  (4c3753ec9, e0713288...), with the mask: the same result, every section but `cpu` 143-148
  matches all 5000 rows (the same two hashes as above); `DHD_VU0_CODE_CHECK=1` never fired.

**EE time** (PR1's recipe and launcher: hidden desktop, scale 8, 144 steps a second, full-pedal
race, `DHD_BENCH=7900-10900`, sound off; one run each, base then new; the PC quiet of builds, CPU
21% before and 46% after, other users' load not excluded):

| build | fields/s | p50 / p99 ms | EE (game) busy ms/field | VU | GS |
|---|---|---|---|---|---|
| render/rust 18c8f153e | 113.2 | 8.68 / 16.75 | 8.25 | 5.30 | 4.57 |
| OP2 | 120.1 | 8.04 / 14.15 | 7.73 | 5.14 | 4.38 |

EE busy -0.52 ms a field (-6%), in line with the profile's 4.5% for the memcmp plus 0.5% for the
getenvs. One pair on a shared PC: finding 089 saw single runs move by up to 15%.

## OP1: guest-memory alias fold out of the hot path

Branch `rr/OP1` (from `render/rust` 84cb6af61, render/rust 2979bc0b2 merged). x64 (and the Linux
host, which shares the header) only: the fold lives in the wasm2c memory layer
(`port/host64/memory_access.h`, applied by `port/tools/prepare_wasm2c.py`), not in the core C, so
`dhd_core.wasm` and the browser build are unchanged (the browser's own pass, BS3's `inline_memory`,
is untouched).

### What changed

1. **Fold out of line (0a6dd9d0e).** The full adapter (window test, crossing barrier, mirror
   fold) is unchanged. It is now inlined only for compile-time constant spans (it folds away) and
   otherwise called out of line (`noinline, cold`). Scalar/SIMD accesses (`MEM_ADDR_MEMOP`,
   1..16 bytes) use `host64_access_address`: one bit test of the address's 32-MiB window, mod 64,
   against a set of nine windows: the five alias windows and the window below each upper one,
   where a span may start and cross into an alias. An address the compiler proves at or above
   0x3a000000 (module data reached through a large memarg offset) has no test.
2. **One test per base value (d9817ee8a).** wasm2c writes every access as
   `NAME_default32(..., (u64)(base) + offset, ...)`. `prepare_wasm2c.py` rewrites each site to
   `host64_base_ok(base, offset) ? NAME_hot_default32(...) : NAME_cold_default32(...)`, and adds a
   `_hot` and a `_cold` copy beside each WABT access template. The hot copy addresses
   `base + address` with no test of its own. The cold copy calls the full adapter. The test reads
   the *base's* window (valid for offsets up to 32 MiB - 16), so every access through the same
   base value shares one test. Clang merges the diamonds and keeps plain addressing modes and
   alias analysis on the hot path. For offsets at or above 0x3a000000 the test is constant true.
   All 201,428 access sites of the module have this shape; `split_accesses` refuses any other.

### Why it is safe

- The fold only exists so that clang never sees two different addresses for one location. The
  OS maps every view onto the same pages (finding 082).
- Outside the six RAM views every address maps uniquely. Canonical RAM is one of the views.
  An address that touches no view other than canonical RAM is its own canonical address, so the
  full adapter returns `base + address` for it.
- The hot path is taken only where that holds:
  - The base's window w is outside the slow set, and offset + size <= 32 MiB. So the span lies
    in windows w and w+1, and neither holds a non-canonical view.
  - A span crossing w to w+1 joins two uniquely mapped ranges, so it needs no barrier.
  - Windows are taken mod 64. Every in-bounds address is below 2 GiB (the module's maximum
    memory). Beyond that the full adapter folds nothing.
- So every access gets the same host address as before, and alias analysis on the plain hot
  addresses only ever relates uniquely mapped locations. Alias spans, constant alias addresses,
  crossings near alias windows and bulk operations still go through the full adapter.
- Nothing is skipped on an unproven guess. The test is still at run time; it is only shared per
  base value.
- `selftest_host.c` checks the new adapter, `host64_base_ok` and the cold arm against the full
  adapter at every 4-MiB edge up to 8 GiB, for sizes 1..16 and offsets of each shape.
  `host64_selftest.py --optimized --negative-controls` passes; the prepared self-test module has
  174 split sites.

Not done: compile-time proofs per base register (sp-, $gp-relative). At the wasm2c level the guest
registers are loads from the module's `ps2.gpr` array. Their values can't be proven canonical
without a whole-program invariant. With the per-base test the remaining cost is about one bit test
per base value, so there is little left to gain there.

### Results

| | dhd64.exe | EE busy ms/field (2 runs) | fields/s |
|---|---|---|---|
| base (render/rust 84cb6af61) | 45.7 MB | 9.55, 10.49 | 98.8, 90.8 |
| step 1 (out of line, bit test) | 26.2 MB | 7.67, 7.84 | 115.6, 112.8 |
| step 2 (per-base test) | 24.9 MB | 6.06, 5.78 | 138.1, 141.9 |

These are finding 089's x64 runs (`hidden_run.py` on a hidden desktop, full-pedal race,
`DHD_FPS=144`, scale 2, sound off, `DHD_BENCH=7900-10900`), alternating base, step 1, step 2 and
then in reverse. The PC was shared: CPU load was 29-72% before the runs, so base ran slower than
in finding 089 (8.3 ms). Relative to base in the same rounds:
- step 1: -20%;
- step 2: -40%, about finding 089's no-fold probe (5.0 ms on a quieter PC, also -40%).

VU and GS busy also fell (VU 6.0-6.4 to 5.1, GS 5.1-5.5 to 4.3-4.4), which is probably less
contention for the cores.

### Checks

- `build_wasm.py -j 2`: offset scan clean.
- `package_release.py`: ok.
- Both web builds (threaded and `--no-threads`): ok, and `check_web_layout.py build/web` ok.
- State trace, `state_trace_run.py race` (5000 fields, through the race start): base exe against
  the new exe, `trace_columns.py` PASS, all 13 columns identical. It was run twice, before and
  after merging render/rust 18c8f153e.
