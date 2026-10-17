"""Exercise full EE quadwords through native/recompiled calls on real i386 ABI.

Synthetic functions only. Checks a 48-byte named-play-shaped packet, following
scalars, signed/unsigned aliases, stale signatures, quadword returns, and
16-byte-aligned arguments spilled past the eight EE integer argument GPRs.
"""
import json
from pathlib import Path
import subprocess
from unittest.mock import patch

import recomp

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/audio-round5-quad-abi'


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    source = OUT / 'native.c'
    source.write_text(r'''
#include "quad_abi.h"
typedef unsigned int Name __attribute__((mode(TI)));
typedef Name NamedAlias;
typedef int SignedName __attribute__((mode(TI)));
struct Packet { uint64_t bank; int volume,pan; NamedAlias name; short pm,pb; int x24; unsigned flags; };
struct Packet received;
typedef char packet_size_check[sizeof(struct Packet)==48?1:-1];
NamedAlias func_00123430(int id,uint64_t key,NamedAlias name,int volume,int pan,int pm,int pb) {
 received.bank=key;received.name=name;received.volume=volume;received.pan=pan;
 received.pm=pm;received.pb=pb;received.x24=id;received.flags=0x3d;
 ps2_q128 bits=ps2_scalar_quad(name);bits.w[3]^=0xf0e1d2c3;
 return ps2_quad_scalar(bits);
}
SignedName func_00123434(SignedName name) { return name; }
unsigned stack_ok;
void func_00123438(unsigned a,unsigned b,unsigned c,unsigned d,unsigned e,unsigned f,unsigned g,unsigned h,
 unsigned ninth,NamedAlias first,unsigned between,NamedAlias second) {
 ps2_q128 x=ps2_scalar_quad(first),y=ps2_scalar_quad(second);
 stack_ok=a==1&&b==2&&c==3&&d==4&&e==5&&f==6&&g==7&&h==8&&ninth==9&&between==10&&
 x.w[0]==0x12345678&&x.w[1]==0x90abcdef&&x.w[2]==0xfeedabcd&&x.w[3]==0x80001234&&
 y.w[0]==0xaabbccdd&&y.w[1]==0x12345678&&y.w[2]==0x98765432&&y.w[3]==0xffffffff;
}
'''.replace('#include "quad_abi.h"', '#include "' + (ROOT/'port/runtime/quad_abi.h').as_posix() + '"'),
                      encoding='utf-8')
    stale = {'func_00123430': dict(ret='i32', args=['i32','i64']+['i32']*5,
                                 variadic=False, **{'from':'native.c'})}
    (OUT / 'signatures.json').write_text(json.dumps(stale))
    (OUT / 'signatures-version.txt').write_text('3\n')
    names = ['func_00123430','func_00123434','func_00123438']
    with patch.object(recomp, 'OUT', str(OUT)), patch.object(recomp, 'SRC', str(OUT)):
        sigs = recomp.signatures(names)
    assert sigs[names[0]]['args'] == ['i32','i64','i128']+['i32']*4
    assert sigs[names[0]]['ret'] == 'i128'
    assert sigs[names[1]]['args'] == ['i128'] and sigs[names[1]]['ret'] == 'i128'
    assert recomp.arg_places(sigs[names[2]])[-4:] == [
        ('stack',0),('stack',16),('stack',32),('stack',48)]
    for name in names:
        wrapper = name.replace('func_', 'quad_probe_')
        sigs[wrapper] = sigs[name]
        insns = [(0x1000,'jal',[name]),(0x1004,'nop',[])]
        with patch.object(recomp, 'OUT', str(OUT)), patch.object(recomp, 'parse', return_value=(insns,set())):
            recomp.write(wrapper, {wrapper}, sigs)
    # Fixed stack quadwords must also survive the existing word-varargs bridge.
    # A slot-count offset would start varargs at +16 and overwrite the name.
    variadic = 'quad_probe_varargs'
    sigs[variadic] = dict(ret='i128', args=['i32'] * 9 + ['i128'],
                          variadic=True, **{'from': 'synthetic EE body'})
    insns = [(0x1000,'lq',['$v0','16($sp)']), (0x1004,'lw',['$v1','32($sp)']),
             (0x1008,'jr',['$ra']), (0x100c,'nop',[])]
    with patch.object(recomp, 'OUT', str(OUT)), patch.object(recomp, 'parse', return_value=(insns,set())):
        recomp.write(variadic, {variadic}, sigs)
    driver = OUT / 'driver.c'
    driver.write_text(r'''
#include "recomp.h"
#include "quad_abi.h"
#include <stdio.h>
#include <string.h>
ps2_cpu ps2;
void (*ps2_graphics_sync)(void);
void (*ps2_intc_read)(void);
static unsigned char stack[512] __attribute__((aligned(16)));
void ps2_recomp_enter(void) { GPR(29)=(uintptr_t)(stack+sizeof stack); }
struct Packet { uint64_t bank; int volume,pan; ps2_u128 name; short pm,pb; int x24; unsigned flags; };
extern struct Packet received;
extern unsigned stack_ok;
#include "quad_probe_00123430.c"
#include "quad_probe_00123434.c"
#include "quad_probe_00123438.c"
#include "quad_probe_varargs.c"
int main(void) {
 for(unsigned trial=0;trial<64;trial++) {
  ps2_q128 name={{0}},got,expected;
  name.w[0]=0x53454d41+trial;name.w[1]=0x87654321^trial;
  name.w[2]=0xfedcba98-trial;name.w[3]=0x80000000+trial;
  uint64_t key=0x8877665544332211ULL+trial;
  memset(&ps2.gpr[6],0xaa,16);
  got=ps2_scalar_quad(quad_probe_00123430(7,key,ps2_quad_scalar(name),803,225,-319,-54));
  expected=name;expected.w[3]^=0xf0e1d2c3;
  if(memcmp(&got,&expected,16)||memcmp(&received.name,&name,16)||
     memcmp(&ps2.gpr[6],&name,16)||received.bank!=key||received.volume!=803||
     received.pan!=225||received.pm!=-319||received.pb!=-54||received.x24!=7||received.flags!=0x3d) return 1;
  got=ps2_scalar_quad(quad_probe_00123434(ps2_quad_scalar(name)));
  if(memcmp(&got,&name,16)) return 2;
 }
 ps2_q128 a={{0}},b={{0}};
 a.w[0]=0x12345678;a.w[1]=0x90abcdef;a.w[2]=0xfeedabcd;a.w[3]=0x80001234;
 b.w[0]=0xaabbccdd;b.w[1]=0x12345678;b.w[2]=0x98765432;b.w[3]=0xffffffff;
 quad_probe_00123438(1,2,3,4,5,6,7,8,9,ps2_quad_scalar(a),10,ps2_quad_scalar(b));
 if(!stack_ok||GPR(29)!=(uintptr_t)(stack+sizeof stack)) return 3;
 ps2_q128 result=ps2_scalar_quad(quad_probe_varargs(1,2,3,4,5,6,7,8,9,ps2_quad_scalar(a),
                                                55u,66u,77u,88u));
 if(memcmp(&result,&a,16)||UW(3)!=55||GPR(29)!=(uintptr_t)(stack+sizeof stack)) return 4;
 puts("Quadword ABI: complete names, following scalars, returns, stack alignment and stale cache pass");
 return 0;
}
''', encoding='utf-8')
    executable = OUT / 'test.exe'
    subprocess.run(['clang','--target=i686-pc-windows-msvc','-fuse-ld=lld','-O2',
                    '-I'+str(ROOT/'port/runtime'),str(source),str(driver),'-o',str(executable)], check=True)
    subprocess.run([str(executable)], check=True)


if __name__ == '__main__':
    main()
