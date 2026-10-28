"""Link and execute canonicalized C++ definitions/imports/data/table entries.

The unrenamed object must fail to resolve its C companion. The renamed
module must give the probe's sum, 54 (below), with relocation/code/data bytes
untouched by renaming.
"""
from pathlib import Path
import os
import subprocess

import test_wasm_postlink as harness
from build_host64 import LLVM
from wasm_postlink import Module
from wasm_retail_names import rename

OUT = harness.ROOT / 'build/w7h/retail-names-test'
CPP = r'''
#define RETAIL
namespace internal { int D_00123458 = 3; }
RETAIL int func_00123456(int x) { return x + 7 + internal::D_00123458; }
RETAIL int func_00123457(int);
RETAIL int func_00123462__as_i_i(int);
extern int func_00123460(char *) asm("func_00123460");
extern int func_00123461(int, int) asm("func_00123461");
RETAIL int func_00123460(int *p) { return *p + 2; }
RETAIL int func_00123461(int x) { return x + 11; }
typedef int (*Fn)(int);
Fn volatile entry = func_00123456;
__attribute__((export_name("probe")))
extern "C" int probe() {
    int value=3;
    /* 15 (5+7+3, through the table) + 15 (the C companion) + 5 (3+2) + 13 (2+11: the adapter drops
       the 99) + 6 (the thunk's C definition): 54 */
    return entry(5) + func_00123457(5) + func_00123460((char *)&value) + func_00123461(2,99) + func_00123462__as_i_i(3);
}
'''
COMPANION = '''int func_00123457(int x) { return x * 3; }
int func_00123462__as_i_i(int x) { return x * 2; }
int func_00123461(int);
int func_00123461__as_ii_i(int x,int ignored) { return func_00123461(x); }
'''
DRIVER = r'''
#include <stdio.h>
#include "probe.h"
#include "wasm-rt-impl.h"
static w2c_probe p;
int main(void) {
    wasm_rt_init(); wasm2c_probe_instantiate(&p);
    printf("0 %08x\n", w2c_probe_probe(&p));
    wasm2c_probe_free(&p); wasm_rt_free();
    return 0;
}
'''


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    harness.OUT = OUT
    run = harness.run
    cpp, c, driver = OUT/'input.cpp', OUT/'companion.c', OUT/'driver.c'
    cpp.write_text(CPP); c.write_text(COMPANION); driver.write_text(DRIVER)
    obj, co, wasm = OUT/'input.o', OUT/'companion.o', OUT/'probe.wasm'
    clang = harness.WASI/'bin/clang.exe'
    run([clang, '--target=wasm32', '-O2', '-fno-exceptions', '-c', cpp, '-o', obj])
    run([clang, '--target=wasm32', '-O2', '-c', c, '-o', co])
    link = [clang, '--target=wasm32', '-nostdlib', '-Wl,--no-entry', obj, co, '-o', wasm]
    rejected = subprocess.run(list(map(str,link)), capture_output=True, text=True,
                              env=dict(os.environ,TEMP=str(OUT),TMP=str(OUT)))
    assert rejected.returncode and any(n in rejected.stderr for n in ('func_00123457','func_00123461')), rejected.stderr
    before = Module(obj.read_bytes())
    saved = obj.read_bytes()
    try:
        rename(obj, LLVM/'llvm-cxxfilt.exe')
    except ValueError as error:
        assert 'incompatible signatures' in str(error)
    else:
        raise AssertionError('a mismatched local declaration needs an adapter')
    assert obj.read_bytes()==saved
    thunks = {}
    assert rename(obj, LLVM/'llvm-cxxfilt.exe', thunks=thunks) >= 3
    assert thunks=={('func_00123461',(('i32','i32'),'i32')):(('i32',),'i32')},thunks
    after = Module(obj.read_bytes())
    assert [(i,p,n) for i,p,n in before.sections if i and i!=2] == [
            (i,p,n) for i,p,n in after.sections if i and i!=2]
    saved = obj.read_bytes()
    assert rename(obj, LLVM/'llvm-cxxfilt.exe') == 0 and obj.read_bytes()==saved
    run(link)
    run([harness.WABT/'bin/wasm-validate.exe', wasm])
    got = harness.through_wasm2c(wasm, 'run', DRIVER)
    assert got=={0:[54]}, got
    print('PASS: unrenamed link fails; definitions/imports/data/table and merged aliases give 54; explicit adapter required for mismatched aliases; code/data unchanged; idempotent')


if __name__ == '__main__':
    main()
