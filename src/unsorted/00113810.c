/* UNIT 0x00113810 0x00113958 .rodata=0x2888F0:0x288950 */
typedef struct Rider { char pad0[0x796c]; int x796c; } Rider;
typedef struct Obj { Rider *x0; char pad4[0x2d0]; char x2d4[0x16c]; unsigned long x440; } Obj;
extern void func_001144F0(void *,Rider *,Obj *);
extern unsigned int D_004B2840[];
void func_00113810(Obj *p) {
 int expected,kind;
 func_001144F0(p->x2d4,p->x0,p);
 p->x440 &= ~(1UL<<56);
 switch(D_004B2840[0]) {
 case 6: if(p->x0->x796c==0xe0) p->x440|=1UL<<56; break;
 case 7: if(p->x0->x796c==0xe0) p->x440|=1UL<<56; break;
 case 2: if(p->x0->x796c==0xa0) p->x440|=1UL<<56; break;
 case 8: if(p->x0->x796c==0xe0 || p->x0->x796c==0xa0) p->x440|=1UL<<56; break;
 case 10: if(p->x0->x796c==0x140 || p->x0->x796c==0xa0) p->x440|=1UL<<56; break;
 case 9: if(p->x0->x796c==0xe0) p->x440|=1UL<<56; break;
 case 12: if(p->x0->x796c==0x40) p->x440|=1UL<<56; break;
 case 15: if(p->x0->x796c==0xe0) p->x440|=1UL<<56; break;
 case 17: if(p->x0->x796c==0xe0) p->x440|=1UL<<56; break;
 case 4: case 13: case 16: case 18: case 22: if(p->x0->x796c==0x140) p->x440|=1UL<<56; break;
 case 0: default: break;
 }
}
