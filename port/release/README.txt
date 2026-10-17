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

Controls
A controller works out of the box: Xbox pads, DualSense, DualShock 4, Switch Pro, 8BitDo and most
other USB or Bluetooth pads (through SDL3 and its controller database), with vibration where the
pad has motors. The buttons go by position, as on a PlayStation pad: bottom = Cross, right =
Circle, left = Square, top = Triangle. A pad SDL doesn't know can be added with a line in SDL's
format in gamecontrollerdb.txt beside dhd.exe.

No controller? The keyboard plays a whole race (only while the game's window is in front; it
works together with a controller):
    Arrow keys       move in menus; steer and lean while riding (they tilt the stick
                     gradually, so a short tap steers a little and holding steers fully)
    Space or X       Cross: confirm; pedal (press twice: sprint)
    Esc, Backspace   Triangle: back in menus. While riding Esc pauses, Backspace brakes
    S                Triangle: brake (in the air: tweak a trick)
    Enter            START: start the race on the race setup screen, pause
    Tab              SELECT: hold + up/down changes the camera, with Q looks back,
                     press twice to bail out
    Z / C            Square / Circle: punch / kick
    A / D            L1 / R1: tricks (both together: Superman)
    Q / E            L2 / R2: tricks; E (R2) is also bunny hop
    W                R3: sprint
    F                L3: powerslide
Letters are places on the keyboard as on a US one: on AZERTY or QWERTZ use the keys in the same
places (e.g. Square is the key left of X, wherever its label says Z, W or Y).
The game's own controls screen (OPTIONS > CONTROLS) and its trick lists name the PlayStation
buttons; the keys above stand for them.

Settings: OPTIONS > PC SETTINGS in the game's menu (resolution, display mode, window size,
picture shape, scaling, widescreen, skip movies, vibration, texture pack, font). Changes are
saved to dhd.ini at once.

Font: the game's text is drawn with sharp edges at any resolution ("font = sharp", the
default). The sharp font is made in memory from the font on your disc each time the game
starts; nothing is saved. "font = original" (PC SETTINGS > Font, at the next start) draws it as
the PS2 does. A texture pack that replaces the font wins over both.

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
