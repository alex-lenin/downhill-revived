"""Reject FP, DI and TI call-boundary regressions, including callbacks."""
from collections import defaultdict
from pathlib import Path
from unittest.mock import patch
import json
import abi_check
import scalar_abi
import recomp


def main():
    root=abi_check.ROOT/'build/port/runtime/scalar-guard'
    out=root/'audit';(out/'src').mkdir(parents=True,exist_ok=True);(root/'src').mkdir(exist_ok=True)
    callee='''
typedef unsigned int Q __attribute__((mode(TI)));
typedef unsigned int W __attribute__((mode(DI)));
void fp(int a,float b,int c) {}
void integer(int a,int b,int c) {}
void precision(double a) {}
void promoted(float a) {}
void wide(int a,W b,int c) {}
W wide_result(void) { return 0x8877665544332211ULL; }
void quad(int a,Q b,int c) {}
Q quad_result(void) { return (Q)0x8877665544332211ULL << 64; }
void (*table[1])(int,float,int)={fp};
'''
    caller='''
typedef unsigned int Q __attribute__((mode(TI)));
typedef unsigned int W __attribute__((mode(DI)));
extern void fp(int,int,int);
extern void integer(int,float,int);
extern void precision(float);
extern void wide(int,int,int);
extern int wide_result(void);
extern void quad(int,W,int);
extern W quad_result(void);
extern void (*table[1])(int,int,int);
void run(void) {
 fp(1,2.5f,3); integer(1,2.5f,3); precision(2.5f); promoted(2.5f);
 wide(1,2,3); wide_result(); quad(1,2,3); quad_result(); table[0](1,2,3);
 { void (*local)(int,float,int)=0; ((void (*)(int,int,int))local)(1,2,3); }
}
'''
    old_root,old_out=abi_check.ROOT,abi_check.OUT
    try:
        abi_check.ROOT,abi_check.OUT=root,out
        def check(c):
            units=[]
            for name,text in [('callee.c',callee),('caller.c',c)]:
                (root/'src'/name).write_text(text);p=out/'src'/name;p.write_text(text);units.append(abi_check.inspect(p))
            definitions=defaultdict(list)
            for u in units:
                for d in u[0]:definitions[d['symbol']].append(d)
            return scalar_abi.audit(units,definitions,abi_check.abi)
        report=check(caller)
        assert {'fp','integer','precision','promoted','@table:table','@indirect-cast'} <= {r['symbol'] for r in report['float_argument_mismatches']},report
        assert {'wide','wide_result','quad','quad_result'} <= {r['symbol'] for r in report['integer64_mismatches']},report
        assert {'quad','quad_result'} <= {r['symbol'] for r in report['integer128_mismatches']},report
        try:scalar_abi.require_clean(report)
        except SystemExit as e:assert 'Scalar ABI guard' in str(e)
        else:raise AssertionError('unsafe scalar calls passed')
        good=caller.replace('fp(int,int,int)','fp(int,float,int)').replace('integer(int,float,int)','integer(int,int,int)').replace('precision(float)','precision(double)').replace('wide(int,int,int)','wide(int,W,int)').replace('extern int wide_result','extern W wide_result').replace('quad(int,W,int)','quad(int,Q,int)').replace('extern W quad_result','extern Q quad_result').replace('(*table[1])(int,int,int)','(*table[1])(int,float,int)').replace('((void (*)(int,int,int))local)','((void (*)(int,float,int))local)').replace('void run(void)','extern void promoted(float);\nvoid run(void)')
        scalar_abi.require_clean(check(good))
        # Address escapes are checked separately, even when no direct call exists.
        callback=good+'\nextern void sink(void (*)(void));\nvoid escape(void) {sink(fp);}\n'
        assert any(r['symbol']=='fp' and r['kind']=='callback/cast' for r in check(callback)['float_argument_mismatches'])
        # Signedness preserves the same width and slots; aggregate quadword
        # records stay distinct from scalar TI values.
        assert abi_check.abi('unsigned long long')==abi_check.abi('long long')=='int64'
        assert abi_check.abi('unsigned __int128')=='int128'
        assert abi_check.abi('struct Quad')!='int128'
        # Even a current-version cache cannot override authoritative bridges.
        cache=root/'cache';cache.mkdir(exist_ok=True)
        names=['func_00201B00','func_00201BD0','func_00201E08','func_001FBDD8','func_0022B1F0','func_0022B548','func_001FB6E0']
        (cache/'signatures.json').write_text(json.dumps({n:dict(ret='i32',args=['i32']*8,variadic=False,**{'from':'stale'}) for n in names}))
        (cache/'signatures-version.txt').write_text(recomp.SIGNATURE_VERSION+'\n')
        with patch.object(recomp,'OUT',str(cache)):sigs=recomp.signatures(names)
        for n in names:assert sigs[n]==recomp.ABI_SIGNATURES[n],(n,sigs[n])
        assert sigs['func_00201B00']['args'][2]=='i128'
        assert sigs['func_001FBDD8']['ret']=='i64'
        assert recomp.ABI_SIGNATURES['func_0022C350']['ret']=='unsigned char'
        assert recomp.ABI_SIGNATURES['func_0023AFA8']['ret']=='char'
        assert recomp.marshal(sigs['func_00201B00'])[0][2]=='ps2_quad_scalar(ps2.gpr[6])'
        assert 'FPR(12)' in recomp.marshal(sigs['func_0022B1F0'])[0]
        definitions={'func_00201B00':[dict(signature=dict(ret='void',args=['int']*7,variadic=False))]}
        try:scalar_abi.verify_native_contracts(definitions,abi_check.abi)
        except SystemExit as e:assert 'Stale native scalar' in str(e)
        else:raise AssertionError('stale scalar metadata passed')
        unknown=[[],[],0,[],[dict(file='caller.c',line=1,symbol='func_00259120',actual_args=['int','int','double'])]]
        try:scalar_abi.verify_variadic_bridges([unknown],{},abi_check.abi)
        except SystemExit as e:assert 'Variadic scalar ABI guard' in str(e)
        else:raise AssertionError('unknown wide vararg marshalling passed')
        known=[[],[],0,[],[dict(file='caller.c',line=1,symbol='func_00254D08',actual_args=['int','double','unsigned __int128'])]]
        scalar_abi.verify_variadic_bridges([known],{},abi_check.abi)
        # New assembly callees cannot silently inherit the eight-int guess.
        assembly_root=root/'assembly-census'
        asm=assembly_root/'asm/nonmatchings/text';asm.mkdir(parents=True,exist_ok=True)
        catalog=assembly_root/'port/scalar_contracts.json';catalog.parent.mkdir(exist_ok=True)
        catalog.write_text(json.dumps({'assembly':{}}))
        (asm/'func_00123450.s').write_text('/* 0 00123450 00000000 */ mul.s $f0, $f12, $f12\n')
        (asm/'func_00123454.s').write_text('/* 0 00123454 00000000 */ sq $a2, 0($sp)\n')
        with patch.object(scalar_abi,'CATALOG',catalog):
            for name in ('func_00123450','func_00123454'):
                try:scalar_abi.verify_assembly_inputs({name})
                except SystemExit as e:assert 'Assembly scalar input guard' in str(e)
                else:raise AssertionError('unreviewed assembly input passed')
            catalog.write_text(json.dumps({'assembly':{
                'func_00123450':dict(ret='float',args=['float']),
                'func_00123454':dict(ret='void',args=['int','int','int128'])}}))
            scalar_abi.verify_assembly_inputs({'func_00123450','func_00123454'})
        print('Scalar guard: FP/default promotions, DI/TI args/results, callbacks/tables, metadata and current cache pass')
    finally:abi_check.ROOT,abi_check.OUT=old_root,old_out


if __name__=='__main__':main()
