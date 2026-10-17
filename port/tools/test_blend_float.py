"""Execute the real rider blend initializer with empty and overflowing tracks."""
from pathlib import Path
import subprocess
import sys
import transform

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/runtime/blend-float'


def main():
    sys.path.insert(0, str(ROOT / 'port'))
    import patches
    OUT.mkdir(parents=True, exist_ok=True)
    text = (ROOT / 'src/unsorted/001E75D8.c').read_text()
    for kind, name, body in patches.PATCHES['unsorted/001E75D8.c']:
        assert kind == 'body' and name == 'Update'
        start = text.index('{', text.index('void Update('))
        end = transform.close_paren(transform.code_mask(text), start) + 1
        text = text[:start] + body + text[end:]
    unit = OUT / 'blend.c'; unit.write_text('#include "port.h"\n' + transform.transform(text))
    obj = unit.with_suffix('.o')
    subprocess.run(['clang', '--target=i686-pc-windows-itanium', '-O1', '-c', '-w',
                    '-msse2', '-mfpmath=sse', '-ffp-contract=off',
                    '-I' + str(ROOT / 'port/include'), str(unit), '-o', str(obj)], check=True)
    driver = OUT / 'driver.c'
    driver.write_text(r'''#include "ps2float.h"
#include <xmmintrin.h>
#include <stdio.h>
#include <string.h>
struct Handle { int x0,x4,x8,xC; };
struct Entry { char p0[4];int x4;char p8[4];int xC;char p10[4];float x14;
 char p18[4];float x1C;char p20[0x28];float x48,x4C,x50; };
struct Entry D_00272888[43],D_00273D60[43];
struct Link { int x0,x4; } D_002752B0[43];
static unsigned char actor[0x6650];
static float control_scale,multiplier;
static unsigned setters,failures;
void func_00203E10(struct Handle *h,float v) { failures+=v!=1 || h->x0!=17;setters++; }
float func_00204280(struct Handle *h) { float v;memcpy(&v,&h->x8,4);return v; }
float func_001E5198(void *p) { failures+=p!=actor+0x6610;return control_scale; }
float func_001E5190(void) { return multiplier; }
float ps2_fdiv(float a,float b) { return ps2_float_div(a,b); }
float ps2_fmul(float a,float b) { return ps2_float_mul(a,b); }
float ps2_fadd(float a,float b) { return ps2_float_add(a,b); }
int ps2_cvt_w_s(float a) { return ps2_float_to_int(a); }
static unsigned bits(float v) { unsigned b;memcpy(&b,&v,4);return b; }
void func_001E75D8(void *);
int main(void) {
 static float cases[][7]={{0,0,0,0,0,0,0},{4,2,2,2,0.5f,2,1},
                         {-4,2,2,2,0.5f,2,1},{4,2,0,0,2,0,1}};
 static float expected[][2]={{0,0},{1,0.25f},{0,0.25f},{2147483648.0f,0}};
 void *owner=actor;
 for(unsigned c=0;c<4;c++) {
  struct Handle *h=(struct Handle *)(actor+0x1e54);h[0].x0=h[1].x0=17;
  memcpy(&h[0].x8,&cases[c][0],4);memcpy(&h[1].x8,&cases[c][1],4);
  control_scale=cases[c][5];multiplier=cases[c][6];setters=0;
  for(unsigned i=0;i<43;i++) {
   struct Entry e={0};e.xC=1;e.x14=cases[c][2];e.x1C=cases[c][3];e.x48=cases[c][4];
   D_00272888[i]=D_00273D60[i]=e;
  }
  _mm_setcsr((_mm_getcsr()|0x8040)&~(0x680|63));
  func_001E75D8(&owner);
  for(unsigned i=0;i<43;i++) {
   failures+=D_00272888[i].x4C!=expected[c][0];
   failures+=c==3 ? bits(D_00272888[i].x50)!=0x7f7fffff
                  : D_00272888[i].x50!=expected[c][1];
  }
  failures+=setters!=172 || !__builtin_isfinite(D_00273D60[0].x4C)
                          || !__builtin_isfinite(D_00273D60[0].x50);
 }
 printf("Real rider blend: empty/ordinary/signed/saturating tracks, %u failures",failures);puts("");
 return failures!=0;
}
''')
    exe = driver.with_suffix('.exe')
    subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-fuse-ld=lld', '-O1',
                    '-I' + str(ROOT / 'port/runtime'), str(driver), str(obj), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


if __name__ == '__main__': main()
