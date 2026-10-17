"""Execute a recompiled call to a native GCC mode(DI) division helper."""
from pathlib import Path
import json
import subprocess
from unittest.mock import patch

import recomp

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/runtime/scalar-abi'


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    decl = OUT / 'helper.c'
    decl.write_text('''typedef unsigned int Word __attribute__((mode(DI)));
typedef Word Wide;
typedef float Rate;
Wide func_00123400(Wide numerator, Wide denominator) { return numerator / denominator; }
Rate func_00123410(Rate value) { return value; }
void func_00123420(void) { }
''')
    stale = {'func_00123400': dict(ret='i32', args=['i32', 'i32'],
                                  variadic=False, **{'from': 'helper.c'})}
    (OUT / 'signatures.json').write_text(json.dumps(stale))
    version = OUT / 'signatures-version.txt'
    if version.exists(): version.unlink()
    with patch.object(recomp, 'OUT', str(OUT)), patch.object(recomp, 'SRC', str(OUT)):
        sigs = recomp.signatures(['func_00123400', 'func_00123410', 'func_00123420'])
    assert sigs['func_00123400']['ret'] == 'i64'
    assert sigs['func_00123400']['args'] == ['i64', 'i64']
    assert sigs['func_00123410']['ret'] == 'float'
    assert sigs['func_00123410']['args'] == ['float']
    assert sigs['func_00123420']['args'] == [] and sigs['func_00123420']['ret'] == 'void'
    insns = [(0x1000, 'jal', ['func_00123400']), (0x1004, 'nop', [])]
    with patch.object(recomp, 'parse', return_value=(insns, set())):
        fn = recomp.translate('scalar_probe', set(), sigs)
    source = '''#include "recomp.h"
#include <stdio.h>
ps2_cpu ps2;
uint64_t func_00123400(uint64_t a, uint64_t b) { return a / b; }
int main(void) {
 static uint64_t cases[][2] = {{11,10},{31,10},{51,10},
  {0x123456789abcdef0ULL,10},{0xfedcba9876543210ULL,0x123456789ULL}};
 for(unsigned i=0;i<sizeof cases/sizeof *cases;i++) {
  GPR(4)=cases[i][0]; GPR(5)=cases[i][1];
''' + '\n'.join(fn.lines) + '''
  if(GPR(2)!=cases[i][0]/cases[i][1]) return 1;
 }
 puts("Scalar typedef ABI: 64-bit division calls, float alias and stale cache pass");
 return 0;
}
'''
    path = OUT / 'test.c'; path.write_text(source)
    exe = path.with_suffix('.exe')
    subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-fuse-ld=lld', '-O1',
                    '-I' + str(ROOT / 'port/runtime'), str(path), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


if __name__ == '__main__': main()
