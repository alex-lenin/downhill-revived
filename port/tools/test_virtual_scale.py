"""Execute the real assembly scale getter through its native float vtable ABI."""
from pathlib import Path
from unittest.mock import patch
import json
import subprocess
import recomp
from pctarget import HOST_TARGET


def main():
    root = Path(recomp.ROOT)
    out = root / 'build/port/runtime/virtual-scale'
    out.mkdir(parents=True, exist_ok=True)
    name = 'func_00261E48'
    (out / 'signatures.json').write_text(json.dumps({name: dict(
        ret='i32', args=['i32']*8, variadic=False, **{'from': 'old fallback'})}))
    (out / 'signatures-version.txt').write_text(recomp.SIGNATURE_VERSION)
    with patch.object(recomp, 'OUT', str(out)):
        sig = recomp.signatures([name])[name]
    assert sig['ret'] == 'float' and sig['args'] == ['i32'], sig
    flags = ['--target='+HOST_TARGET, '-O1', '-w', '-msse2', '-mfpmath=sse',
             '-I', str(root / 'port/runtime')]
    for negative in (False, True):
        test_sig = dict(sig, ret='i32') if negative else sig
        with patch.object(recomp, 'OUT', str(out)):
            recomp.write(name, {name}, {name: test_sig})
        target = out / (name+'.c')
        obj = out / 'target.o'
        subprocess.run(['clang', *flags, '-c', str(target), '-o', str(obj)], check=True)
        driver = out / 'driver.c'
        driver.write_text(r'''
#include "recomp.h"
#include <stdio.h>
ps2_cpu ps2;
void (*ps2_graphics_sync)(void);
void (*ps2_intc_read)(void);
void ps2_recomp_enter(void) {}
extern float func_00261E48(uint32_t);
typedef struct { short delta,index; void *function; } Slot;
static Slot table[6];
int main(void) {
 unsigned env[7], failures=0;
 table[5].function=(void *)func_00261E48;
 __asm__ volatile("fninit");
 for(unsigned i=0;i<16;i++) {
  volatile float result=((float (*)(void *))table[5].function)((void *)0x12340000);
  failures += result != 1.0f;
 }
 __asm__ volatile("fnstenv %0\n\tfldenv %0" : "=m"(env) : : "memory");
 unsigned bad = (env[1]&0x41) != 0 || (env[2]&65535) != 65535;
 __asm__ volatile("fninit");
 printf("virtual scale: wrong=%u SW=%04X TAG=%04X\n",failures,env[1]&65535,env[2]&65535);
 return NEGATIVE ? !(bad && failures==16) : (bad || failures);
}
'''.replace('NEGATIVE', str(int(negative))))
        exe = out / ('wrong.exe' if negative else 'correct.exe')
        subprocess.run(['clang', *flags, str(driver), str(obj), '-o', str(exe)], check=True)
        subprocess.run([str(exe)], check=True)
    print('Virtual scale getter: real wrapper, stale cache, vtable result and x87 negative/positive cases pass')


if __name__ == '__main__':
    main()
