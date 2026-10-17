"""Generate redistributable ABI tests using synthetic EE callback instructions."""
import json
from pathlib import Path
from unittest.mock import patch

import recomp


def prepare(directory):
    out = Path(directory) / 'recomp'
    out.mkdir(parents=True, exist_ok=True)
    names = ['func_' + address for address in recomp.ANIMATION_HANDLERS]
    # Exercise the old on-disk declaration cache as well as wrapper generation.
    stale = {n: {'ret': 'void', 'args': ['i32', 'i32'], 'variadic': False,
                 'from': 'old callback declaration'} for n in names}
    (out / 'signatures.json').write_text(json.dumps(stale))
    (out / 'signatures-version.txt').write_text(recomp.SIGNATURE_VERSION + '\n')
    operations = [('mtc1', ['$a0', '$f1']), ('cvt.s.w', ['$f1', '$f1']),
                  ('add.s', ['$f0', '$f12', '$f12']), ('add.s', ['$f0', '$f0', '$f1']),
                  ('mtc1', ['$a1', '$f1']), ('cvt.s.w', ['$f1', '$f1']),
                  ('sub.s', ['$f0', '$f0', '$f1']), ('jr', ['$ra']), ('nop', [])]
    insns = [(0x1000 + i * 4, op, args) for i, (op, args) in enumerate(operations)]
    with patch.object(recomp, 'OUT', str(out)), patch.object(recomp, 'parse',
                                                          side_effect=lambda n: (insns, set())):
        sigs = recomp.signatures(names)
        for name in names:
            recomp.write(name, set(names), sigs)
    source = '#include "recomp.h"\n#include <stdio.h>\n#include <string.h>\n'
    source += 'ps2_cpu ps2;\nvoid (*ps2_graphics_sync)(void);\nvoid ps2_recomp_enter(void) {}\n'
    source += ''.join('#include "%s.c"\n' % n for n in names)
    source += 'typedef float (*Callback)(uint32_t, uint32_t, float);\n'
    source += 'static Callback callbacks[] = {%s};\n' % ', '.join(names)
    source += r'''int main(void)
{
    unsigned i, j;
    for (i = 0; i < sizeof callbacks / sizeof *callbacks; i++)
        for (j = 0; j < 65; j++) {
            float time = ((int)j - 32) * 0.125f;
            float expected = time * 2.0f + 7.0f - 11.0f;
            float actual;
            ps2.fpr[12] = 999.0f;
            ps2.fpr[0] = -999.0f;
            actual = callbacks[i](7, 11, time);
            if (memcmp(&actual, &expected, 4)) {
                fprintf(stderr, "callback %u case %u: %.9g != %.9g\n", i, j, actual, expected);
                return 1;
            }
        }
    puts("Recompiled animation callback ABI: 845 cases pass");
    return 0;
}
'''
    path = out / 'test_callbacks.c'
    path.write_text(source)
    return str(path)


def check_forwarders(directory):
    """A timer thunk must refresh a stale void(void) wrapper from its callee."""
    ops = [('addiu', ['$sp', '$sp', '-0x10']), ('sd', ['$ra', '0x0($sp)']),
           ('jal', ['timer_target']), ('nop', []), ('ld', ['$ra', '0x0($sp)']),
           ('jr', ['$ra']), ('addiu', ['$sp', '$sp', '0x10'])]
    insns = [(0x1000 + i*4, op, args) for i, (op,args) in enumerate(ops)]
    assert recomp.transparent_target(insns) == 'timer_target'
    bad = insns.copy(); bad[3] = (0x100c, 'addu', ['$a0', '$a1', '$zero'])
    assert recomp.transparent_target(bad) is None
    bad = insns.copy(); bad[6] = (0x1018, 'addiu', ['$v0', '$zero', '0'])
    assert recomp.transparent_target(bad) is None
    out = Path(directory) / 'thunk-test'; out.mkdir(exist_ok=True)
    stale = {'timer_thunk': dict(ret='void', args=[], variadic=False, **{'from':'stale'}),
             'timer_target': dict(ret='i32', args=['i32'], variadic=False, **{'from':'callee definition'})}
    (out / 'signatures.json').write_text(json.dumps(stale))
    (out / 'signatures-version.txt').write_text(recomp.SIGNATURE_VERSION + '\n')
    with patch.object(recomp, 'OUT', str(out)), \
         patch.object(recomp, 'forwarded_target', side_effect=lambda n: 'timer_target' if n == 'timer_thunk' else None), \
         patch.object(recomp, 'parse', return_value=(insns, set())):
        sigs = recomp.signatures(['timer_thunk'])
        assert sigs['timer_thunk']['args'] == ['i32'] and sigs['timer_thunk']['ret'] == 'i32'
        recomp.write('timer_thunk', {'timer_thunk'}, sigs)
    source = r'''#include "recomp.h"
#include <string.h>
#include <stdio.h>
ps2_cpu ps2;
void (*ps2_graphics_sync)(void);
void (*ps2_intc_read)(void);
static uint64_t stack[8];
void ps2_recomp_enter(void) { ps2.gpr[29].d[0] = (uintptr_t)(stack + 8); }
void ps2_store64(uint32_t address, uint64_t value) { memcpy((void *)(uintptr_t)address, &value, 8); }
uint32_t timer_target(uint32_t arg) { return arg == 0x12345678 ? 0x87654321 : 0; }
extern char D_00123458[];
#include "timer_thunk.c"
int main(void) {
    ps2.gpr[4].d[0] = 0x4c;
    if (timer_thunk(0x12345678) != 0x87654321) return 1;
    if (((uint32_t (*)(uint32_t))D_00123458)(0x12345678) != 0x87654321) return 2;
    puts("Recompiled timer thunk arguments and return passed");
    return 0;
}
'''
    path = out / 'test_thunk.c'; path.write_text(source)
    return str(path)
