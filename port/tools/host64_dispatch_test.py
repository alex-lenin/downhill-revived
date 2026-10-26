"""Check prepared dispatch against mutable tables, foreign contexts and traps.
All WAT/C/build products are synthetic and remain under build/.
"""
import os
import subprocess
from build_host64 import ROOT, WABT, LLVM, MSVC, SDK, VERSION as VER, SOURCE as SRC
from prepare_wasm2c import prepare

RT = WABT / 'share/wabt/wasm2c'


def main():
    out = ROOT / 'build/host64-dispatch-test'
    out.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, TEMP=str(out), TMP=str(out))
    def run(args):
        r = subprocess.run(list(map(str, args)), cwd=out, env=env,
                           capture_output=True, text=True, timeout=120)
        if r.returncode:
            raise RuntimeError(r.stdout + r.stderr)
        return r.stdout
    (out/'probe.wat').write_text(r"""(module
      (type $unary (func (param i32) (result i32)))
      (memory 32768 32768)
      (global $bias (mut i32) (i32.const 3))
      (table (export "callbacks") 4 funcref)
      (func $add (type $unary) (param i32) (result i32)
        local.get 0 global.get $bias i32.add)
      (func $mul (type $unary) (param i32) (result i32)
        local.get 0 global.get $bias i32.mul)
      (func $wrong (result f32) f32.const 1)
      (elem (i32.const 0) $add $mul $wrong)
      (func (export "call") (param i32 i32) (result i32)
        local.get 1 local.get 0 call_indirect (type $unary))
    )""")
    run([WABT/'bin/wat2wasm.exe', '--debug-names', out/'probe.wat', '-o', out/'probe.wasm'])
    run([WABT/'bin/wasm2c.exe', out/'probe.wasm', '-n', 'probe', '-o', out/'probe.c'])
    unprepared=(out/'probe.c').read_text()
    prepare(out/'probe.c')
    prepared = (out/'probe.c').read_text()
    assert 'CHECK_CALL_INDIRECT(instance' in prepared and '? wrap_' in prepared
    prepare(out/'probe.c')
    assert prepared == (out/'probe.c').read_text()
    (out/'driver.c').write_text(r"""
#include <windows.h>
#include <stdio.h>
#include "probe.c"
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL %s:%d: %s\n",__FILE__,__LINE__,#x); return 1; } } while(0)
#include "wasm-rt-impl.h"
static u32 foreign(void *context, u32 x) { return x + *(u32*)context; }
static int expect_trap(w2c_probe *p, u32 index, wasm_rt_trap_t expected) {
 wasm_rt_trap_t t=WASM_RT_SETJMP(g_wasm_rt_jmp_buf);
 if (!t) { w2c_probe_call(p,index,9); return 0; }
 return t==expected && wasm_rt_call_stack_depth==0;
}
int main(void) {
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
 wasm_rt_init(); w2c_probe p; wasm2c_probe_instantiate(&p);
 wasm_rt_funcref_table_t *t=w2c_probe_callbacks(&p);
 CHECK(w2c_probe_call(&p,0,9)==12 && w2c_probe_call(&p,1,9)==27);
 wasm_rt_funcref_t saved=t->data[0];
 t->data[0]=t->data[1]; CHECK(w2c_probe_call(&p,0,9)==27);
 /* Same native entry with a different module context. No second allocation. */
 w2c_probe other=p; other.w2c_bias=100;
 t->data[0]=saved; t->data[0].module_instance=&other;
 CHECK(w2c_probe_call(&p,0,9)==109);
 /* Unknown same-signature function must retain the checked indirect fallback. */
 u32 bias=41; t->data[0]=saved; t->data[0].func=(wasm_rt_function_ptr_t)foreign;
 t->data[0].module_instance=&bias; CHECK(w2c_probe_call(&p,0,9)==50);
 /* Identical SHA bytes at a different address exercise WABT's type slowpath. */
 char hash[32]; memcpy(hash,saved.func_type,32); t->data[0].func_type=hash;
 CHECK(w2c_probe_call(&p,0,9)==50);
 CHECK(expect_trap(&p,2,WASM_RT_TRAP_CALL_INDIRECT));
 CHECK(expect_trap(&p,3,WASM_RT_TRAP_CALL_INDIRECT));
 CHECK(expect_trap(&p,4,WASM_RT_TRAP_CALL_INDIRECT));
 CHECK(expect_trap(&p,0xffffffffu,WASM_RT_TRAP_CALL_INDIRECT));
 t->data[0]=saved;
 wasm_rt_call_stack_depth=WASM_RT_MAX_CALL_STACK_DEPTH-1;
 CHECK(expect_trap(&p,0,WASM_RT_TRAP_EXHAUSTION));
 wasm2c_probe_free(&p); wasm_rt_free();
 puts("PASS dispatch: promotion, mutation, foreign instance, unknown target, equal hash, wrong type, null, OOB, depth, idempotence");
 return 0;
}
""")
    inc=['/imsvc'+str(MSVC/'include')]+['/imsvc'+str(SDK/'Include'/VER/x) for x in ('ucrt','um','shared')]
    flags=['/O2','/clang:-O3','/clang:-march=x86-64-v2','/clang:-mtune=znver3',
           '/clang:-ffp-model=strict','/clang:-ffp-contract=off',
           '-DWASM_RT_STACK_DEPTH_COUNT=1',*inc,
           '-I'+str(WABT/'include'),'-I'+str(RT),'-I'+str(SRC),'-I'+str(out)]
    objects=[]
    for source in [out/'driver.c',RT/'wasm-rt-impl.c',SRC/'guest_memory.c']:
        obj=out/(source.stem+'.obj'); objects.append(obj)
        run([LLVM/'clang-cl.exe',*flags,'/c',source,'/Fo'+str(obj)])
    run([LLVM/'clang-cl.exe',*objects,'/Fe'+str(out/'test.exe'),'/link',
         '/libpath:'+str(MSVC/'lib/x64'),'/libpath:'+str(SDK/'Lib'/VER/'ucrt/x64'),
         '/libpath:'+str(SDK/'Lib'/VER/'um/x64'),'onecore.lib'])
    print(run([out/'test.exe']))
    # Explicit hot selection in a wider group: the other known target is still
    # callable through the checked fallback, as are later foreign table entries.
    (out/'probe.c').write_text(unprepared)
    prepare(out/'probe.c',hot_functions=['wrap_w2c_probe_add'])
    assert ' ? wrap_w2c_probe_add(' in (out/'probe.c').read_text()
    assert ' ? wrap_w2c_probe_mul(' not in (out/'probe.c').read_text()
    run([LLVM/'clang-cl.exe',*flags,'/c',out/'driver.c','/Fo'+str(objects[0])])
    run([LLVM/'clang-cl.exe',*objects,'/Fe'+str(out/'test.exe'),'/link',
         '/libpath:'+str(MSVC/'lib/x64'),'/libpath:'+str(SDK/'Lib'/VER/'ucrt/x64'),
         '/libpath:'+str(SDK/'Lib'/VER/'um/x64'),'onecore.lib'])
    print('Explicit hot target: '+run([out/'test.exe']))

if __name__=='__main__': main()
