# 017 — PC data conversion and link census

Plan steps 3 and 4 (finding 015). Tools: `port/tools/data.py` converts the data, and
`port/tools/link.py` renames symbols, links, and writes the census. Run `port/tools/build.py`
first. The output goes to `build/port/link/`: `dhd.elf` (32-bit x86, linked with
`--noinhibit-exec`, not runnable yet) and `census.json`.

## Data
The six splat data files (`asm/data/*.s`, generated from your own ELF) become x86 assembler at
build time:
- `.word` becomes `.long`.
- `.align N` becomes `.p2align N`.
- `.sdata`/`.sbss` join `.data`/`.bss`.
- `.gcc_except_table` goes to `.rodata`.

Both CPUs are little-endian and values stay symbolic, so the layout is the PS2's. Three kinds
of references need special handling:
- **Code labels** (`.L…` switch tables and `$LEH_…` exception ranges of MIPS code, about 6,100
  entries) become 0. Only MIPS code read them.
- **`D_<small hex>`** (`D_1010`, `D_50`, …) are absolute values, such as VU1 memory addresses
  passed to the packet builders. They become absolute symbols with the same value.
  `D_8A0400` is the end of the PS2 `.bss`, where the heap starts: it is linked to `_end`.
- **`D_` addresses inside `.vutext`** become labels into `assets/vutext.bin` (the VU1
  microcode, extracted by splat), which is included whole.

616 data objects are defined by the C sources, so the converted data leaves them out and the C
definition is used. When C defines an object whose neighbour the code reaches by pointer
arithmetic (e.g. `STR8` in 001BA3B8), that pairing breaks. Nothing detects this yet.

## Symbols
- **C++ names.** A C++ function's mangled name depends on the declaration its unit happened to
  see: a caller declares `func_X(Obj*)` while the definition is `Obj::func_X(int)`, or the
  function sits in a per-unit namespace. The data and the C units only know `func_X`. Every
  mangled symbol whose demangled identifier is a retail name (`func_XXXXXXXX`/`D_XXXXXXXX`) is
  renamed to that name with `llvm-objcopy --redefine-syms`. That covers 909 symbols, and the
  merged-unit mangling problem is gone.
- **Tentative definitions.** gcc 2.95 merged C tentative definitions (`T x;` in two units);
  clang needs `-fcommon` for that (one case: `D_0029DE30`).
- **varargs.** Two units define the EE's `va_start` with gcc 2.95 MIPS builtins; they are
  patched to `__builtin_va_start` (`port/patches.py`).

## Census (2026-10-12)
3,797 units + the asm runtime + the data link with **0 duplicate definitions and 0 other errors**.
493 symbols stay undefined, and every one is accounted for:

| count | kind | what replaces it |
|------:|------|------------------|
| 294 | SDK functions still in asm (0x243000–0x263BC0) | HLE runtime (plan step 6) |
| 23 | named SDK library functions: libmpeg 17, libkernl 4, libgraph 2 | HLE |
| 7 | callback entry points inside asm SDK functions (`D_00260898`, …), passed as function pointers | HLE, with their functions |
| 114 | libgcc functions still in asm (0x1FC000–0x201000), mostly referenced from data tables | the host compiler's runtime |
| 26 | C library / libgcc / C++ runtime by name (`memcpy`, `strcmp`, `__dynamic_cast`, …) and the runtime's own libc calls | the host C library |
| 29 | game (11), engine (17) and crt0 (1) functions not decompiled: queue, hand-written, remnants | the decomp queue, or static recompilation (plan step 5) |

So, apart from the 29 unfinished functions, everything left is platform work: the SDK and the C
library. The game's own code and data link completely.

## Windows target (2026-10-13)
The link now targets 32-bit Windows (`port/tools/pctarget.py`): clang's `i686-pc-windows-itanium`
for the game, and `i686-pc-windows-msvc` for host code such as the asm runtime. lld-link links
against the MSVC C runtime into `dhd.exe`. The reasons:
- **Layout.** windows-itanium lays out structs and bitfields as gcc does. A test struct had
  the same offsets as on i686 Linux, while the MSVC ABI moved them (bitfields 7→16, size 32→48).
- **Calling convention.** Under the Itanium C++ ABI, C++ methods on this triple use cdecl. The
  mingw and MSVC triples use thiscall, and that would break C callers and the game's function
  tables.

Win32 C symbols carry a `_` prefix, which asm labels bypass. The transform therefore prefixes
every asm label, and the data stage writes COFF assembler with prefixed names. clang emits
separate complete and base constructors (C1/C2) and destructors (D0/D1/D2) on COFF, where Linux
uses aliases, so only C1/D1 take a class-named function's retail name. A variant that is only
called (its base class lives in C or asm) is still renamed.

The MSVC C runtime resolves the C library calls except `fabsf`/`floorf`, which are header
inlines on x86 MSVC. Census: 474 undefined, 0 duplicates.
