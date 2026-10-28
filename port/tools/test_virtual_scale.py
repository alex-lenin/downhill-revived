"""Execute the real assembly scale getter through its native float vtable ABI."""
from pathlib import Path
from unittest.mock import patch
import json
import recomp
import testprog


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
    for negative in (False, True):
        test_sig = dict(sig, ret='i32') if negative else sig
        with patch.object(recomp, 'OUT', str(out)):
            recomp.write(name, {name}, {name: test_sig})
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
 unsigned failures=0;
 table[5].function=(void *)func_00261E48;
 for(unsigned i=0;i<16;i++) {
  volatile float result=((float (*)(void *))table[5].function)((void *)0x12340000);
  failures += result != 1.0f;
 }
 printf("virtual scale: wrong=%u\n",failures);
 return failures != 0;
}
''')
        # The wrapper with the stale int return has another wasm type than the float vtable call:
        # the call traps (exit 134), as the core's would without a matching declaration.
        exe = testprog.guest('virtual-scale-' + ('wrong' if negative else 'correct'),
                             [out / (name + '.c'), driver], out=out / ('wrong' if negative else 'correct'))
        testprog.run(exe, expect=134 if negative else 0, quiet=negative)
    print('Virtual scale getter: real wrapper, stale cache, vtable result; the stale int return traps')


if __name__ == '__main__':
    main()
