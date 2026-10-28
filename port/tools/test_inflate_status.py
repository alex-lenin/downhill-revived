"""The inflate block status (func_00241E70) as the PS2 returns it, from the patched game function.

The decompiled body is void; its callers read $v0. The retail code (asm func_00241E70.s) returns
the block decoder's $v0 for block types 0, 1 and 2 (the jal is its last call) and, for the
invalid type 3, the 2 its last compare loaded into $v0 (the delay slot at 00241F50): gzip's
"bad block type". The patched unit must give exactly that; the unpatched body through wasm (its
in-TU bitcast returns the argument) is the negative control. Synthetic block decoders, no game
data. Scratch stays on F:.
"""
from pathlib import Path
import re
import sys

import test_wasm_postlink as harness

OUT = harness.ROOT/'build/w7h/inflate-status-test'
sys.path.insert(0,str(harness.ROOT/'port'))
from patches import PATCHES

HEADER = r'''
typedef unsigned int u32;
#define NOINLINE __attribute__((noinline))
unsigned int D_0029DCD4, D_0029DCD0;
const unsigned short D_00299440[] = {0,1,3};
volatile int calls;
NOINLINE int func_002413E8(void) { ++calls; return 10; }
NOINLINE int func_00241598(void) { ++calls; return 20; }
NOINLINE int func_002417E0(void) { ++calls; return 30; }
NOINLINE unsigned char func_00242058(void) { return 0; }
'''
ENTRY = r'''
__attribute__((export_name("run_case")))
u32 run_case(u32 type) {
    int done=0;
    D_0029DCD4=(type<<1)|1; D_0029DCD0=3;
    return ((int (*)(int *))func_00241E70)(&done);
}
'''
DRIVER = r'''
#include <stdio.h>
#include "probe.h"
#include "wasm-rt-impl.h"
static w2c_probe p;
int main(void) {
    wasm_rt_init(); wasm2c_probe_instantiate(&p);
    for (unsigned k=0;k<4;k++) printf("%u %08x\n",k,w2c_probe_run_case(&p,k));
    return 0;
}
'''
# the decoders' statuses for types 0-2, and the retail $v0 (2) for the invalid type 3
EXPECTED = {0: [10], 1: [20], 2: [30], 3: [2]}


def function(text):
    start=re.search(r'(?:void|int) func_00241E70\(int \*e\)\s*\{',text).start()
    end=text.index('\n}',start)+2
    return 'NOINLINE '+text[start:end]


def main():
    OUT.mkdir(parents=True,exist_ok=True); harness.OUT=OUT
    run=harness.run
    original=(harness.ROOT/'src/unsorted/00240270.c').read_text()
    fixed=original
    for kind,a,b in PATCHES['unsorted/00240270.c']:
        if kind=='text' and ('func_00241E70' in a or '    if (t == 0)' in a):
            assert fixed.count(a)==1
            fixed=fixed.replace(a,b)
    results={}
    for name,text in [('original',original),('fixed',fixed)]:
        out=OUT/name; out.mkdir(exist_ok=True)
        src=out/'input.c'; src.write_text(HEADER+function(text)+ENTRY)
        wasm=out/'probe.wasm'
        run([harness.WASI/'bin/clang.exe','--target=wasm32','-O2','-nostdlib',
             '-Wno-incompatible-function-pointer-types','-Wl,--no-entry',src,'-o',wasm])
        results[name]=harness.through_wasm2c(wasm,name+'-run',DRIVER)
    assert results['original']!=EXPECTED,results
    assert results['fixed']==EXPECTED,results
    print('PASS: retail statuses 10/20/30 and 2 (invalid type); unfixed wasm differs; the patched returns match all four')


if __name__=='__main__':
    main()
