"""Synthetic cross-unit ABI audit and executable call-rewrite regression."""
import json
from pathlib import Path
import abi_check
from transform import fix_abi_calls


def prepare(directory):
    root = Path(directory).resolve() / 'abi-test'
    raw = root / 'src'
    generated = root / 'audit/src'
    raw.mkdir(parents=True, exist_ok=True)
    generated.mkdir(parents=True, exist_ok=True)
    sources = {
        'callee.c': '''
float inner(int n, float v) { return n + v; }
int target(int n, float a, int *p, float b, int k) {
    return n == 3 && a == 2.5f && *p == 7 && b == 4.5f && k == 9;
}
void arity(int a, int b) {}
void precision(float a) {}
int no_proto(int a, int *b) { return a + *b; }
void promoted(float a) {}
''',
        'color.cpp': '''
union Color { unsigned int packed; };
extern "C" unsigned int func_00161868(Color c,float *v) { return c.packed; }
''',
        'caller.c': '''
typedef float Scalar;
extern float inner(Scalar, int);
extern int alias(Scalar, int, Scalar, int *, int) __asm__("_target");
extern void arity();
extern void precision(double);
#define CALL_PRECISION precision
extern int no_proto();
extern unsigned int func_00161868(unsigned int,float *);
unsigned int reviewed_color(void) { float v=0;return func_00161868(7,&v); }
extern void promoted();
void flagged(void) { arity(1); CALL_PRECISION(1.0); promoted(1.0f); }
int reviewed(void) { int p=3;return no_proto(2,&p); }
int main(void) {
    int p = 7;
    int a = alias(inner(0.5f, 2), 3, 4.5f, &p, 9);
    int b = ((int (*)(float,int,float,int*,int))alias)(2.5f, 3, 4.5f, &p, 9);
    return !(a && b);
}
'''}
    old_root, old_out = abi_check.ROOT, abi_check.OUT
    try:
        abi_check.ROOT, abi_check.OUT = root, root / 'audit'
        for name, source in sources.items():
            (raw / name).write_text(source)
            (generated / name).write_text(source)
        manifest, report = abi_check.audit([abi_check.inspect(generated / name) for name in sources])
        assert len(manifest['sites']) == 3, manifest
        assert len(report['warnings']) == 3, report
        assert report['classification'] == dict(real_remaining=3,reviewed_false_positives=2,gameplay_real_remaining=0),report
        bounds=[{'symbol':name} for name in ['func_0010FFF8','func_00110000','func_001BFFF8','func_001C0000','outside','unsorted/x.c:func_00120000']]
        assert abi_check.classified_counts(bounds,[{}]) == dict(real_remaining=6,reviewed_false_positives=1,gameplay_real_remaining=2)
        assert len(report['reviewed_false_positives']) == 2, report
        reviewed=next(r for r in report['reviewed_false_positives'] if r['symbol']=='no_proto')
        assert reviewed['symbol']=='no_proto' and reviewed['reviewed_reason'], reviewed
        assert any(w['symbol']=='promoted' for w in report['warnings']), report
        assert {s['reason'] for s in report['warnings']} == {
            'argument count / variadic mismatch', 'parameter ABI mismatch'}
        wide=sources['color.cpp'].replace('unsigned int packed','unsigned long long packed')
        (generated/'color.cpp').write_text(wide)
        _,negative=abi_check.audit([abi_check.inspect(generated/name) for name in sources])
        assert any(w['symbol']=='func_00161868' for w in negative['warnings']),negative
        (generated/'color.cpp').write_text(sources['color.cpp'])
        paths = []
        for name, source in sources.items():
            fixed = fix_abi_calls(source, name, manifest['sites'])
            if name == 'caller.c':
                assert 'arity(1); CALL_PRECISION(1.0);' in fixed
                assert fixed.count('\n') == source.count('\n')
                try:
                    fix_abi_calls(source.replace('0.5f', '1.5f'), name, manifest['sites'])
                except ValueError:
                    pass
                else:
                    raise AssertionError('stale source accepted')
            path = root / name
            path.write_text(fixed)
            paths.append(str(path))
        print('ABI audit: typedef, asm alias, cast, nested call, arity, precision and stale-site checks passed')
        return paths
    finally:
        abi_check.ROOT, abi_check.OUT = old_root, old_out


if __name__ == '__main__':
    prepare(abi_check.ROOT / 'build/port/runtime')
