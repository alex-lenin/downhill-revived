/* UNIT 0x0011F4C0 0x0011F760 */
typedef struct { float x[4]; } Vec;
typedef struct __attribute__((packed)) { float x[4]; } PackedVec;
typedef struct { char pad[0x30]; short delta; short pad32; Vec *(*get)(void *); } VTable;
typedef struct { char pad[0x792c]; char *x792c; } World;
typedef struct { VTable *vt; char pad4[12]; char *x10; char pad14[0x24]; World *x38; char pad3c[0xa0]; PackedVec xdc, xec; char padfc[0x16e]; short x26a; } Obj;
typedef struct { char pad[0xc80]; unsigned char indices[32]; unsigned char count; } Pool;
extern Pool D_004A9160;
extern float func_002223F0(void *, void *);
extern int func_00222BD8(int,int);
extern void func_00221888(void *,void *,float *);
extern float func_00203C28(PackedVec *);
extern void func_001AC7B0(void *,void *);
extern void func_001ACB70(void *,float);
extern void func_001AC920(void *,float,float);
extern void func_0011E568(Obj *,int);
static inline char *alloc(Pool *pool) {
 unsigned int old=pool->count; unsigned int n=old & 255;
 if(n<32) { pool->count=old+1; return (char *)pool+pool->indices[n]*100; }
 return 0;
}
void func_0011F4C0(Obj *o) {
 Vec diff; struct { float side; float pad[3]; PackedVec copy; } local; char *p; Vec *v; float f; unsigned int n;
 if (func_002223F0(o->x38->x792c+0x10,o->x10+0x10)<30.0f && func_00222BD8(0,60)==0) {
 p=o->x38->x792c;
 p+=0x10;
 v=o->vt->get((char *)o+o->vt->delta);
 __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(&diff),"r"(p),"r"(v) : "memory");
 func_00221888(o->x10+0x40,&diff,&local.side);
 func_00222BD8(0,2);
 if(local.side>0.0f) { float f;
 local.copy=o->xec; f=func_00203C28(&local.copy);
 if(f>=0.99f || f<=0.0f) {
 p=alloc(&D_004A9160);
 if(p) {func_001AC7B0(p,&o->xec);p[9]=4;func_001ACB70(p,1.5f);func_001AC920(p,100.0f,100.0f);o->x26a=func_00222BD8(60,180);}
 }
 } else { float f;
 local.copy=o->xdc; f=func_00203C28(&local.copy);
 if(f>=0.99f || f<=0.0f) {
 p=alloc(&D_004A9160);
 if(p) {func_001AC7B0(p,&o->xdc);p[9]=4;func_001ACB70(p,1.5f);func_001AC920(p,100.0f,100.0f);}
 o->x26a=func_00222BD8(60,180);
 }
 }
 }
 func_0011E568(o,5);
}
