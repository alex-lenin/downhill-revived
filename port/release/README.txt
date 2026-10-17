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
picture shape, scaling, widescreen, vibration). Changes are saved to dhd.ini at once.

Files the game writes beside dhd.exe
- dhd.ini       your settings, one "key = value" per line.
- saves\card1\  memory card 1: your saves. Copy this folder to back them up.
- dhd.log       messages from the last run. Attach it when you report a problem.
To update, copy the files of a newer version over your folder. A release holds no dhd.ini and
no saves, so yours are kept.

Online races (experimental)
Choose ONLINE in the main menu: create a public or private race, or join one from the list or
with its code. It works out of the box, on the project's official server
(wss://dhd-online.downhill-revived.workers.dev). Every player needs the same USA disc image.
Running your own server is optional: put its address on the "server =" line of dhd.ini, e.g.
    server = wss://races.example.org

Known gaps
- Movies are skipped (no MPEG-2 decoder yet).
- In 16:9 the HUD is stretched.

Source code, building and bug reports: see the project's repository.
Licence: GPL-3.0-or-later (LICENSE.txt). Third-party notices: THIRD-PARTY-NOTICES.txt.
The original game, its data, names and trademarks belong to their owners.
