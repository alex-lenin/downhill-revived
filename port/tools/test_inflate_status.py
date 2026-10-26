"""Prove the explicit inflate status against i686's original EAX behavior.

Uses the actual game function and port patch with synthetic block decoders,
without game data. The unfixed wasm is a negative control.
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
#ifdef __wasm__
__attribute__((export_name("run_case")))
#endif
u32 run_case(u32 type) {
    int done=0;
    D_0029DCD4=(type<<1)|1; D_0029DCD0=3;
    return ((int (*)(int *))func_00241E70)(&done);
}
'''
DRIVER = r'''
#include <stdio.h>
#ifdef NATIVE
unsigned run_case(unsigned);
#define RUN(k) run_case(k)
#else
#include "probe.h"
#include "wasm-rt-impl.h"
static w2c_probe p;
#define RUN(k) w2c_probe_run_case(&p,k)
#endif
int main(void) {
#ifndef NATIVE
    wasm_rt_init(); wasm2c_probe_instantiate(&p);
#endif
    for (unsigned k=0;k<4;k++) printf("%u %08x\n",k,RUN(k));
    return 0;
}
'''


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
    driver=OUT/'driver.c'; driver.write_text(DRIVER)
    old=OUT/'original.c'; old.write_text(HEADER+function(original)+ENTRY)
    reference=harness.native('x86',[old,driver],OUT/'i686.exe',defines=['NATIVE'])
    expected={0:[10],1:[20],2:[30],3:[3]}
    assert reference==expected,reference
    results={}
    for name,text in [('original',original),('fixed',fixed)]:
        out=OUT/name; out.mkdir(exist_ok=True)
        src=out/'input.c'; src.write_text(HEADER+function(text)+ENTRY)
        wasm=out/'probe.wasm'
        run([harness.WASI/'bin/clang.exe','--target=wasm32','-O2','-nostdlib',
             '-Wno-incompatible-function-pointer-types','-Wl,--no-entry',src,'-o',wasm])
        run([harness.WABT/'bin/wasm2c.exe',wasm,'-n','probe','-o',out/'probe.c'])
        results[name]=harness.native('x64',[driver,out/'probe.c',harness.RT/'wasm-rt-impl.c',
                                           harness.RT/'wasm-rt-mem-impl.c'],out/'run.exe',
                                    extra=['-I'+str(out),'-I'+str(harness.WABT/'include'),'-I'+str(harness.RT)])
    assert results['original']!=reference,results
    assert results['fixed']==reference,results
    print('PASS: original i686 statuses 10/20/30/3; unfixed wasm differs; explicit returns match all four')


if __name__=='__main__':
    main()
