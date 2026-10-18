"""Execute the real completion/ready-state ratio with zero totals and SSE traps."""
from pathlib import Path
import subprocess
import sys

from pctarget import HOST_TARGET
ROOT = Path(__file__).resolve().parents[2]


def main():
    sys.path.insert(0, str(ROOT / 'port'))
    import patches
    code = (ROOT / 'src/unsorted/001F8920.c').read_text()
    for kind, old, new in patches.PATCHES['unsorted/001F8920.c']:
        assert kind == 'text' and code.count(old) == 1
        code = code.replace(old, new)
    out = ROOT / 'build/port/runtime/ready-ratio'; out.mkdir(parents=True, exist_ok=True)
    source = out / 'fixture.c'
    source.write_text('#include "ps2float.h"\n#include <assert.h>\n#include <xmmintrin.h>\n#include <stdio.h>\n'
                     'static float ps2_fdiv(float a,float b){return ps2_float_div(a,b);}\n' + code + r'''
struct Game D_003F52D0;
static int complete, empty;
void func_001F84A8(struct Race *r){(void)r;}
void func_0018E468(struct Race *r){(void)r;empty++;}
void func_00190358(struct Game *g){assert(g->x10C==15);complete++;}
void func_0018E098(struct Race *r){(void)r;}
void func_001F8AC8(struct Race *r){(void)r;}
void func_0018DCE8(struct Race *r){(void)r;}
void func_0018E1C0(struct Race *r){(void)r;}
int port_online_guest_track(void){return 0;}
void func_001F8A08(void *r){(void)r;}
int main(void){
 struct Race r={0}; _mm_setcsr((_mm_getcsr()&~0x680u)&~63u);
 r.x508=1;func_001F8920(&r);assert(complete==1 && r.x4A8==5);
 r.x508=-1;func_001F8920(&r);assert(empty==0);
 r.x504=10;func_001F8920(&r);assert(empty==1);
 r.x508=1;r.x500=5;func_001F8920(&r);assert(complete==1);
 r.x500=10;func_001F8920(&r);assert(complete==2);
 r.x500=-10;r.x504=0;func_001F8920(&r);assert(complete==2);
 puts("Ready/completion ratio: zero, ordinary and signed totals pass with SSE traps");return 0;
}
''')
    exe = out / 'fixture.exe'
    subprocess.run(['clang', '--target=' + HOST_TARGET, '-O1', '-w', '-msse2', '-mfpmath=sse',
                    '-I', str(ROOT / 'port/runtime'), str(source), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


if __name__ == '__main__': main()
