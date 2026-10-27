# 093: the player launcher (DownhillRevived.exe)

Players needed one place for settings, the rider name, updates and the Discord server, on every
platform the game reaches. `port/launcher` is that window. It opens before the game and starts it.

## Shape

- **Tauri 2.** A Rust core (`src-tauri/src`) and an HTML/CSS front end (`ui/`), one codebase for
  Windows, macOS and Linux. On Windows it uses the system's WebView2, so nothing is bundled.
- **Size.** The release exe is about 3.9 MB. It imports only Windows' own DLLs (comctl32, secur32
  and shlwapi beyond the game's list). HTTPS goes through native-tls (SChannel and the system
  certificates).
- **Window.** 960x600, not resizable, no system title bar. The page draws its own title bar, a
  sidebar (Play, Settings, Patch notes, the rider card), the main pane and a footer. The footer
  holds the update state, Discord and PLAY on every page.
- **Design.** The design came from mockups in the site's palette (night, ember, gold, lilac;
  Archivo), checked against the Impeccable design skill's rules:
  - real Lucide icons, no glyphs;
  - no eyebrow labels and no card grids;
  - solid panels, no glass;
  - every state drawn.

  A second pass (an Impeccable-rules review, then v4 of the mockup) made it plainer:
  - no screenshot art;
  - every control on one 8 px radius; PLAY is an ordinary ember button, and the italic wordmark
    is the only slanted thing;
  - one primary button whose label follows the state (Play, Choose disc, Update);
  - the focus ring only after a key press, with PLAY focused at start so Enter starts the game;
  - errors stay in the footer until dismissed, with their fix as a button (Open folder, Retry);
  - closing a running game asks first, in the footer.

## What it does

- **Settings** are written to `dhd.ini` the way `port/hle/host.c` does (`ini.rs`): it replaces a
  key's first line or appends one, and keeps comments and unknown keys. The tabs are:
  - Graphics: scale, `DHD_ASPECT`, `DHD_PRESENT_FILTER`, fps, vsync, textures, font;
  - Display: window, width and height, widescreen;
  - Game: iso via a file dialog, skip_movies, vibration, auto_update, launcher;
  - Online: name, server.

  Where `dhd.ini` has no line for a key, the launcher shows the game's player defaults.
- **Rider name** is `name`: up to 12 of A-Z, 0-9, space and dash, the same rule as
  `online_menu.c`. It can be edited in place on the rider card or on the Online tab.
- **Updates** (`update.rs`) follow the same rules as `port/hle/update.c` (finding 054):
  1. The source is update_url, or the GitHub releases API, with a latest.json asset or a zip and
     its digest.
  2. The zip goes to `dhd-update/` and its SHA-256 is checked.
  3. The files of the zip's one folder are copied over the game's. dhd.ini, the logs and folders
     are never touched.
  4. A file in use (the launcher itself) is renamed `NAME.old` first and deleted at the next
     start.
  5. The launcher then restarts itself.

  `auto_update`: ask shows Update in the footer, on installs without asking, off skips the check.
- **Patch notes** are the release body from GitHub. Everything above `## Commits` is shown, as a
  small markdown subset. The Play page lists up to four bold section titles, each with the first
  sentence of its first bullet, and the release date (`published_at`).
- **Disc image.** `disc.rs` makes the game's own check (`host.c disc_problem`): an ISO 9660
  volume whose SYSTEM.CNF boots SCUS_971.77. It skips the game's last step, hashing the program
  and data. Without a usable `iso`, the Play page shows the two first-run steps and the primary
  button is Choose disc. The file dialog refuses a disc the game would refuse, with the game's
  words.
- **Race server.** The Play page shows the server's public lobby list (`GET /v1/lobbies` on
  dhd.ini's `server`, or wss://dhd.skuza.app): the public races and the riders in them. Private
  rooms aren't listed. When the server's `minVersion` is newer than the launcher's, that is shown
  instead.
- **Settings details.** Window size is greyed out unless Display is Window. Texture pack is greyed
  out when no pack is found (`texture_pack`'s folder, else textures/SCUS-97177/replacements).
  Each tab has Reset tab (the player defaults; the disc and the name stay).
- **PLAY** starts `dhd.exe` with `DHD_LAUNCHER=1 DHD_AUTO_UPDATE=off` and minimizes the launcher.
  When the game exits, the launcher comes back. The online menu's Update Now exits the game with
  `UPDATE_EXIT_CODE`; the launcher then checks and installs.
- **Controller** (`ui/pad.js`, the browser's Gamepad API):
  - the d-pad or left stick moves the focus; in Settings, up and down go row by row and land on
    the chosen option;
  - A (Cross) presses, B (Circle) goes back to PLAY, LB/RB (L1/R1) change the settings tab, and
    Start presses the primary button;
  - the first pad press shows the focus ring and the button hints in the footer, in Xbox letters
    or PlayStation shapes by the pad's name; a mouse click hides them.
- **Discord** opens https://discord.gg/XrSeTyzfr2. Only the project's own addresses can be
  opened.

## Game side

- `update.c start_launcher`: dhd.exe started without `DHD_LAUNCHER` opens `DownhillRevived.exe`
  when it is beside it, then exits. This happens before its own update check, so old shortcuts
  open the launcher. It doesn't happen with `launcher = off` (`DHD_LAUNCHER_MODE`), with
  `DHD_UPDATE_NOW`, or with any tool variable (`automated()`).
- `update_request` exits with `UPDATE_EXIT_CODE` under the launcher, as under `DHD_RELAUNCHED`.
- The dhd.ini template documents `launcher = on`.

## Release

- `port/tools/build_launcher.py` runs `npx tauri build --no-bundle` with the renderer's flags
  (`+crt-static`, CARGO_HOME and RUSTUP_HOME remapped). It refuses an exe that names a folder of
  this machine or was built with another version (the version is read from version.h by build.rs).
- `package_release.py` builds the launcher (or takes `--launcher PATH`), checks its DLLs and its
  bytes (no retail ELF runs; no hosts beyond the official ones) and puts it in the zip.
- THIRD-PARTY-NOTICES now also lists the launcher's crates.
- MPL-2.0 is allowed for the HTML parser crates Tauri uses (cssparser, selectors, dtoa-short,
  option-ext), unmodified. A crate that ships no MPL text (selectors) is listed under the copy
  another crate ships.
- Tauri's `compression` feature is off: brotli's alloc crates have no licence file, and the UI is
  small.

## Checks

- `cargo test` covers:
  - dhd.ini round trip;
  - versions and relative URLs;
  - a full download, checksum and install from a local HTTP server, where dhd.ini survives;
  - with `-- --ignored`, the live GitHub source (0.5.0, 19,010,349 bytes, notes).
- The front end was screenshotted headless with a mocked Tauri API: all pages, the update state
  and the online tab.
- The real exe was captured off-screen with PrintWindow. It read dhd.ini (rider name), fetched
  the live notes and showed the no-dhd.exe state.
- `update.c` and `host.c` pass a clang-cl syntax check. The game hasn't been rebuilt or run with
  the launcher yet.

## Open

- **macOS and Linux.** These need the game builds first; the launcher code is ready for them.
  Signing applies there: Gatekeeper notarization on macOS, AppImage on Linux.
- **Online player count.** Only public races are counted; a count of all riders online would
  need a new server endpoint.
- **Untested flows.** The Update Now flow under the launcher, and a real update from 0.5.0 to the
  first release that has the launcher, still need a run.
