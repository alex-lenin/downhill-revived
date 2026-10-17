# Downhill Revived: notes for contributors and coding agents

These rules apply to everyone who changes this repository, people and AI agents alike.

Read `docs/STATE.md` first: it says where each part of the project stands. Findings go in
`docs/findings/NNN-*.md`; update `docs/STATE.md` when a step is done.

## Rules

- A function counts as decompiled only when its compiled bytes match the retail
  executable exactly, as checked by the tools (`tools/try_func.py`, `tools/promote.py`,
  `tools/check_image.py`). Never weaken the check.
- Never commit game data: no ISO, executable, IRX modules, extracted assets, generated asm,
  captures, saves or BIOS files.
- Never commit a key or other secret, or a server address other than the official server
  (`wss://dhd-online.downhill-revived.workers.dev`, the game's default). Any other server lives
  only in a player's `dhd.ini` or the `DHD_NET_SERVER` environment variable.
- Readable code is the goal: real names, structs and types once understood.
- Port changes go in `port/` and apply at build time; `src/` must keep matching.

## Research

Search online before inventing a solution. Most PS2 problems are already solved somewhere:
- PCSX2 (source and wiki) for how the hardware behaves: GS, VU0/VU1, VIF, DMA, SPU2, IPU,
  timers.
- ps2sdk, ps2tek and Sony SDK documentation for libraries and protocols.
- Other PS2 decompilation, static recompilation and porting projects (ps2recomp, Play!,
  other decomps) for tools and approaches.
- vgmstream and similar projects for game file formats.

Note in the relevant `docs/findings/` file what you used. Code may be adapted only when
its licence allows it (it must be compatible with GPL-3.0-or-later); keep its copyright notice
and say where it came from.
