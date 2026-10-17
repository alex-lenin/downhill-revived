"""Exercise the real particle constructor's duration divisions with SSE traps."""
from pathlib import Path
import subprocess
import sys
import transform

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/runtime/particle-float'


def main():
    sys.path.insert(0, str(ROOT / 'port'))
    import patches
    OUT.mkdir(parents=True, exist_ok=True)
    text = (ROOT / 'src/unsorted/00228BE8.c').read_text()
    for kind, old, new in patches.PATCHES['unsorted/00228BE8.c']:
        assert kind == 'text' and text.count(old) == 1
        text = text.replace(old, new)
    start = text.index('typedef struct Vec__w1')
    definition = text.index('int func_0022C468(', start)
    brace = text.index('{', definition)
    end = transform.close_paren(transform.code_mask(text), brace) + 1
    text = 'extern unsigned char D_0029DBA5;\n' + text[start:end]
    unit = OUT / 'constructor.c'
    unit.write_text('#include "port.h"\n' + transform.transform(text, False, '_', None))
    obj = unit.with_suffix('.o')
    subprocess.run(['clang', '--target=i686-pc-windows-itanium', '-O1', '-c', '-w',
                    '-msse2', '-mfpmath=sse', '-ffp-contract=off',
                    '-I' + str(ROOT / 'port/include'), str(unit), '-o', str(obj)], check=True)
    driver = OUT / 'driver.c'
    driver.write_text(r'''#include "ps2float.h"
#include <xmmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned char D_0029DBA5 = 1;
int D_0029E308, D_0029E30C;
int D_0071F980[3];
__declspec(align(16)) unsigned char D_0067E180[0x2860];
__declspec(align(16)) unsigned char D_0027BFC0[64*0x40];
__declspec(align(16)) unsigned char D_0027A4B0[64*0x50];
__declspec(align(16)) unsigned char D_0027D300[64*0x50];
__declspec(align(16)) unsigned char D_00279B00[64*0x20];
float ps2_fdiv(float a,float b) { return ps2_float_div(a,b); }
int func_0023D608(void) { return 0; }
void func_0022A280(void *p) { abort(); }
void ps2_asm(const char *a,const char *b,int n,void *p) { abort(); }
int func_0022C468(int,float);
static unsigned bits(unsigned char *p) { unsigned v;memcpy(&v,p,4);return v; }
int main(void) {
 static int duration[]={0,4,-2,0};
 static float numerators[]={0,2,2,-3};
 static unsigned reciprocal[]={0x7f7fffff,0x3f000000,0xbf800000,0x7f7fffff};
 static unsigned slope[]={0x7f7fffff,0x3f000000,0xbf800000,0xff7fffff};
 unsigned flags=0x01000000;
 memcpy(D_00279B00+42*0x20+0x1c,&flags,4);
 for(unsigned i=0;i<4;i++) {
  short d=(short)duration[i];
  D_0029E308=-1;D_0029E30C=0;D_0071F980[2]=-1;
  memcpy(D_0027D300+42*0x50+0x1a,&d,2);
  memcpy(D_0027D300+42*0x50+0x28,&numerators[i],4);
  _mm_setcsr((_mm_getcsr()|0x8040)&~(0x680|63));
  if(func_0022C468(42,1)!=0 || bits(D_0067E180+0x2764)!=reciprocal[i]
     || bits(D_0067E180+0x2808)!=slope[i]
     || bits(D_0067E180+0x27b4)!=0x7f7fffff
     || bits(D_0067E180+0x27b8)!=0xff7fffff) return 1;
 }
 puts("Real particle constructor: zero, positive and signed durations pass with SSE traps");
 return 0;
}
''')
    exe = driver.with_suffix('.exe')
    subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-fuse-ld=lld', '-O1',
                    '-I' + str(ROOT / 'port/runtime'), str(driver), str(obj), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


if __name__ == '__main__': main()
