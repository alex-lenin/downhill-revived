/* UNIT 0x00148860 0x00148AE0 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Body { char pad[16]; Vec x10; char pad20[32]; char x40[64]; char pad80[32]; Vec xA0; } Body;
typedef struct Rider { char pad[0x792C]; Body *x792C; } Rider;
typedef struct Obj {
    char x0[64], x40[64]; Vec x80,x90,xA0; char padB0[16]; Vec xC0; char padD0[16]; char xE0[64];
    float x120,x124,x128; char pad12C[4]; Vec x130; char pad140[0x2B0-0x140];
    Body *x2B0; Rider *x2B4; char pad2B8[4]; int x2BC; char pad2C0[0x1C]; float x2DC,x2E0;
    char pad2E4[0x28]; float x30C,x310; char pad314[0x40]; int x354,x358,x35C; char pad360[8]; int x368;
} Obj;
typedef struct Global { char pad[0x12220]; int x12220[4]; float x12230[4]; } Global;
extern Global D_004225A0;
extern unsigned char D_0043FA34[];
extern int D_004B26AC[];
extern Body *func_0023D040(int);
extern void func_00221ED0(void *,void *);
extern void func_00223348(void *,void *);
extern void func_0014DAA8(Obj *,int);
extern void func_0014CFB0(Obj *);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void Apply(Vec *out, void *mat, Vec *v) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v),"r"(mat) : "memory");
}
static inline void Add(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline void Sub(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline void Normalize(Vec *v) {
    float length,inv,x=v->x,y=v->y,z=v->z;
    __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(length) : "f"(x),"f"(y),"f"(z));
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f),"f"(length));
    v->x=x*inv; v->y=y*inv; v->z=z*inv;
}

void func_00148860(Obj *o) {
    Vec v;
    Body *body;
    Rider *rider;
    int mode;
    float rate;
    float eps = 0.001f;
    if (o->x2B0 && (rider=o->x2B4)) {
        if (o->x358) {
            body=func_0023D040(o->x35C);
            COPY_VECTOR(o->x80,&body->x10);
            COPY_VECTOR(o->x90,&body->xA0);
            func_00221ED0(o,body->x40);
            func_0014DAA8(o,o->x2BC);
        } else {
            Apply(&v,rider->x792C->x40,&o->xA0);
            Add(&o->x80,&rider->x792C->x10,&v);
            func_00221ED0(o,o->x2B4->x792C->x40);
        }
        func_00223348(o->xE0,o);
        func_00221ED0(o->x40,o);
        Sub(&o->xC0,&o->x2B4->x792C->x10,&o->x80);
        Normalize(&o->xC0);
        o->x354=1;
        COPY_VECTOR(o->x2B0->x10,&o->x80);
        COPY_VECTOR(o->x2B0->xA0,&o->x90);
        func_00221ED0(o->x2B0->x40,o);
        o->x120=0.1f; o->x124=0.1f; o->x128=0.1f;
        o->x30C=1000.0f; o->x310=0.002f; o->x368=600;
        __asm__ volatile("sqc2 $vf0, %0" : "=m"(o->x130));
        rate=D_004225A0.x12230[o->x35C];
        mode=D_004225A0.x12220[o->x35C];
        o->x2DC=rate; o->x2E0=rate;
        if (D_004B26AC[0]==10 || D_0043FA34[0]) {
            o->x2DC=1.0f; o->x2E0=1.0f; mode=2;
        }
        func_0014DAA8(o,mode);
        { float d=o->x2DC-(int)o->x2DC; float a=__builtin_fabsf(d);
          __asm__ volatile("L148860_a:" : : "f"(d));
          if (a>eps) func_0014CFB0(o); }
    }
}
