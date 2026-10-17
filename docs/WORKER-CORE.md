# Worker core: the rules (read completely)

Goal: C (or C++) that compiles with the retail toolchain to **exactly** the retail bytes of your
function. Only `tools/try_func.py` decides: `MATCH FUNC` and `whole image: MATCH`. Never claim a
match it did not print.

## Loop

1. Your prompt names the function, your candidate dir, your `--tag`, and usually the closest
   earlier attempt and its current diff. Continue from that attempt when there is one;
   otherwise read `asm/nonmatchings/text/FUNC.s` and draft with
   `python tools/ext/m2c/m2c.py --target mipsee-gcc-c asm/nonmatchings/text/FUNC.s`.
2. If `grep -l "INCLUDE_ASM.*FUNC)" src/unsorted/*` finds a file whose UNIT range holds your
   function, copy that whole file as your candidate and replace that line with your C.
3. Test: `python tools/try_func.py FUNC CAND --tag TAG --compact` (one per shell command).
   `--compact` shows the first 10 differing words; `--full` only when you need the whole listing.
4. Edit the candidate directly and test again. Read files in ranges (`sed -n`, `grep`), not whole.

## Budget (progress-gated)

Start with **20** try_func runs. Continue in steps of **10** (up to the limit in your prompt) only
when the last 10 runs improved the best diff, or you have a new hypothesis about which compiler
pass decides the difference (write one line `EXTEND: <reason>` before continuing). If three
different C variants give byte-identical output, that kind of change doesn't reach the compiler:
switch to another kind or stop. When stopping, leave the closest **clean** attempt in the
candidate file.

## Allowed and forbidden

- Inline asm only for what gcc 2.95 cannot emit: VU0 macro instructions, MMI, EE FPU extras
  (`mula.s`/`madda.s`/`madd.s`/`sqrt.s`/`rsqrt.s`/`min.s`/`max.s`), privileged instructions,
  `COPY_VECTOR`, and the proven helper forms (WORKER.md "The game's vector helpers"), which
  include the game's volatile `Abs` (`abs.s`) helper in its one exact form (finding 013) and
  libgcc's longlong.h `umul_ppmm` (finding 014).
  Every operand must be used and every clobber really written.
- Forbidden: steering tricks (fake clobbers, unused operands, empty asm or label barriers,
  `volatile` variables only to steer codegen), `register ... asm("$N")`, inline asm for
  anything gcc can generate, compiler flags other than a UNIT marker's allowed ones
  (WORKER.md "Files built with other flags"), editing anything outside your candidate dir
  (`build-try/sweep/` is allowed for tools/sweep.py), git.
- A try_func `WARNING` about inline asm means final verification rejects the file: drop that
  approach.

## Work quietly

No plans or narration between tool calls; spend the effort on reasoning and try_func runs.

## Handoff notes (always, MATCH or FAIL)

Overwrite `CAND_DIR/FUNC.notes.md`, at most 12 lines:

    best: <file> <N>/<M> words (clean: yes/no)
    attempts: <try_func runs this session>
    differs: <what still differs, e.g. "s0/s1 swapped in loop", "sched2 order of lw x30">
    pass: <compiler pass that decides it, if found (passdump/regalloc)>
    tried: <rejected hypotheses, one per line, short>
    next: <the experiment you would run next>

## Report

End with one line: `FUNC MATCH|FAIL FILE attempts=N`.

## Reference (open the section you need, not the whole file)

docs/WORKER.md: "The toolchain" (gp/sdata, types, store order, switch, C++ vtables), "Data your
function owns" (`--data` ranges), "Several functions in one file", "Unoptimized (-O0) files",
"Files built with other flags", "C++", "Register and order near misses" (regalloc, passdump,
adiff, sweep), "When try_func shows no listing", "VU0 / MMI instructions" + proven helper table,
"Style". docs/findings/012-sn-gcc-matching-levers.md: C forms that steer this compiler, by kind
of diff (register allocation, store order, delay slots, cse/combine, loops, min/max, booleans).
