"""Bitwise differential for the native vertex conversion (no GPU or game data)."""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def main():
    out = ROOT / "build/port/native-vertex-test"
    out.mkdir(parents=True, exist_ok=True)
    text = (ROOT / "port/gs/native_core.inc").read_text()
    start = text.index("static void native_set(")
    end = text.index("\n}\n", start) + 3
    source = r'''#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "gs.h"
typedef struct { float x,y,z,s,t,q,fog; uint32_t rgba; } NativeVertex;
typedef struct { uint32_t zmax; float zscale, uv_scale[2], vertex_fog[256]; } Native;
''' + text[start:end] + r'''
static uint32_t rng = 81257;
static uint32_t next(void) { rng ^= rng<<13; rng ^= rng>>17; return rng ^= rng<<5; }
static int check(Native *n, const GsVertex *v, int fst, unsigned tw, unsigned th) {
    NativeVertex a,b;
    native_set(&a,v,v,n,fst,123.5f,-321.25f);
    b.x=123.5f; b.y=-321.25f;
    uint32_t z=v->z>n->zmax?n->zmax:v->z;
    b.z=(float)((double)z*n->zscale);
    b.s=fst?(float)v->u/16.0f/(float)tw:v->s;
    b.t=fst?(float)v->v/16.0f/(float)th:v->t;
    b.q=fst?1:v->q; b.fog=(float)(v->fog&255)/255.0f; b.rgba=v->rgba;
    if (memcmp(&a,&b,sizeof a)) { fprintf(stderr,"vertex differs z=%u uv=%u,%u size=%u,%u fst=%d\n",v->z,v->u,v->v,tw,th,fst); return 1; }
    return 0;
}
int main(void) {
    Native n;
    for(unsigned i=0;i<256;i++) n.vertex_fog[i]=(float)i/255.0f;
    for(unsigned fmt=0;fmt<3;fmt++) {
        n.zmax=fmt==0?UINT32_MAX:fmt==1?0xffffff:0xffff;
        n.zscale=fmt==0?0x1p-32f:fmt==1?0x1p-24f:0x1p-16f;
        for(unsigned size=0;size<=10;size++) {
            unsigned tw=1u<<size,th=1u<<(10-size);
            n.uv_scale[0]=1.0f/(16.0f*tw); n.uv_scale[1]=1.0f/(16.0f*th);
            for(unsigned i=0;i<200000;i++) {
                GsVertex v={0}; v.z=next(); v.u=i&16383; v.v=next()&16383;
                v.fog=i&255; v.rgba=next();
                uint32_t bits=next(); memcpy(&v.s,&bits,4);
                bits=next();memcpy(&v.t,&bits,4); bits=next();memcpy(&v.q,&bits,4);
                if(check(&n,&v,1,tw,th) || check(&n,&v,0,tw,th)) return 1;
            }
        }
        /* Every uint32 float rounding tie and its neighbours, for every exponent. */
        for(unsigned e=24;e<32;e++) {
            unsigned step=1u<<(e-23),begin=1u<<e;
            for(uint64_t z=(uint64_t)begin+step/2;z<(UINT64_C(1)<<(e+1));z+=step) {
                GsVertex v={0}; v.u=1;v.v=16383;v.q=1;
                for(int d=-1;d<=1;d++) {v.z=(uint32_t)(z+d);if(check(&n,&v,1,1024,1))return 1;}
            }
        }
    }
    puts("PASS: native vertex bytes, UV/fog coverage, all uint32 depth rounding ties");
}
'''
    (out / "test.c").write_text(source)
    exe = out / "test.exe"
    subprocess.run(["clang-cl", "/nologo", "/O2", "/clang:-ffp-contract=off", "/I" + str(ROOT / "port/gs"), str(out / "test.c"), "/Fe" + str(exe)], cwd=out, check=True)
    subprocess.run([str(exe)], check=True, timeout=180)


if __name__ == "__main__":
    main()
