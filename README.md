# Downhill Revived

A PC port of Downhill Domination.

Downhill Revived is a native Windows port of *Downhill Domination* (PlayStation 2, Incognito
Entertainment / SCEA, 2003). It is built on a matching decompilation: C/C++ source that
compiles back to the original game program byte for byte. The port takes that source, builds it
for the PC and replaces the PS2 hardware with native code. It adds a modern renderer, PC display
options, the game's movies, memory card saves, texture packs, online races and automatic
updates.

## Legal notice

- This is a fan project. It is not made, endorsed or supported by Sony Interactive
  Entertainment, Incognito Entertainment or anyone who holds rights to the game.
- This repository and its releases contain **no game data**: no disc image, no game executable
  or data from it, no IOP modules, no textures, sound, music or movies, and no BIOS. `dhd.exe` is
  the translated code only; at every start it reads the game's data (strings, tables, vector
  unit microcode) from your disc image, after checking it is the unchanged original.
- To play you need your own copy of the game: the **USA disc (SCUS-97177)**, dumped by you to an
  `.iso` file. Other regions and modified images are not supported.
- The project takes no donations and sells nothing.

## Features

What works today:

- **Native renderer** (Direct3D 11). The original PS2 graphics hardware is reproduced on the
  GPU. An exact emulation path stays in the code as the reference it is tested against.
- **60 fps with even frame pacing.** The game runs at its original 59.94 Hz, and each picture is
  shown on a fixed 59.94 Hz grid, so frames don't come out long-short-long. Pad input is read as
  late as possible before each frame.
- **Higher resolution and widescreen.** Internal resolution 1x to 4x (or matched to the window).
  Widescreen at the monitor's shape (16:9, 21:9 or any other) is Hor+: a wider view with the same
  vertical framing, and the HUD, menus and 2D screens keep their shape. Windowed, borderless and
  exclusive fullscreen. A new install starts borderless at 2x in widescreen.
- **The movies play**: the intro, the attract loop and OPTIONS > VIDEO CLIPS, decoded with
  libmpeg2 from your disc. Skip Movies in PC SETTINGS turns them off.
- **PC SETTINGS in the game's own OPTIONS menu**: resolution, display mode, window size, picture
  shape, scaling, widescreen, skip movies, vibration, texture pack and font, drawn with the
  game's own menus and font. Changes are saved to `dhd.ini`.
- **A sharp font**: the game's text is drawn with clean edges at any resolution, made in memory
  from the font on your disc (PC SETTINGS > Font: Sharp or Original).
- **Texture packs**: replacement packs made for PCSX2 (PNG files named by PCSX2's texture hash)
  work unchanged with the native renderer (see [Texture packs](#texture-packs)).
- **Automatic updates**: at start the game looks for a newer release and offers to install it;
  your settings and saves are never touched.
- **EXIT GAME** in the main menu.
- **Online races** for 2 to 4 players (experimental): an ONLINE entry in the main menu, a list of
  public races, private races joined with a 6-character code, and a race room where the host
  picks the track, on the project's official server (see [Online](#online)). After the
  results the game goes back to offline play with your own controller (TRY AGAIN, NEW LEVEL or
  QUIT).
- **Memory card saves**, stored as plain files beside the game.
- **Controllers**: Xbox pads, DualSense, DualShock 4, Switch Pro, 8BitDo and most other USB or
  Bluetooth pads (through SDL3 and its controller database), with vibration where the pad has
  motors. Buttons go by position, as on a PlayStation pad.
- **Keyboard play**: a whole race on the keyboard, with gradual steering; letters go by their
  place on the keyboard, so AZERTY and QWERTZ work too (see [Controls](#controls)).
- **Sound**: the game's sound driver is replaced by a native mixer, with music, effects and reverb.

Known gaps:

- Movies are shown 4:3 in widescreen, as on the PS2.
- The 2-4 player split screens have not been checked in widescreen.
- Online races have been tested with two games and two scripted players, not yet with four
  people on four PCs.
- The game has only been tested on a few PCs. Please report problems.

## Playing

1. Download `DownhillRevived-<version>-win.zip` from the [Releases](../../releases) page and
   unzip it into a folder you can write to (not Program Files). It holds `dhd.exe`,
   `README.txt`, `LICENSE.txt` and `THIRD-PARTY-NOTICES.txt`; nothing else is needed.
2. Run `dhd.exe`.
3. The first time, the game asks for your disc image. Pick your `.iso` of the USA disc. It is
   checked and remembered in `dhd.ini`, which the game writes then; you won't be asked again
   unless the file moves or changes.

Files the game keeps beside `dhd.exe`:

| File | What |
|---|---|
| `dhd.ini` | Your settings, one `key = value` per line. The game adds your disc image; the PC SETTINGS page (OPTIONS) writes the rest. |
| `saves\card1\` | Memory card 1, made at the first save. Copy this folder to back up your saves. |
| `dhd.log` | Messages from the last run. Attach it when you report a problem. |
| `dhd.exe.old` | The previous version, for a moment after an update; the next start deletes it. |

### Updates

At start the game looks for a newer release. If there is one it asks: **Update now** downloads
it, checks its SHA-256, puts it in place and starts the game again; **Later** starts this
version. Without a network the game just starts. An update replaces `dhd.exe` and the text files
only, never `dhd.ini` or `saves\`. You can also update by hand: copy the files of a newer release
over your folder (a release holds no `dhd.ini` and no saves, so yours are kept). Online races
need the newest version; an older one is offered Update Now in the ONLINE menu.

### Controls

A controller works out of the box. The buttons go by position, as on a PlayStation pad:
bottom = Cross, right = Circle, left = Square, top = Triangle. A pad SDL doesn't know can be added
with a line in SDL's format in `gamecontrollerdb.txt` beside `dhd.exe`.

No controller? The keyboard plays a whole race (only while the game's window is in front; it
works together with a controller):

| Key | Button | What |
|---|---|---|
| Arrow keys | d-pad / left stick | move in menus; steer and lean while riding (gradually: a tap steers a little, holding steers fully) |
| Space or X | Cross | confirm; pedal (press twice: sprint) |
| Esc, Backspace | Triangle | back in menus; while riding Esc pauses, Backspace brakes |
| S | Triangle | brake (in the air: tweak a trick) |
| Enter | START | start the race on the race setup screen, pause |
| Tab | SELECT | hold + up/down changes the camera, with Q looks back; press twice to bail out |
| Z / C | Square / Circle | punch / kick |
| A / D | L1 / R1 | tricks (both together: Superman) |
| Q / E | L2 / R2 | tricks; E (R2) is also bunny hop |
| W | R3 | sprint |
| F | L3 | powerslide |

Letters are places on the keyboard as on a US one: on AZERTY or QWERTZ use the keys in the same
places. The game's own controls screen (OPTIONS > CONTROLS) names the PlayStation buttons; the
keys above stand for them.

### Online

Online works out of the box: choose ONLINE in the main menu to create a public or private race,
or join one from the list or with its code. The game uses the project's official server,
`wss://dhd-online.downhill-revived.workers.dev`. Every player needs the same USA disc image.

Running your own server is optional. `online/server/` is the server (Cloudflare Workers), and
[online/server/README.md](online/server/README.md) explains how to deploy it. Then put its
address on the `server =` line of `dhd.ini` (the game writes the file at the first start):

    server = wss://YOUR-SERVER-ADDRESS

An empty `server =` line means the official server.

### Texture packs

Texture replacement packs made for PCSX2 work unchanged. Packs are made by their authors and are
not part of this project. Either put the pack's files in `textures\SCUS-97177\replacements`
beside `dhd.exe`, or name the pack's folder on the `texture_pack =` line of `dhd.ini` (for
example PCSX2's own `textures\SCUS-97177\replacements` folder in your Documents). PC SETTINGS >
Texture Pack turns it off or on at the next start.

### Settings

`dhd.ini` beside `dhd.exe` holds `key = value` lines; the game writes it, with comments, at the
first start. The PC SETTINGS page writes most of them for you.

| Key | Default | What |
|---|---|---|
| `iso` | (asked at the first start) | your disc image |
| `renderer` | `native` | `native`, or `exact` (the slow reference path) |
| `scale` | `2` | the 3D picture's resolution, `1` to `4` times the PS2's, or `window` |
| `window` | `borderless` | `windowed`, `borderless` or `fullscreen` |
| `widescreen` | `1` | `1` the monitor's shape, `0` off, or a shape such as `21:9` |
| `DHD_ASPECT` | `wide` | the picture's shape: `wide`, `4:3`, `16:9`, `stretch` or `pixels` |
| `skip_movies` | `0` | `1` skips the movies |
| `vibration` | on | `0` turns rumble off |
| `texture_pack` | (empty) | a texture pack's folder; empty: `textures\SCUS-97177\replacements` beside `dhd.exe` |
| `textures` | `on` | `off` draws the game's own textures |
| `texture_cache_mb` | half the card's memory, at most 2048 | graphics memory for replacements |
| `auto_update` | `ask` | `ask`, `on` (update without asking) or `off` (never look) |
| `update_url` | (empty) | where to look for updates; empty: this project's GitHub releases |
| `server` | (empty) | the online server; empty: the official one |
| `font` | `sharp` | `original` draws the game's font as the PS2 does |

Each key has a `DHD_*` environment variable (`scale` is `DHD_SCALE`, `server` is
`DHD_NET_SERVER`; see [docs/BUILDING.md](docs/BUILDING.md)), which wins over the file.

### System requirements

- Windows 10 or 11, 64-bit (`dhd.exe` is a 32-bit program).
- A graphics card with Direct3D 11 (feature level 11_0).
- A reasonably modern CPU: the PS2's vector units are run on the CPU.

## Building from source

The PC port and the matching build both need the original game executable, `SCUS_971.77`,
taken from your own disc. Nothing in this repository can be built without it.

Short version (Windows 10/11, Python 3.12+, Git, LLVM, Visual Studio Build Tools; details
in [docs/BUILDING.md](docs/BUILDING.md)):

    git clone <this repository>
    cd <repository folder>
    python -m pip install -r requirements.txt
    7z e "<your dump>.iso" SCUS_971.77 -oiso
    python tools/split.py
    python port/tools/build_clean.py

The executable is written to `build/port/clean/<revision>/dhd.exe`. This is the same build as
the releases (VU1 ahead-of-time code on, nothing else needed). To make the release zip from it:

    python port/tools/package_release.py --exe build/port/clean/<revision>/dhd.exe --version <version>

## Decompilation progress

<!-- progress:start -->
| Region | What | Code matched | Functions |
|---|---|---|---|
| crt0 | startup code | 0.00% | 0 / 5 |
| game | game code | 98.17% | 3439 / 3474 |
| libgcc | compiler runtime (library, not decompiled) | 0.00% | 0 / 145 |
| engine | engine (same toolchain as the game) | 92.76% | 1088 / 1119 |
| sdk | Sony SDK + C/math libraries (not decompiled) | 0.00% | 0 / 802 |
| handwritten | game/engine functions written in assembly (not decompilable) | 0.00% | 0 / 7 |
| remnant | dead-strip leftovers of removed functions (not decompilable) | 0.00% | 0 / 138 |
| **target** | **game + engine: everything there is to decompile** | **97.03%** | **4527 / 4593** |
| **total** | whole executable, libraries included | **85.76%** | **4527 / 5690** |
<!-- progress:end -->

"Matching" means that the C/C++ in `src/`, compiled with the game's original compiler (SN
Systems ProDG 3.01, gcc 2.95.3) and the original flags, gives exactly the bytes of the retail
executable. A function counts only when a tool confirms that its bytes and the whole rebuilt
executable are identical to retail; nobody's word is taken for it. Sony's SDK and the compiler's
own runtime are library code and are not decompiled. `python tools/report.py` refreshes this
table and `progress/report.json` (an [objdiff](https://github.com/encounter/objdiff) report, in
the format [decomp.dev](https://decomp.dev) reads).

## Repository layout

| Path | What |
|---|---|
| `src/` | The decompiled game and engine, in C/C++. Each file starts with the address range it replaces. |
| `include/` | Shared headers: structs and types of the game. |
| `config/` | splat configuration, symbol names, identified library functions. |
| `tools/` | The matching build and its checks (`configure.py`, `check_image.py`, `try_func.py`, `promote.py`, `report.py`). |
| `port/` | The PC port: build tools (`port/tools/`), PS2 system replacement (`port/hle/`), graphics (`port/gs/`), CPU/VU runtime (`port/runtime/`), network client (`port/net/`), launchers. |
| `online/` | The online race server (Cloudflare Workers + Durable Objects). |
| `docs/` | `STATE.md` (where each part stands), `findings/` (how things were worked out), `WORKER.md` (how to decompile a function), `BUILDING.md`, the online protocol. |

## Contributing

Contributions are welcome: decompiling the last functions, naming and typing what is matched,
port fixes, widescreen split screens, testing on other PCs.

- Read `docs/STATE.md` and [AGENTS.md](AGENTS.md) (the project's rules) first.
- To decompile a function, follow `docs/WORKER.md`. A function is accepted only when
  `tools/try_func.py` prints MATCH and `tools/promote.py` confirms the whole image still matches.
- Port changes go in `port/`; `src/` must keep matching.
- Never commit game data, secrets or server addresses other than the official server's.

## Credits

This project builds on the work of others:

- [PCSX2](https://github.com/PCSX2/pcsx2) (GPL-3.0-or-later): the reference for how the hardware
  behaves. The GS memory layouts in `port/gs/memory.c` follow PCSX2's GSLocalMemory/GSTables.
  The test and comparison tools build PCSX2 from source at test time.
- The PCSX2 widescreen patch for this game, by Aced14
  ([pcsx2_patches](https://github.com/PCSX2/pcsx2_patches)): the map of values the 16:9 mode
  changes.
- [ps2sdk](https://github.com/ps2dev/ps2sdk): protocol and library references (no code
  included). The reverb presets are read from the player's own disc (Sony's libsd module).
- [OpenGOAL](https://github.com/open-goal/jak-project) (ISC): the SPU2 Gaussian interpolation
  table in `port/hle/sound_gaussian.h`.
- [SDL](https://libsdl.org) 3.4.18 (zlib): controllers, built from its pinned release source by
  `port/tools/sdl3.py` and linked in statically.
- [libmpeg2](https://libmpeg2.sourceforge.io) 0.5.1 (GPL-2.0-or-later): the movie decoder,
  `port/third_party/libmpeg2`, unchanged.
- [xxHash](https://github.com/Cyan4973/xxHash) (BSD-2-Clause): the XXH64 specification, from
  which `port/net/hash.c` is written, and `port/gs/xxhash.h` (XXH3, unchanged) for texture pack
  names.
- PCSX2's texture replacement names (its GSTextureCache / GSTextureReplacements hash scheme),
  reimplemented in `port/gs/texpack.c` so PCSX2 packs load unchanged.
- [splat](https://github.com/ethteck/splat), [spimdisasm](https://github.com/Decompollaborate/spimdisasm)
  and [rabbitizer](https://github.com/Decompollaborate/rabbitizer) (MIT): splitting and
  disassembling the executable.
- [objdiff](https://github.com/encounter/objdiff) (MIT/Apache-2.0) and [decomp.dev](https://decomp.dev):
  progress reports.
- [m2c](https://github.com/matt-kempster/m2c) (GPL-3.0): first drafts of functions.
- [decomp.me](https://decomp.me) and its [compilers](https://github.com/decompme/compilers)
  collection, and the SN ProDG mirror by AngheloAlf: the period compilers.
- [ps2dev](https://github.com/ps2dev/ps2dev) binutils (GPL-3.0): assembling and linking.
- [ps2tek](https://psi-rockin.github.io/ps2tek/), [Play!](https://github.com/jpd002/Play-),
  [vgmstream](https://github.com/vgmstream/vgmstream) and other PS2 projects: documentation and
  ideas.
- [ninja](https://ninja-build.org) (Apache-2.0), LLVM/clang (Apache-2.0 with LLVM exception),
  [wrangler](https://github.com/cloudflare/workers-sdk) (MIT/Apache-2.0).

The full notices of the code adapted into this repository are kept with it
(`port/hle/audio_licenses/`, file headers).

## Licence

[GPL-3.0-or-later](LICENSE). The original game, its data, names and trademarks belong to their
owners and are not covered by this licence. Code adapted from other projects keeps its own
notice (see Credits).
