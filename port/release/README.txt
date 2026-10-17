Downhill Revived - a PC port of Downhill Domination (PS2)
=========================================================

This is a fan project. It is not made, endorsed or supported by Sony Interactive Entertainment,
Incognito Entertainment or anyone who holds rights to the game. It contains no game data: the
game reads its program data, graphics, sound and everything else from your own copy of the disc
each time it starts.

What you need
- Windows 10 or 11 (64-bit), a graphics card with Direct3D 11.
- Your own dump of the Downhill Domination USA disc (SCUS-97177) as an .iso file. Other regions
  and modified images are refused.

How to play
1. Unzip this folder anywhere you can write to (not Program Files).
2. Run dhd.exe.
3. The first time, the game asks for your disc image: choose your .iso. It is checked and
   remembered in dhd.ini (the game writes this file then); you are only asked again if the file
   moves or changes.

Controls: a controller works out of the box (Xbox-style XInput pads with vibration, DualSense and
DualShock 4 over USB or Bluetooth). Keyboard: arrow keys, Enter = START, Backspace = SELECT,
X = Cross, C = Circle, Z = Square, S = Triangle, Q/W = L1/R1, 1/2 = L2/R2.

Settings: OPTIONS > PC SETTINGS in the game's menu (resolution, display mode, window size,
picture shape, scaling, widescreen, skip movies, vibration, texture pack). Changes are saved to
dhd.ini at once.

Texture packs (upscale mods)
Texture replacement packs made for PCSX2 work unchanged (PNG files named by PCSX2's texture
hash). Packs are made by their authors and are not part of this game; get one yourself. Either
- put the pack's files in a folder textures\SCUS-97177\replacements beside dhd.exe, or
- name the pack's folder in dhd.ini (its full path), e.g. the one PCSX2 uses:
    texture_pack = <Documents>\PCSX2\textures\SCUS-97177\replacements
OPTIONS > PC SETTINGS > Texture Pack turns it off or on (at the next start); in dhd.ini that is
"textures = off" or "on". Replacements load in the background the first time a texture shows,
so the game's own texture can show for a moment first. texture_cache_mb = N sets how much
graphics memory they may use (default: half the card's, at most 2048).

Files the game writes beside dhd.exe
- dhd.ini       your settings, one "key = value" per line.
- saves\card1\  memory card 1: your saves. Copy this folder to back them up.
- dhd.log       messages from the last run. Attach it when you report a problem.
- dhd.exe.old   the previous version, for a moment after an update; the next start deletes it.

Updates
At start the game looks for a newer version. If there is one it asks: "Update now" downloads
it, checks it, puts it in place and starts the game again; "Later" starts this version. Without
a network the game just starts. An update replaces dhd.exe and the text files only: dhd.ini and
saves\ are never touched. You can also update by hand: copy the files of a newer version over
your folder (a release holds no dhd.ini and no saves, so yours are kept).
In dhd.ini:
    auto_update = ask    ask before updating (the default); on: update without asking;
                         off: never look
    update_url =         where to look; empty means the official releases on GitHub. It can
                         name a latest.json of your own (version, url of the zip, its sha256).

Online races (experimental)
Choose ONLINE in the main menu: create a public or private race, or join one from the list or
with its code. It works out of the box, on the project's official server
(wss://dhd-online.downhill-revived.workers.dev). Every player needs the same USA disc image,
and the newest version of the game: an older one is told "A new version is out. Update to play
online." and offered Update Now.
Running your own server is optional: put its address on the "server =" line of dhd.ini, e.g.
    server = wss://races.example.org
A server of your own can keep an older version playing (its MIN_VERSION setting); then point
update_url at your own latest.json, or set auto_update = off.

Known gaps
- Movies are shown 4:3 in widescreen, as on the PS2.
- The 2-4 player split screens have not been checked in widescreen.
- Online races have been tested with two players, not yet with four people on four PCs.

Source code, building and bug reports: see the project's repository.
Licence: GPL-3.0-or-later (LICENSE.txt). Third-party notices: THIRD-PARTY-NOTICES.txt.
The original game, its data, names and trademarks belong to their owners.
