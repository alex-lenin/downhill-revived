/* UNIT 0x0010C630 0x0010C940 */
typedef struct Vec { float x,y,z,w; } Vec;
static inline float Dot(Vec *a, Vec *b) { float r; __asm__ volatile("mula.s %1,%2
	madda.s %3,%4
	madd.s %0,%5,%6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));return r;}
struct Root { char p0[0x792c]; float *x792c; char p1[0x3c]; int x796c; };
struct Obj { struct Root *x0; char p4[0x2c]; float x30[4]; char p40[0xb0]; float xf0[4]; char p100[0x140]; float x240; char p244[0x6c]; float x2b0; char p2b4[0x184]; int x438; char p43c[0xc]; unsigned long x448; };
struct Other { char p0[0x1a4]; int x1a4; char p1a8[0xc4]; int x26c; char p270[0x184]; unsigned char x3f4; };
static inline void Set(struct Other *b,int on) {if(b->x3f4) b->x26c=on;else {b->x1a4=on;b->x26c=0;}}
struct Info { char p0[0x28]; float x28; };
extern struct Info *func_00138558(void *);
extern float func_001385A8(void *);
extern void func_0010F068(struct Obj *,struct Other *);
void func_0010C630(struct Obj *a, struct Other *b) {
 float limit=50.0f, ratio, speed, dot; float diff[4] __attribute__((aligned(16)));
 if(func_00138558((char *)a->x0+0x40c0)->x28>0.1f) limit=50.0f/func_00138558((char *)a->x0+0x40c0)->x28;
 if(func_001385A8((char *)a->x0+0x40c0)>0.25f) limit+=(func_001385A8((char *)a->x0+0x40c0)-0.25f)*50.0f;
 speed=a->x240;
 if(speed<5.0f) { Set(b,1); return; }
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(diff) : "r"(a->x30), "r"(a->x0->x792c+4));
 dot=Dot((Vec *)a->xf0,(Vec *)diff);
 if(dot<0.0f) {Set(b,1); return;}
 if(limit<speed) ratio=1.0f; else ratio=(speed/limit)*0.3f;
 func_0010F068(a,b);
 if(ratio<func_001385A8((char *)a->x0+0x40c0)) {
 Set(b,1);
 a->x448|=2;
 if(func_001385A8((char *)a->x0+0x40c0)>0.7f) a->x2b0=0.03f;
 else if(func_001385A8((char *)a->x0+0x40c0)>0.57f) a->x2b0=0.04f;
 else if(func_001385A8((char *)a->x0+0x40c0)>0.4f) a->x2b0=0.05f;
 else a->x2b0=0.07f;
 if(a->x448&0x10) a->x2b0*=3.0f;
 if(a->x0->x796c==0xa0) a->x438=7;
 }
}
