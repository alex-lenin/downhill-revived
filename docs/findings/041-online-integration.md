# 041 — Online races in the port: four pads, lockstep, full-screen view, launcher

2026-10-16. Step 3 of `docs/ONLINE-PLAN.md`: the client module (`port/net/`, finding 042) wired
into the game, following finding 040's recipe, with the relay of finding 039.

**Dropped the same day.** The owner replaced input lockstep with state sync (finding 044): every
PC runs its own one-player race and the other riders are puppets. The race path below
(lockstep, state hashes, the 4-pad split screen online) is gone from `port/hle/online.c`; the
lobby, launcher, package, split-screen data fix, `DHD_WATCH` and the stack-struct fix stay.

## What is there

| Where | What |
|---|---|
| `port/hle/pad.c`, `iop.c`, `sif.c` | A multitap on port 0: slots 0-3, `PortOpen`/`GetConnection`/version handlers (0x80000901/903/905). Player i reads pad 2i (port 0, slot i), each slot from its own source (`pads_read`). |
| `port/hle/online.c` | Everything online: pad sources, race setup, the barrier, lockstep, state hashes, the view choice. |
| `port/hle/host.c` | `host_pad_script` / `host_pad_device` split, fiber stack ranges, `DHD_WATCH` (a diagnostic hardware data watchpoint). |
| `port/hle/kernel.c` | `kernel_pace_slip`: the field clock slips by the time spent waiting for other PCs. |
| `port/gs/gpu_native.inc`, `present.c` | `DHD_VIEW`: one quarter of the split screen, scaled to the window (present only). |
| `port/patches.py` | `001A4F70.c`: `offset` zero-initialised (below). |
| `port/online/` | The launcher (`Play-online.cmd`, `Online.ps1`), the friends' README and the settings template. |
| `port/tools/lockstep_local.py` | E4: two instances, same scripted pads, different configurations, hash per 60 fields. |
| `port/tools/online_race.py` | Two game instances and two `nettool --bot` players through the relay. |
| `port/tools/package_online.py` | The zip for friends (`build/dhd-online.zip`). |

## Settings (environment; the launcher sets them)

| Variable | Meaning |
|---|---|
| `DHD_PADS=1..4` | Offline: how many pads are connected (multitap when more than 2). Online: always 4. |
| `DHD_RACE_SETUP=track,r0,r1,r2,r3` | Offline: track and rider indexes written into the setup block at the load screen. |
| `DHD_NET_SERVER` | The relay (from `online-settings.txt`; never in a tracked file). |
| `DHD_NET_CREATE=1` / `DHD_NET_JOIN=CODE` | Host a room / join one. |
| `DHD_NET_NAME`, `DHD_NET_RIDER`, `DHD_NET_TRACK`, `DHD_NET_PLAYERS`, `DHD_NET_PUBLIC`, `DHD_NET_ROOM_NAME` | Lobby choices. `DHD_NET_PLAYERS=n`: the host starts when n are in and ready; otherwise on START/Enter. |
| `DHD_NET_ISO_SHA256` | The ISO hash the server compares (the launcher computes it). |
| `DHD_NET_SCRIPTED=1` | Tests: this PC's slot comes from `DHD_RACE_PAD<slot>` instead of the controller. |
| `DHD_VIEW=0..3` / `all` | Full-screen view of that player in 3-4 player races. Online the default is this PC's player. |
| `DHD_HASH_LOG=FILE`, `DHD_HASH_DUMP=field` | Hash every 60 fields; dump the hashed memory at one field. |
| `DHD_WATCH=addr[:rw]` | Diagnostic: hardware data watchpoint on one EE word; logs writer, field and value. |

## How a race starts

1. Both PCs boot with the same menu script and stop at the player-count menu (menu 10): the
   barrier. The host's game sends the room settings, every game sends its pick and ready.
2. The host starts; every PC leaves the barrier at the server's `startAt`. Field 0 of the
   lockstep is the first field after it. From here every PC runs the same fields.
3. The rest of the menus are scripted by the player count, the setup block (track, level
   name, riders) is written on the load screen, `DHD_RACE_SEED` is the room's seed (never 0).
4. From the countdown (menu 9, phase 1) each slot's pad comes from `net_inputs_for_field`.
   Before that the script drives all four pads the same way on every PC.

Lockstep per field (`lockstep_field`): submit the hash of field F-1 when due, sample this PC's
pad, `net_submit_local(F)`, wait for all four slots (window title "waiting for NAME"), then the
field clock slips by the wait (finding 042's pacing rule).

## Disconnects

A slot whose player left gets the AI kind from the server. The game holds that slot neutral
(the rider stops). It does not hand the rider to the game's AI (rider +0x7A5C = 3): in a 3-4
player race the game sets up no AI controller for human slots, so switching the type byte
would run AI code on uninitialised state. Neutral is deterministic on every PC.

## Host values in EE memory (DHD_WATCH)

Two runs of the same build with the same pads gave EE memory that differed in words holding
host addresses or host stack bytes. Finding them: `DHD_WATCH=addr` sets a hardware data
watchpoint (DR0/DR7 through `SetThreadContext` from a helper thread, a vectored handler for
`EXCEPTION_SINGLE_STEP` that logs the writing function, the field and the value). Results:

- **Rider 0 +0x535C (EE 0x2B461C), `obj->xDB0.w`.** Written once (field 637, loading) by
  `func_00221888`, a VU0 macro routine that loads and stores all four lanes (`lqc2`/`sqc2`),
  called from `func_001A6D00` (`001A4F70.c`): `Sub__c10(&offset, ...)` sets x, y, z of a stack
  `Vec__c10 offset`, never w, so host stack bytes went into the rider. No read of the word in
  80 s of race with a read/write watchpoint, but it sits in the rider block the race hash covers
  and a later version could read it. Fixed: `port/patches.py` zero-initialises `offset`
  (as finding 034 did for other stack structs). After the fix the race hash matched where it
  had differed before (E4 below).
- **Menu stack (`D_003E3BD0`, e.g. EE 0x3E3C34).** `func_00187048` (from `func_001881C0` /
  `func_001882C8`) pushes the address of a stack local (`pair`) and uses it during the call:
  on the PC that's a host stack address that depends on where Windows put the fiber stacks.
  The game reads it only as a pointer, during that call. Harmless; the hashes leave host stack
  ranges out (`host_stack_ranges`, recorded per fiber).
- Other differing words in the static area (0x5F0000-0x630000, 0x862000+) were saved fiber
  stack addresses of the same kind; masked, the whole 32 MB matched at field 660.

The online hash (`game_hash`) covers the race-deciding state of finding 040: the rider world
(0x2AF2C0, 0x501C8 bytes), the twister state `D_00663140`, libc `rand`'s state, the three LCGs
and the pads. `DHD_HASH_LOG` also prints a hash of the static area and of all 32 MB, for
information only.

## The 3-4 player split screen (a data image bug)

In 3-4 player races the top row of the split screen (players 1 and 2) showed a flat brown
fill or a blurred close-up from the start, in both renderers; 2 players (side by side) were
fine. Not the pads (the same with neutral pads, already at GO), not the riders or the track,
not the cameras (the four camera blocks `D_0037EA60[i]` were sane and alike).

- A GS dump of one field (`DHD_GS_CAPTURE`, 8 fields) replayed in PCSX2 (finding 025's
  referee) left the top row black: the GS input itself was wrong, upstream of both renderers.
- In the dump, each view's draw environment had the right FRAME and SCISSOR, but the top row's
  XYOFFSET was y 912 against the bottom row's 1936, so its geometry landed ~1000 lines below
  the scissor (PCSX2 clips it all; our GS rasterised the guard-band triangles as garbage).
- The views' descriptors (`D_0027ECC0`, 0xC0 each, read by `func_0023D650` and friends) held
  height 2160 where the ELF has 112, and corner y 2160 where it has 112. No game store touched
  them (`DHD_WATCH` on the words: no hit), so the port's data image was wrong from the start.
- Cause: splat wrote the halfword pairs `(256, 112)` = 0x00700100 as `.word D_00700100`, an
  address inside the image, and `port/tools/data.py` relocated it (+0x08000000 → height
  0x0870). Fixed by listing those words as numbers (`data.py` `NUMBERS`). Now all four views
  draw, with their place numbers.

The same splat pattern (packed small numbers read as addresses) could exist in other tables;
a word whose target is not a real label, in the middle of a numeric record, is the sign.

## Exact versus native renderer

E4 between the exact and the native renderer (4 players, same pads) once parted at field 1560
of the race (a run with screenshots every 600 race ticks), and another run matched through
field 3540. Not chased: every online PC uses the same renderer, which the launcher sets and the
game enforces (`DHD_RENDER=native` or online play stops); interpolation is free per PC (E4
native+interp 120 Hz against plain native matched through field 2760).

## Results

- Live relay, 2 game instances plus 2 `nettool --bot` players: lobby, start, setup and the race
  inputs arrived on the same field everywhere (the countdown at field 403), input delay 5
  fields, ping about 55 ms, for 7200 fields.
- But the race state parted at field 1020 (state hashes differed). The cause wasn't found;
  sound was the next suspect.
- Once, the host froze when the 4th player joined. It didn't happen again.
- Lockstep was dropped before either was chased (above).

## Install and play (friends)

`python port/tools/package_online.py` builds `build/dhd-online.zip` (dhd.exe, launcher, README,
settings template). The owner sends the zip and, privately, an `online-settings.txt` with the
server line. Friends unzip, add the settings file, run `Play-online.cmd`, choose their own USA
ISO; the launcher refuses any other image (SHA-256 below). The host gets a 6-character code to
pass on.

USA disc (SCUS-97177, redump disc 30393, 2,530,869,248 bytes): MD5
`3654a6f4438c899be4a1f5236cfdcd74`, SHA-1 `a2739ce3b632c1513d2a96cf3982b5effcca5a3c`, SHA-256
`0fe2b77713d3440cc3030741ea0b18f49e3feaa09671a30a421e0d4888a7eb86`.

## Sources

- Finding 040 (recipe, E1-E6), 042 (client), 039 (server), 034 (uninitialised stack structs).
- Intel SDM vol. 3, ch. 18 (debug registers DR0-DR7, DR6 status) for the watchpoint.
- redump.org disc 30393 and HFS DB for the US disc hashes.
