"""Check patched 32-bit Huffman tables across stack poison and compiler choices."""
import re, subprocess, sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'port'))
import patches
out=ROOT/'build/port/huft-test'; out.mkdir(parents=True,exist_ok=True)
s=(ROOT/'src/unsorted/00240270.c').read_text()
for kind,a,b in patches.PATCHES['unsorted/00240270.c']:
 if kind=='text':
  assert s.count(a)==1; s=s.replace(a,b)
 else:
  s,n=re.subn(a[0],b,s); assert n==a[1]
struct=s[s.index('typedef struct Huft {'):s.index('extern const unsigned short D_00299440')]
fn=s[s.index('int func_00240CB8('):s.index('\n\nextern unsigned short D_00299446')]
harness=r'''
#include <windows.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
STRUCT
typedef char layout_check[(sizeof(Huft)==8 && offsetof(Huft,v)==4)?1:-1];
static unsigned char *arena; static unsigned used;
void *func_00254EB0(unsigned n) { void *p=arena+used; used+=n; if(used>65536) abort(); return p; }
int func_00240C70(char *p) { return 0; }
FUNCTION
__declspec(noinline) void poison(unsigned seed) { volatile unsigned a[4096]; for(unsigned i=0;i<4096;i++) a[i]=seed*0x01010101u; }
static int check(Huft *t,unsigned bits,unsigned *seen) {
 for(unsigned i=0;i<(1u<<bits);i++) {
  Huft *p=t+i; if(p->padding) return 1;
  if(p->e>16 && p->e<32) { if(check(p->v.t,p->e-16,seen)) return 2; }
  else { if(p->v.bits>65535) return 3; if(p->e!=99) seen[p->v.n]++; else if(p->v.n) return 4; }
 } return 0;
}
int main(void) {
 arena=VirtualAlloc((void*)0x08800000,65536,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE); if(arena!=(void*)0x08800000) return 10;
 unsigned char expected[65536]; unsigned expected_size=0; uint64_t hash=1469598103934665603ULL;
 for(unsigned test=0;test<2;test++) for(unsigned seed=0;seed<256;seed++) {
  unsigned lens[288],seen[65536]={0},bits=test?2:7,n=test?2:288; Huft *t=0;
  for(unsigned i=0;i<288;i++) lens[i]=test?2:i<144?8:i<256?9:i<280?7:8;
  used=0; memset(arena,0,65536); poison(seed);
  int rc=func_00240CB8(lens,n,288,0,0,&t,&bits); if(rc!=(test?1:0)) return 11;
  if(check(t,bits,seen)) return 12;
  for(unsigned i=0;i<n;i++) if(!seen[i]) return 13;
  if(!seed) {memcpy(expected,arena,used);expected_size=used;} else if(used!=expected_size||memcmp(expected,arena,used)) return 14;
  if(!seed) for(unsigned i=0;i<used;i++) hash=(hash^arena[i])*1099511628211ULL;
 }
 printf("%016llX\n",(unsigned long long)hash); return 0;
}
'''.replace('STRUCT',struct).replace('FUNCTION',fn)
source=out/'test.c';source.write_text(harness)
answers=[]
for opt,isa in [('1','sse2'),('2','sse2'),('2','sse4.1'),('2','avx2')]:
 exe=out/f'huft-{opt}-{isa}.exe'
 subprocess.run(['clang-cl','-m32','/nologo','/clang:-O'+opt,'-m'+isa,'-mno-fma','/clang:-ffp-contract=off',str(source),'/Fe'+str(exe),'/Fo'+str(out/'test.obj'),'/link','/dynamicbase:no'],check=True,timeout=60)
 result=subprocess.check_output([str(exe)],timeout=20,text=True).strip();answers.append(result)
 print(opt,isa,result,flush=True)
assert len(set(answers))==1,answers
print('Huffman table bytes MATCH: 4 builds, 256 stack patterns, complete and incomplete trees')
