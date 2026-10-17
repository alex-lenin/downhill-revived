# 001 — Compiler identification (superseded)

> **Resolved in `002-compiler-sn-prodg.md`: SN ProDG 3.01 (2.95.3 SN BUILD 1.36 + ps2eeas),
> `-O2 -G8 -fopt-stack`.** The SN "ruled out" verdicts below were wrong: `sq` saves are SN's
> default, `-fopt-stack` gives the retail `sd` frame. Kept for history.

Target: Downhill Domination (USA) `SCUS_971.77`, SYSTEM.CNF VER 1.00, NTSC.

## ELF facts
- Stripped: no `.symtab`, no `.mdebug`/DWARF. `.text` 0x159bc0 bytes at 0x10a000, entry 0x10a008.
- `.gcc_except_table` present → GNU toolchain, C++ with exceptions.
- `.vutext`/`.DVP.overlay.*` → SCE dvp-as VU microcode (~40 KB).
- Library tags `PsIIlib* 2700` → SCE SDK 2.7.0.
- `MOD/SNPROFIL.IRX` on disc (SN profiler) — does not imply SN compiler, see below.

## Prologue fingerprint (`tools/prologue_fingerprint.py <elf>`)
- 3,763 `addiu $sp,$sp,-N` prologues; `$ra` saved with `sd` in 3,107/3,107.
- Callee-saved `$s` regs: `sd` 9,770, `sw` 147, `sq` 0. 8-byte slots, scheduled into body.

## Candidates tested (decomp.me `compilers` release archives)
| Compiler | Host | `$s`/`$ra` save style on test fn | Verdict |
|---|---|---|---|
| ee-gcc2.95.2-273a / -274 (SN) | Win32 | `sq` | ruled out |
| ee-gcc2.95.3-107 / -114 (SN) | Win32 | `sq` | ruled out |
| ee-gcc2.95.3-136 (SN) | Win32 | `sq` + `sd $ra` | ruled out |
| ee-gcc3.2-030210-beta2 | Win32 (Cygwin) | not run: needs `cygwin1.dll` + `cygintl-2.dll` | **untested — test next** |
| ee-gcc2.9-990721 / 991111 / 991111a / 991111-01 (SCE) | Linux i386 | `sd`, but 16-byte slot stride (0,16,32), frame 48 | ruled out (target uses 8-byte stride) |
| ee-gcc2.96 (`2.96-ee-001003-1`, SCE) | Linux i386 | `sd`, 8-byte stride (0,8,16), frame 32 — same as target | **best so far** |

## Matching attempts (ee-gcc2.96, sources in `tools/match/m*.c`)
Target `func_10A3B0` (0x10a3b0, 16 insns). Grader: `tools/match/fsweep.sh`.
- `-O2 -G0 -fno-optimize-sibling-calls -fno-schedule-insns`: identical through the
  first `jal`; second call differs only by delay-slot choice
  (target `move a0,sp; jal; li a1,1`, ours `li a1,1; jal; move a0,sp`).
- `-O1`: second call matches, but prologue order differs (`sd ra` before `move v0,a0`).
- Plain `-O2` emits sibling calls (`j func`); target never does → sibcalls off
  (or a build where -O2 doesn't do them).
- Source variants (C, C++ methods, pointer `this`, bool/int args) don't change the
  delay-slot choice. 4 opt levels × 21 sub-flags × C/C++ → no exact match.

Hypotheses, in order: (1) ee-gcc 3.2 beta2 (Feb 2003, fits a mid-2003 SDK 2.7 game);
(2) another 2.96 build/patch (SN or SCE re-release) not on decomp.me;
(3) a flag combination not yet swept (`-fno-delayed-branch` ruled out: target has filled slots).

## Environment notes
- SCE Linux compilers are 32-bit: WSL Ubuntu needs `libc6-i386` (installed offline from
  launchpad deb; WSL mirrored networking is broken on this host → no apt).
- 32-bit compilers fail with EOVERFLOW on `/mnt/*`; scripts copy compilers to `/opt/eegcc`
  and work in `/opt/eegcc/work`.
- Invoke from PowerShell: `$env:WSL_UTF8=1; wsl -d Ubuntu -u root -- bash /mnt/<drive>/<repo>/tools/match/<script>`
  (Git Bash mangles `/mnt` paths and `$vars` in `wsl -- bash -c`).

## Archive SHA-256 (decomp.me `compilers` release)
ee-gcc2.96.tar.xz 0590d2ca9da8f5903889d66761220d14b47a8d14ba987ca53db84a1650a1fd0a
ee-gcc3.2-030210-beta2.tar.gz 5244521973d3fa9712cc9f945e6e45c489d69d846ca6639b0c07c3e885a923a1
ee-gcc2.9-991111-01.tar.xz ed684fd98f89d36b0121caab311052089103e3b36241fcef4338cc9ea41c75b8
