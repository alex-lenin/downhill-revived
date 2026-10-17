/* UNIT 0x00147F40 0x00148140 */
typedef float Vec[4] __attribute__((aligned(16)));
typedef struct { char pad[0x14]; float x14,x18,x1c; } Params;
typedef struct { int x0; Params *x4; void *x8; int xc; Vec x10,x20,x30,x40,x50,x60,x70,x80,x90; } Obj;
extern float func_00223728(float);
extern void func_00222EF0(float *,float *);
extern void func_002235B0(float *,float *,float *);
extern int func_00238F30(void);
extern void func_00209248(void *,float *,float *,float);
extern void func_00211280(void *);
#define COPY(d,s) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(d) : "r"(s) : "$2")
#define CROSS(d,a,b) __asm__ volatile("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(d) : "r"(b),"r"(a) : "memory")
#define ADD_SCALE(d,a,b,s) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(d) : "r"(a),"r"(b),"f"(s) : "$2","memory")
void func_00147F40(Obj *o,int count,float offset,float scale) {
 Vec result,rotation,quat; float value,phase; float *pos;
 phase=(float)count/o->x4->x14-offset;
 while(phase>1.0f) phase-=2.0f;
 pos=o->x60;
 while(phase< -1.0f) phase+=2.0f;
 value=func_00223728(phase);
 ADD_SCALE(o->x60,o->x10,o->x20,o->x4->x18*value*scale);
 COPY(rotation,o->x30);
 rotation[3]=o->x4->x1c*value*scale;
 func_00222EF0(quat,rotation);
 func_002235B0(result,quat,o->x40);
 COPY(o->x90,result); COPY(o->x80,o->x30);
 CROSS(o->x70,o->x80,o->x90);
 func_00209248(o->x8,pos,o->x70,1.0f/(float)func_00238F30());
 func_00211280(o->x8);
}
