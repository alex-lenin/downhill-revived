# 053 — The first public release: reverb from the disc, a reproducible build, the zip (round 39)

Goal: a release that anyone can rebuild from the public repository and their own disc, with no
third-party code under a GPL-incompatible licence and no local inputs.

## 1. Reverb presets from the player's disc

The native mixer's reverb (`port/hle/sound_reverb.h`, SetReverbType modes 0-9) needs ten preset
register sets. They were in `port/hle/sound_reverb_presets.h`, adapted from ps2sdk's
`iop/sound/libsd/src/effect.c` under the Academic Free License 2.0, which the FSF lists as
GPL-incompatible. The header and its licence file are gone; the presets are read at run time.

Where the game's own copy lives: the disc's `MOD/LIBSD.IRX` (28,557 bytes), Sony's libsd, which
989snd calls for the SPU2 effect. In its data:

- `uint32 size[10]`: the effect work area of each mode, in units of 4 sample words (mode 0: 2);
- directly after it, ten entries `{uint32 flags; uint16 reg[32]}` (68 bytes), the 32 SPU2 effect
  registers in hardware order (dAPF1, dAPF2, vIIR, vCOMB1-4, vWALL, vAPF1-2, mSAME, mCOMB1, ...,
  vIN L/R).

On this disc the table starts at file offset 0x4AE8. The values are the ps2sdk table exactly:
all ten modes, every register, and size = ps2sdk size × 2 (checked by script).

The mixer does not use the offset. `reverb_presets_load` (first SetReverbType) finds the module
with `cdvd_find("/MOD/LIBSD.IRX;1")`, reads it whole and scans it (2-byte steps) for the table's
shape:
- every flags word 0;
- every size non-zero and fitting `SndReverb.ram` (size × 4 ≤ 0x1804 × 8 sample words);
- mode 0 (OFF): every register 0 except mSAME and mDIFF (10, 11, 18, 19), which are 1;
- modes 1-9: vIN L and R (registers 30, 31) both 0x8000.

The shape matches once in the module. Without a match (no disc, another module), every mode plays
as OFF and the log says so once. `DHD_SND_TRACE=1` logs the registers each SetReverbType writes.

Checks:
- `test_sound` (test_runtime.py) serves an invented table of the same shape (made-up rooms,
  not the retail values) as the disc's LIBSD.IRX; the reverb tests pass on it.
- `replay_sound.c` (the audio replay) looks the module up on the ISO with its own ISO 9660 walk.
- At a race start (bench race, fields 3000-3100, `DHD_SND_TRACE=1`), the game sets one preset:
  core 1, mode 3, size 9,248 words. Its 32 registers equal what the old table gave for mode 3.

Sources: ps2sdk `effect.c` and `libsd-common.h` for the table layout and register order (no code
or values copied); the [no$psx SPU specification](https://psx-spx.consoledev.net/soundprocessingunitspu/)
for the register meanings.

## 2. The release build is the plain build

Recent test executables were linked with
`build_clean.py --recomp-basis build/port/r29/native16/objects.rsp`, a local object set that is
not in the repository.

What the basis provides: it only swaps the recompiled asm-only functions (`build/port/recomp`;
721 objects of this link) for frozen copies compiled before commit 8815d61e. Speed does not come
from it:
- the VU1 AOT blocks and the native VU1 kernels come from `DHD_BUILD_VU_AOT=1`, which
  `build_clean.py` always sets;
- `vu1_aot.py` and `vu1_native_clip.py` generate them under `build/` from `assets/vutext.bin`,
  which `tools/split.py` writes from the player's executable.

The "third of the speed" (finding 047) is a bare `link.py` without that variable, not a plain
`build_clean.py`.

Proof from a fresh clone of main, in a temporary folder:

1. `git clone`, then `7z e <iso> SCUS_971.77 -oiso` (SHA-1 `4e25a222…`).
2. `python tools/split.py` (13 s).
3. `python port/tools/build_clean.py`: compile 81 s, link 273 s, no other input.

Results on b7904561:
- Speed. Bench race 6000-7000, 3 interleaved runs, all focused: plain 127.2 fields/s, a basis
  link of the same revision 123.5. An earlier pair on 2176dc56 (2 runs): plain 141.6 against the
  1499a7c4 basis build 140.9. The machine was busier for the second pair; the plain build is
  never slower.
- The exe's only absolute path was the shader's (`link.py` embedded it). It is now the
  repo-relative `port/gs/raster.hlsl`.
- `gate_native_renderer.py` against `r38-reviewed`: both arms match each other at all six moments
  and in every readback (1,153). The canonical reference differs in 54 words at each early
  moment, all in 0x8A0450-0x8A0780:
  - that is the bottom of the main thread's stack (`D_8A0400`, `port/hle/boot.c`): dead frames
    shifted by 16 bytes, holding host return addresses;
  - the cause is the recompiled SDK functions, compiled from the tree instead of the frozen basis;
  - the basis link of the same revision passes against r38-reviewed, which confirms it;
  - two plain builds give identical early dumps.
- So the reference is re-recorded from the plain build, `build/port/compare/r39-plain` (manifest
  lists the 54 words), and is the gate's default now. No check or exclusion changed. Gate on the
  plain exe, separate run: MATCH (all moments, 1,167 readbacks).

## 3. The release zip

`port/tools/package_release.py --exe build/port/clean/<rev>/dhd.exe --version X` writes
`build/release/DownhillRevived-X-win.zip`, one folder holding:
- `dhd.exe`;
- `dhd.ini`, from `port/release/dhd-default.ini`: comments and an empty `server =` line (no
  longer: section 4);
- `README.txt`, `THIRD-PARTY-NOTICES.txt` (port/release/), and `LICENSE.txt`.

`dhd.exe` imports only Windows DLLs (kernel32, user32, d3d11, dxgi, D3DCOMPILER_47, winhttp,
ole32, bcrypt); the C runtime is static. So nothing else is shipped.

The script refuses:
- an exe that build_clean.py didn't make (hash and manifest) or that was linked with
  `--recomp-basis`;
- any absolute path in the exe, ASCII or UTF-16;
- `\Users\`, the builder's account name, or the checkout's path or its parent's in the exe;
- a `workers.dev` host, or a server named by `DHD_NET_SERVER` or `--settings`;
- a server line in dhd.ini, or a `ws://` host other than the example in the text files;
- game-data file names.

An empty `server =` is ignored by `host.c` (empty values are skipped), so ONLINE says
"No server: set server = ... in dhd.ini" until the player adds one. `package_online.py` (the
friends' zip) is retired.

0.1.0 test (dhd.exe of b7904561, zip 1,988 KB), unzipped into a new folder and started without
`DHD_*` variables:
- the disc message and the file-open dialog appeared; the choice was saved into the shipped
  dhd.ini below `server =`, with `renderer = native` and `skip_movies = 1`;
- the title screen came up, then (driven by `DHD_NAV` from that point) rider select, track
  select and a race on Mt. Midoule at 60 fps;
- screenshots in `build/port/r39rel` (local).

## 4. Game data out of dhd.exe (round 39, second part)

Before: dhd.exe held the PS2 data image (`.ps2img`, 238,080 bytes: .vutext, .data, .rodata,
.gcc_except_table, .sdata, assembled from `asm/data`) and the VU microcode a second time, as the
compare words of the VU1/VU0 AOT blocks and the native VU1 kernels. A scan of the b7904561
exe (`port/tools/scan_retail.py`: every 64-byte window of the ELF with 8 or more distinct byte
values, against every exe offset) found 272 runs, 231,918 bytes.

Now:
- `port/hle/game_data.c` reads `SCUS_971.77` from the disc image's root at start (its own ISO
  9660 lookup), checks size and SHA-1 (`4e25a222…`, own SHA-1 code), and hands the bytes of its
  one PT_LOAD segment from 0x263BC0 to 0x29DCF0 to `ps2_image_load` (`port/runtime/memory.c`).
- `ps2_image_load` copies them to PS2 RAM, then writes the words the PC build changes: 4,419
  {PS2 address, value} pairs in `fixups.s` (pointers, which are host addresses here, code
  addresses of recompiled functions, `$LEH_` ranges). Switch-table entries equal to the retail
  word need none. Then the movie/heap words get PS2_RAM_BASE as before.
- `data.py` still writes `image.s` with every label in place; the link assembles it only to place
  the fixups and to check the result: outside the fixups the image must equal
  `iso/SCUS_971.77` byte for byte (it does, gaps included), and its end must be 0x29DCF0.
  `build_clean.py` copies the ELF into its worktree for that.
- The AOT blocks and kernels: `vu1_aot.py` and `vu1_native_clip.py` emit
  `AOT_WORDS(name, count, ...)` arrays that are empty (.bss) in dhd.exe, plus a table of where
  each array's words are in .vutext (`vudis.walk(..., sources=True)`; 781 + 12 entries, checked
  against the microcode when generated). `ps2_vu_aot_load` fills them from PS2 RAM right after
  the image load, before any VU code runs. The test and bench programs, which have no disc, define
  `PS2_VU_AOT_EMBED` and keep the constants. The comparison code is unchanged.
- A missing, changed or short SCUS_971.77: the first-start disc check (`host.c disc_problem`)
  now asks `game_data_read` too, so the prompt says why ("isn't the original one: the disc image
  is changed or damaged, or from another release") and offers the file dialog again. If the
  check fails later (DHD_ISO from the environment), a message box says so and the game ends.
  Tested on a copy of the first 7 MB of the disc with one ELF byte flipped, a file that is no
  disc, and the real disc.

Scan of the 90a288bb exe: 14 runs, 3,423 bytes, all compiled from `src/`: string literals of
seven units, a float table of `001EB1D8.cpp`, and the CRC-32 table clang computes at compile time
from `func_00234F38`'s loop (`scan_retail.SOURCE` lists them; `package_release.py` refuses any
other run in any packaged file). Against every other disc file up to 4 MB (2,246 of 2,338:
modules, banks, textures; the rest are movies and big audio banks), the exe's data sections
match only `MOD/LIBSD.IRX` in `snd_note_table`, the pitch table of `port/hle/sound_tables.h`:
it is floor(32768 · 2^(n/12)) and floor(32768 · 2^(n/1536)), which the formula reproduces
exactly (as `snd_pan_table` is floor(16383 · sin(πn/360))). Same math, so the same bytes; no
data from the disc.

Results on 90a288bb (plain `build_clean.py`, compile 73 s, link 215 s):
- Gate against `r39-plain`, unchanged: MATCH at all six moments, 1,153 readbacks. The data
  image built from the disc plus fixups is the image the exe used to hold.
- Bench race 6000-7000, native renderer, all runs focused, against the b7904561 release exe:
  two sessions, 3 runs in order before/disc then 5 in order disc/before. Medians 141.96 vs
  140.95, then 139.71 vs 141.32 fields/s: the first candidate of each pair is ahead; over all
  8 runs each, 140.87 (before) vs 141.13 (disc). No drop.
- 0.1.0 zip (1,916 KB; dhd.exe, README.txt, LICENSE.txt, THIRD-PARTY-NOTICES.txt) unzipped into
  a new folder and started with no `DHD_*` variables: the disc prompt; a copy of the disc's first
  sectors with one ELF byte flipped was refused with "isn't the original one" and Retry; the real
  disc was accepted, dhd.ini written from the template, the title screen came up. A second start
  (driven by `DHD_NAV`) reached a race at 60 fps. `DHD_ISO` pointing at the changed copy gives
  the start-time message box and exits. Unzipping the release again over the folder kept dhd.ini
  and a file in `saves\card1` byte for byte. The folder was deleted afterwards.

dhd.ini: the zip ships none (port/release/dhd-default.ini is gone). `settings_save` writes the
same template (comments and an empty `server =`) when it creates the file, which the first start
does when it saves the disc image. A new version unpacked over an old folder replaces only
dhd.exe and the text files.

## 5. The official server is the default (round 39, third part)

The project's public race server is deployed at `wss://dhd-online.downhill-revived.workers.dev`
(`online/server/`, HTTP `GET /v1/lobbies`). It is the one server address the repository and the
release may name. `host.c` `host_settings_load` sets `DHD_NET_SERVER` to it after reading dhd.ini
when nothing set it: no dhd.ini, an empty `server =`, no `DHD_NET_SERVER`. An explicit
`server = ...` or `DHD_NET_SERVER` still wins (self-hosting). The template's comment says so.
This replaces section 3's "No server: set server = ... in dhd.ini" for an empty line.

`package_release.py` allows that host and nothing else: any other `*.workers.dev` host in dhd.exe
or the text files, a `ws(s)://` address other than it and `races.example.org`, or the value of
`DHD_NET_SERVER` / `--settings` (unless it is the official one) refuses the zip.

Test on bb898f87 (plain `build_clean.py`, dhd.exe aade1819...), the 0.1.0 zip unzipped into a new
folder, no `DHD_NET_SERVER` and `server =` empty, three instances driven by `DHD_NAV` (no keys):
- HOST: ONLINE > Create A Public Race: room VVGW8W; `GET /v1/lobbies` listed it (1/4, ALPINE).
- A second instance: ONLINE > Public Races showed "HOST 1/4 ALPINE".
- A third: ONLINE > Join With A Code, VVGW8W typed with left/right and Cross, Join: in the room,
  both players READY (pings 42 and 47 ms); the server listed 2 players. The race was not started.
The zip is unchanged by the packaging fix (sha256 5434d5db...b033). The folder was deleted.
After the three were stopped the lobby stayed listed (1 player) for about two minutes, then the
server dropped it.

## 6. Release 0.2.0 (round 41): a new gate reference, the zip, smoke and updater checks

Build: `build_clean.py` on 22e70d54 (main after findings 054-057 and the icon commits),
compile 62 s, link 198 s; dhd.exe sha256 2253ddf9d124662a76c70bdfb391854a1e124e09a81e9d5537b384056d47f706.
The zip's README.txt is from 777fa75b (known gaps updated; the exe is unchanged by it).

Gate reference `build/port/compare/r40-plain`:
- On 22e70d54 against r39-plain the arms match each other at every moment through GO+4500 and in
  all 1,161 readbacks; against r39-plain both arms differ in exactly 2 words at GO+0/2/12/16,
  0x004F4324 (0x10 -> 0x24) and 0x004F4454 (0 -> 0x10). Those are GS packet bytes left in the
  font buffer by the main menu's EXIT GAME text (finding 056), not race state.
- So the reference is recorded from that build (`--record`, its exact arm; arms matched, 1,159
  readbacks), and `gate_native_renderer.py`'s default is now r40-plain: the release build, and
  the first build with every round-40 change in.
- The gate on HEAD (the same exe; 777fa75b changes only README.txt) against r40-plain:
  NATIVE RENDERER GAME-STATE GATE MATCH, EE at every moment, 1,151 readbacks. The readback count
  varies from run to run (1,161, 1,159, 1,151 on this exe); within a run the arms always agree.

Package: `package_release.py --exe build/port/clean/22e70d54/dhd.exe` wrote
`DownhillRevived-0.2.0-win.zip` (2,135 KB, sha256
a14684e0576fc8534afc0d09b5dd8045a3a97f20cedc72dd4ac2d0ed29756a0c) and `latest.json` (version 0.2.0,
the v0.2.0 release asset URL, that sha256, size 2,186,595). `scan_retail.py`: 14 runs of 64+
bytes, all from the decompiled source, none other. `llvm-readobj --coff-resources`: ICON ids 1-7
and GROUP_ICON 1 (port/res/dhd.rc); the window title bar shows it.

Smoke test, the zip unzipped into a new folder, no `DHD_*` variables except `DHD_WINDOW=windowed`
(so the borderless default didn't cover the desktop of the PC in use), menus by `DHD_NAV`,
dialogs by window messages:
- First start: the disc prompt; the real disc chosen; dhd.ini written with `renderer = native`,
  `scale = 2`, `window = borderless`, `widescreen = 1`, `DHD_ASPECT = wide`, `skip_movies = 0`,
  `auto_update = ask`, `textures = on` and the iso. The movies played (SCEA, copyright, the
  Incognito logo animation).
- EXIT GAME > Yes, Quit: "exit game: chosen in the main menu", exit code 0, no process left.
- A race (the gate's menu script): Alpine, the race clock running at 60 fps.
The folder was deleted.

Updater dry run (finding 054 tested the mechanics with a fake 0.2.1; this ran the real 0.2.0
zip): a copy of this dhd.exe with its three "0.2.0" strings changed to "0.1.9", in a folder with
`auto_update = on`, `update_url` = a latest.json on a local `http.server` naming the 0.2.0 zip and
its sha256, a README and a save file. Log: "0.2.0 is available (this is 0.1.9)", downloaded
2,186,595 bytes with the right sha256, "installed over 0.1.9; starting it", the new process
deleted dhd.exe.old and found "0.2.0 is the newest". After it: dhd.exe is the release's (sha256
2253ddf9...), README.txt replaced, LICENSE and notices added, dhd.ini and the save byte for byte
the same, no dhd-update folder. The folder was deleted.

## Open

- A save writes `saves\card1` on demand; the first run above made none.
