"""Negative and positive coverage for the strict floating-return ABI guard."""
from pathlib import Path
import abi_check
import float_abi


def main():
    root = abi_check.ROOT / 'build/port/runtime/float-abi-test'
    out = root / 'audit'
    (out / 'src').mkdir(parents=True, exist_ok=True)
    (root / 'src').mkdir(exist_ok=True)
    sources = {
        'callee.c': '''
float direct(void) { return 1.25f; }
double wide(void) { return 2.5; }
float cast_target(void) { return 3.75f; }
float implicit_target(void) { return 4.5f; }
float header_target(void) { return 5.5f; }
float (*callbacks[1])(void) = { direct };
''',
        'caller.c': '''
#include "wrong.h"
extern void direct(void);
extern int wide(void);
extern float cast_target(void);
extern void func_00260E08(void *, void *, void *, void *);
extern void (*callbacks[1])(void);
void run(void) {
 direct(); wide(); implicit_target(); header_target();
 func_00260E08(0,0,0,0);
 ((void (*)(void))cast_target)();
 { float (*local)(void) = cast_target; ((void (*)(void))local)(); }
 callbacks[0]();
}
'''}
    (out / 'src/wrong.h').write_text('extern int header_target(void);\n')
    old_root, old_out = abi_check.ROOT, abi_check.OUT
    try:
        abi_check.ROOT, abi_check.OUT = root, out
        def check(data):
            for name, text in data.items():
                (out / 'src' / name).write_text(text)
                (root / 'src' / name).write_text(text)
            return abi_check.audit([abi_check.inspect(out / 'src' / name) for name in data])[1]
        report = check(sources)
        errors = report['float_return_mismatches']
        symbols = {d['symbol'] for d in errors}
        assert {'direct','wide','cast_target','implicit_target','header_target',
                'func_00260E08','@table:callbacks','@pointercast'} <= symbols, errors
        assert any(d['kind'] == 'function-pointer conversion' and d['symbol']=='cast_target' for d in errors)
        try:
            float_abi.require_clean(report)
        except SystemExit as e:
            assert 'Floating-return ABI guard' in str(e)
        else:
            raise AssertionError('unsafe float returns did not fail')
        good = dict(sources)
        good['caller.c'] = good['caller.c'].replace('extern void direct','extern float direct') \
            .replace('extern int wide','extern double wide') \
            .replace('extern void func_00260E08','extern float func_00260E08') \
            .replace('extern void (*callbacks','extern float (*callbacks') \
            .replace('((void (*)(void))cast_target)', '((float (*)(void))cast_target)') \
            .replace('((void (*)(void))local)', '((float (*)(void))local)') \
            .replace('void run(void)', 'extern float implicit_target(void);\nvoid run(void)')
        (out / 'src/wrong.h').write_text('extern float header_target(void);\n')
        positive = check(good)
        assert not positive['float_return_mismatches'], positive
        float_abi.require_clean(positive)
        # Stale inferred integer/void wrappers must also regain FP returns and
        # the software-double argument widths before emitting native bridges.
        import json
        from unittest.mock import patch
        import recomp
        cache_dir = root / 'cache'
        cache_dir.mkdir(exist_ok=True)
        names = ['func_00260E08','func_001FD300','func_001FC810',
                 'func_001FCDA0','func_001FCDF0','func_001FD010','func_00248A80']
        stale = {n: dict(ret='void', args=['i32'], variadic=False, **{'from':'stale'}) for n in names}
        (cache_dir / 'signatures.json').write_text(json.dumps(stale))
        (cache_dir / 'signatures-version.txt').write_text('3\n')
        with patch.object(recomp, 'OUT', str(cache_dir)):
            sigs = recomp.signatures(names)
        for name in names:
            assert sigs[name] == recomp.ABI_SIGNATURES[name], sigs[name]
        assert recomp.marshal(sigs['func_00260E08'])[1] == 'FPR(0) = %s;'
        assert recomp.marshal(sigs['func_001FD300'])[1] == 'GPR(2) = ps2_d2u(%s);'
        assert recomp.marshal(sigs['func_001FC810'])[0] == ['ps2_u2d(GPR(4))','ps2_u2d(GPR(5))']
        print('Float-return guard: direct, double, implicit, header, cast, asm and table negative/positive checks pass')
    finally:
        abi_check.ROOT, abi_check.OUT = old_root, old_out


if __name__ == '__main__':
    main()
