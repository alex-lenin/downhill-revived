# Building Downhill Revived

There are two builds:

- **The PC port**: `dhd.exe`, the game for Windows. Built with clang.
- **The matching build**: the original PS2 executable rebuilt from `src/` with the game's
  original compiler, then compared with retail section by section. This is what proves the
  decompilation. You only need it to work on `src/`.

Both start from your own copy of the game executable. The repository contains none of it.

## 1. Prerequisites

For both builds:

- Windows 10 or 11, 64-bit. Commands below are for PowerShell; Git Bash works too.
- [Git](https://git-scm.com).
- [Python](https://www.python.org) 3.12 or newer, on PATH as `python`.
- [7-Zip](https://www.7-zip.org) (or any tool that can read an ISO).
- A dump of your own *Downhill Domination* USA disc (SCUS-97177) as an `.iso`.

For the PC port, also:

- [LLVM](https://github.com/llvm/llvm-project/releases) for Windows: `clang`, `clang-cl`,
  `lld-link`, `llvm-rc`, `llvm-nm`, `llvm-objcopy` and `llvm-cxxfilt` on PATH. The project is built with
  LLVM 23. `build_clean.py` also finds a [scoop](https://scoop.sh) install (`scoop install llvm`)
  that is not on PATH.
- Visual Studio 2022 Build Tools (or Visual Studio) with the "Desktop development with C++"
  workload: the MSVC **x86** libraries and a Windows SDK. clang finds them by itself.
- [CMake](https://cmake.org) 3.16 or newer, on PATH or in `C:\Program Files\CMake`, for SDL3
  (controllers). `requirements.txt` brings the `ninja` package it builds with.
- A network connection once, for SDL3's source (see [SDL3](#sdl3-controllers)).

For the matching build, also:

- A few hundred MB of disk for the toolchain, which `tools/setup_toolchain.py` downloads.
- For four units built with Sony's older compiler (`-sce29` in `src/`): WSL with Ubuntu
  and its 32-bit C library (`sudo apt install libc6-i386`), and SCE ee-gcc 2.9-991111 from the
  [decomp.me compilers](https://github.com/decompme/compilers/releases/tag/compilers) release,
  unpacked into `tools/compilers/ee-gcc2.9-991111/` (archive `ee-gcc2.9-991111.tar.xz`, SHA-256
  `99cb5db603cd985b50fc6851157020ad69489f2227309ccf825bd43ac7bc19dd`).

## 2. Get the code and the executable

    git clone <repository URL> downhill-revived
    cd downhill-revived
    python -m pip install -r requirements.txt
    7z e "<your dump>.iso" SCUS_971.77 -oiso

Check that it is the right file:

    certutil -hashfile iso\SCUS_971.77 SHA1

It must print `4e25a222ad8a475b5d26c2e928e90b5f2b7075eb`. `config/splat.yaml` checks it too.

Then split it:

    python tools/split.py

This takes about 15 seconds and writes `asm/` and `assets/` (about 37 MB) from your executable with
[splat](https://github.com/ethteck/splat). Both folders are game-derived and gitignored, like
`iso/`. Never commit them.

## 3. Build the PC port

    python port/tools/build_clean.py

This builds the last commit (`--rev` picks another) in a separate, clean git worktree
(`build/port/clean/tree`), so uncommitted edits and old objects can't get into the executable.
It compiles every unit of `src/` and `port/` (`port/tools/build.py`), recompiles the functions
that are still assembly into C, converts the game's data, and links with VU1 ahead-of-time code
on (`DHD_BUILD_VU_AOT=1`: the VU1 ahead-of-time blocks and the native VU1 kernels, generated
under `build/` from your executable's microcode).

`dhd.exe` holds no game data. The game's data sections (`.vutext` to `.sdata`) are read from
`SCUS_971.77` on the player's disc image at every start, checked by SHA-1
(`port/hle/game_data.c`); the executable keeps only the words the port changes in them
(pointers and code addresses, `port/tools/data.py`), and the link checks that everything else
is byte for byte your `iso/SCUS_971.77`. The VU ahead-of-time blocks and kernels take their copy
of the microcode from the same place. About 3,800 units: on a 6-core desktop CPU the
compile takes about 1.5 minutes and the link about 5.

The result is `build/port/clean/<revision>/dhd.exe`, with `manifest.json` (revision, hash,
build flags) beside it. This plain command is the release build: nothing else is needed for a
full-speed executable. `--install` also copies it to `build/port/fast-preview/native/dhd.exe`
with the settings launcher. (`--recomp-basis` exists only to compare with old local builds; never
use it for a release.)

### SDL3 (controllers)

`port/tools/sdl3.py`, which `link.py` runs, provides SDL3 as a static library, so `dhd.exe`
stays one file:

- it downloads `SDL3-3.4.18.tar.gz` from SDL's GitHub release into `build/downloads/` and checks
  its SHA-256 (pinned in the script; any other file is refused). `build_clean.py` copies
  `build/downloads/` into its worktree, so later builds need no network;
- it builds with CMake + Ninja + clang for `i686-pc-windows-msvc`, the static C runtime, only the
  joystick, gamepad, HIDAPI and event parts (DirectInput, XInput, RawInput, Windows.Gaming.Input
  and HID drivers), into `build/port/sdl3/3.4.18/` (about 2 minutes, once per worktree);
- `link.py` links `SDL3-static.lib` and the Windows libraries it needs.

To change the version, update `VERSION` and `SHA256` in `sdl3.py` together (the hash of the
release's `.tar.gz` as GitHub lists it). `python port/tools/sdl3.py` builds it alone.

### Package a release

    python port/tools/package_release.py --exe build/port/clean/<revision>/dhd.exe --version <version>

This writes `build/release/DownhillRevived-<version>-win.zip`: one folder with `dhd.exe`,
`README.txt`, `LICENSE.txt` and `THIRD-PARTY-NOTICES.txt` (from `port/release/`). There is no
`dhd.ini` in it: the game writes one the first time it saves a setting, so unpacking a new
version over an old folder never replaces a player's settings or saves. `dhd.exe` needs only
Windows' own DLLs. The script refuses an executable that `build_clean.py` didn't make or that was
linked with `--recomp-basis`. It also refuses one that holds an absolute path (your user or
checkout folder) or a server address other than the official server's, and any file that
names another server. Every packaged file is
scanned for runs of 64 bytes or more of your `iso/SCUS_971.77` (`port/tools/scan_retail.py`);
any run outside the short list of string literals and tables that the decompiled source itself
holds stops the package.

For a quicker loop while working on the port (uses your working tree as it is):

    python port/tools/build.py -j 8
    $env:DHD_BUILD_VU_AOT = "1"
    python port/tools/link.py

The result is `build/port/link/dhd.exe`. Without `DHD_BUILD_VU_AOT=1` the vector unit code runs
in the interpreter, at about a third of the speed.

To run a build, copy `dhd.exe` to a folder of its own and start it. It asks for your `.iso` the
first time. `DHD_ISO=<path to iso>` in the environment skips the question.

Tests of the port runtime (after `port/tools/build.py`):

    python port/tools/test_runtime.py

They include `test_keyboard` (keyboard layout and steering ramp) and `test_gamepad` (SDL starts
and lists the controllers; none needed).

### The game-state gate

`port/tools/gate_native_renderer.py` plays a recorded race twice with one executable, once with
the exact PS2 graphics path and once with the native renderer, and requires the game's memory
to match between the two and, at the first moments of the race, a reference recording. The
reference is a capture of the game's memory, so it is not in the repository: record your own
once, from a plain `build_clean.py` executable of a release commit:

    $env:DHD_ISO = "<path to your iso>"
    python port/tools/build_clean.py --rev <release tag or commit>
    python port/tools/gate_native_renderer.py --exe build/port/clean/<revision>/dhd.exe --out build/port/gate/record --reference build/port/compare/mine --record

`--record` checks the two arms against each other and then writes the exact arm to
`build/port/compare/mine` (it must not exist yet). Later builds are compared with it:

    python port/tools/gate_native_renderer.py --exe build/port/link/dhd.exe --out build/port/gate/<name> --reference build/port/compare/mine

Each arm plays the race once, a few minutes each. The gate prints `NATIVE RENDERER GAME-STATE GATE MATCH` when it passes. Record the reference again
only when a change to the game's memory is understood and intended, and say why in the commit.

## 4. Build the matching executable

    python tools/setup_toolchain.py
    python tools/configure.py
    python -m ninja

`setup_toolchain.py` downloads the pinned toolchain into `tools/compilers/` and checks every file
against `toolchain.lock.json`:

- SN Systems ProDG 3.01 (gcc 2.95.3 "SN BUILD 1.36" and the `ps2eeas` assembler), the game's
  own compiler, from a public mirror. Flags: `-O2 -G8 -fopt-stack`, C++ adds `-fno-exceptions`
  (`docs/findings/002-compiler-sn-prodg.md`).
- ps2dev binutils, to assemble the functions that are still assembly and to link.
- objdiff-cli, for progress reports.

`python -m ninja` rebuilds the executable from `asm/` and `src/`, then `tools/check_image.py`
compares every section with retail. The build must end with `MATCH`.

## 5. Decompile a function

    python tools/queue.py list
    python tools/try_func.py func_XXXXXXXX build-try/cand/func_XXXXXXXX.c
    python tools/promote.py func_XXXXXXXX build-try/cand/func_XXXXXXXX.c --commit

`try_func.py` builds the whole image with your candidate and compares the function word for
word. `promote.py` moves it into `src/` only if the whole image still matches.
`docs/WORKER.md` has the rules that make code match. `python tools/report.py` refreshes the
progress table in `README.md`.

## 6. The online server

`online/server/` is a Cloudflare Workers project. Its
[README](../online/server/README.md) covers tests, a local server (`wrangler dev`) and
deployment. The game's built-in default is the official server,
`wss://dhd-online.downhill-revived.workers.dev` (`port/hle/host.c`, `OFFICIAL_SERVER`), used when
`dhd.ini`'s `server =` is empty and `DHD_NET_SERVER` is unset. Self-hosting is optional: a
`server = wss://...` line in `dhd.ini`, or the `DHD_NET_SERVER` environment variable, points the
game at another server. Don't commit any other deployed address; `package_release.py` refuses
a package that names one.

## Settings for developers

`dhd.ini` beside `dhd.exe` holds `key = value` lines. Each key stands for a `DHD_*` environment
variable (`port/hle/host.c`, `apply_setting`), and a variable set in the environment wins over
the file. A few useful ones:

| Key / variable | What |
|---|---|
| `iso` / `DHD_ISO` | the disc image |
| `scale` / `DHD_SCALE` | internal resolution, `1` to `4` or `window` |
| `widescreen` / `DHD_WIDESCREEN` | `1` the monitor's shape, `0` off, or a shape such as `21:9` |
| `DHD_ASPECT` | the picture's shape: `wide`, `4:3`, `16:9`, `stretch` or `pixels` |
| `window` / `DHD_WINDOW` | `windowed`, `borderless` or `fullscreen` |
| `skip_movies` / `DHD_SKIP_MOVIES` | `1` skips the movies (the test tools set it) |
| `vibration` / `DHD_VIBRATION` | `0` turns rumble off |
| `memory_card` / `DHD_MEMCARD` | `0` turns the memory card off |
| `texture_pack` / `DHD_TEXTURE_PACK` | a PCSX2-format texture pack folder |
| `textures` / `DHD_TEXTURES` | `off` draws the game's own textures |
| `texture_cache_mb` / `DHD_TEXTURE_CACHE_MB` | graphics memory for replacement textures |
| `auto_update` / `DHD_AUTO_UPDATE` | `ask`, `on` or `off` |
| `update_url` / `DHD_UPDATE_URL` | a `latest.json` or GitHub releases API address (empty: the official releases) |
| `server` / `DHD_NET_SERVER` | the online server address (empty: the official server) |
| `renderer` / `DHD_RENDER` | `native` (default for players) or `exact` (the reference) |
| `font` / `DHD_FONT` | `sharp` (the default) or `original`, the game's font as the PS2 draws it |
| `DHD_SDL` | `0` reads controllers without SDL (XInput and Sony pads only) |

A dhd.ini without a key gets the player defaults (`port/hle/host.c`, `player_defaults`):
native renderer, scale 2, borderless, widescreen 1, movies on. With no dhd.ini at all (the test
tools set `DHD_SETTINGS=none`) none of them apply. Tools that drive the game (`DHD_NAV`,
`DHD_RACE_SYNC`, ...) never run the updater.

## Releases

`port/include/version.h` holds the version, the only place it is written. `package_release.py`
names the zip after it and writes `build/release/latest.json` beside it (version, the release
asset URL, the zip's SHA-256 and size) for the updater; publish both as release assets.
