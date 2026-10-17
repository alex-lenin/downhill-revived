/* UNIT 0x001460D0 0x001475B8 .sdata=0x29C568:0x29C5A8 .sbss=0x29DD4C:0x29DD5C */
#include "spatial_batch.h"
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00231E00_2_func_00231E00 func_00231E00
#else
#define CallScalarBridge00231E00_2_func_00231E00 ScalarBridge00231E00_2
#endif
#include "object_registry.h"
#include "particle.h"
#include "include_asm.h"

typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Obj {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0xD0 - 0x20];
    Vec4 xD0;
    char padE0[0x164 - 0xE0];
    int x164;
    int x168;
    char pad16C[0x170 - 0x16C];
    int x170;
    char pad174[0x188 - 0x174];
    float x188, x18C, x190;
} Obj;

typedef struct State {
    char pad0[0x8];
    float x8;
    int xC;
    Vec4 x10;
    Vec4 x20;
} State;

typedef struct FlagsObj { char pad0[0x178]; int x178; int **x17C; } FlagsObj;

struct ResetObj {
    char pad0[0xD00];
    int xD00;
    char padD04[0x14];
    int xD18;
    char padD1C[0xC];
    int xD28;
    char padD2C[4];
    char xD30;
};

float D_0029C568 = -65.0f;
float D_0029C56C = 0.125f;
float D_0029C570 = 0.125f;
float D_0029C574 = 0.5f;
float D_0029C578 = 0.5f;
float D_0029C57C = 20.0f;
float D_0029C580 = 130.0f;
float D_0029C584 = 3000.0f;
float D_0029C588 = 5000.0f;
float D_0029C58C = 400.0f;
float D_0029C590 = 700.0f;
float D_0029C594 = 180.0f;
float D_0029C598 = 100.0f;
float D_0029C59C = 22000.0f;
float D_0029C5A0 = 0.26f;
float D_0029C5A4 = 50.0f;

extern void func_00232BF8(Vec4 *, void *);
extern void func_002327B0(Vec4 *, float);
extern int func_00238F30(void);
extern void func_00231A48(Vec4 *, Vec4 *);
extern void func_002320B0(Vec4 *, Vec4 *, Vec4 *, float, float, float);

typedef struct Group480 {
 char pad0[3328];
 int unkD00;
 char padd04[32];
 int unkD24;
 char padd28[8];
 unsigned char unkD30;
} Group480;
typedef struct Elem {
    void *vtbl;
    char pad4[0xC];
    char x10[0x190];
} Elem;

typedef struct Group {
    Elem elems[8];
    char padD00[0x2C];
    void *xD2C;
} Group;

extern Group D_00372650;
extern void func_00147480(Group480 *);
extern int D_0029DD5C;


extern char *func_0022DD88(void *);

int func_001460D0(float speed, State *state, int arg)
{
    Vec4 m[4];
    char *p;

    func_002226C8(m, &state->x20);
    if (arg != 0 || D_0029C580 < speed) {
        func_0022CDC0((Vector4 *)(&state->x10), 0xB, (Matrix4 *)(m), (int)(0), 3.0f, (Vector4 *)(0));
        func_0022CDC0((Vector4 *)(&state->x10), D_0029DD5C, (Matrix4 *)(m), (int)(0), 1.0f, (Vector4 *)(0));
        return 0;
    }
    p = func_0022DD88(func_0022CDC0((Vector4 *)(&state->x10), 0xB, (Matrix4 *)(m), (int)(0), 1.0f, (Vector4 *)(0)));
    if (p != 0) {
        *(float *)(p + 0x2740) = speed * 0.015625f;
        *(float *)(p + 0x272C) = speed * 0.125f;
    }
    return 1;
}

void func_001461D0(void)
{
    func_00147480((Group480 *)&D_00372650);
}

typedef union { unsigned long flags; struct { unsigned int *ptr; unsigned char x4; } b; } InitFlags;
typedef struct { InitFlags f; char pad8[0xb4-8]; short xb4; } InitTarget;
typedef struct { char pad0[0x30]; void *x30,*x34,*x38,*x3c,*x40; char pad44[0x100-0x44]; } InitBody;
typedef struct { char pad0[0x10]; InitBody body; float x110,x114,x118; char pad11c[0x34]; int x150,x154; float x158,x15c; int x160,x164,x168,x16c,x170,x174,x178; InitTarget *x17c,*x180; int x184; float x188,x18c,x190; char pad194[4]; char x198[8]; } InitObj;
typedef struct { u128 x0,x10,x20,x30; } Matrix;
extern InitTarget *func_00208FB8(void);
extern InitTarget *func_00209108(void);

extern void func_00231B70(InitBody *,float,float);
extern void func_00231BD8(InitBody *);
extern void func_002319A0(InitBody *,float,float,float);
extern void func_00231A08(InitBody *,Vec4 *);
extern void func_00231A78(InitBody *,Matrix *);
extern void func_001D3228(void *);
extern Vec4 D_00373390[];
extern Matrix D_00373410[];
extern float D_0029DD4C,D_0029DD50;
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)
	sq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static inline void CopyMatrix(Matrix *dest,Matrix *matrix) {
 __asm__ volatile("lq $8,0(%1)\n\tlq $9,16(%1)\n\tlq $10,32(%1)\n\tlq $11,48(%1)\n\tsq $8,0(%2)\n\tsq $9,16(%2)\n\tsq $10,32(%2)\n\tsq $11,48(%2)" : "=m"(*dest) : "r"(matrix),"r"(dest),"m"(*matrix),"m"(*matrix) : "$8","$9","$10","$11");
}
void func_001461F0(InitObj *o,unsigned int *handle,Vec4 *pos,Matrix *matrix,int index,float radius) {
 Vec4 p; InitBody *body=&o->body; InitTarget *t; float delta,mass; Matrix *dest;
 o->x17c=func_00208FB8(); delta=(float)index*0.03f;
 o->x154=index; o->x150=0; o->x168=0; o->x170=0; o->x18c=0;
 o->x188=D_0029DD4C-delta; o->x190=D_0029DD50-delta;
 o->x17c->f.flags|=(unsigned long)0x8000<<43;
 t=o->x17c;
 body->x34=(char *)t+0x10; body->x38=(char *)t+0x40;
 if(t->f.flags & ((unsigned long)0x8000<<43)) {
 body->x3c=(char *)t+0x90;body->x40=(char *)t+0xc0;body->x30=(char *)t+0xb0;
 }
 o->x17c->f.b.ptr=handle; o->x17c->f.b.x4=0; o->x17c->f.flags&=~((unsigned long)1<<57);
 o->x17c->xb4=RegisterObjectRetail((void *)(o), (unsigned int)(handle));
 o->x180=func_00209108();o->x180->f.b.ptr=0;o->x180->f.b.x4=0;o->x180->f.flags&=~((unsigned long)1<<57);
 mass=radius*(radius*4.1887903f)*radius*D_0029C5A0;
 p=*pos; o->x158=radius;o->x15c=mass;p.z+=2.0f;
 func_00231B70(body,radius,mass);func_00231BD8(body);func_002319A0(body,0.1f,0.2f,0.3f);
 func_00231A08(body,&p);func_00231A78(body,matrix);
 o->x114=0;o->x110=0;o->x118=D_0029C568*o->x15c;
 COPY_VECTOR(D_00373390[index],&p);dest=&D_00373410[index]; CopyMatrix(dest,matrix);
 func_001D3228(o->x198);o->x178=(*handle>>7)&0x7ff;
}


typedef unsigned int U128 __attribute__((mode(TI)));
typedef union { unsigned long flags; struct { char pad[4]; unsigned char x4; } b; } Flags490;
typedef struct { char pad[0x130]; U128 x130; char pad140[0x28]; int x168; char pad16C[4]; int x170; char pad174[8]; Flags490 *x17C,*x180; } Obj490;
extern u128 D_00373620[];
void func_00146490(Obj490 *o) {
 o->x17C->b.x4=1;
 o->x17C->flags |= (unsigned long)0x8000 << 47;
 o->x17C->flags |= (unsigned long)-1 << 63;
 if(o->x180) o->x180->b.x4=1;
 o->x168=0; o->x170=0; o->x130=D_00373620[0];
}

struct Target4F8 { unsigned long flags; char pad[8]; char x10[1]; };
struct Obj4F8 { char pad0[0x164]; int x164; char pad168[8]; int x170; char pad174[8]; struct Target4F8 *x17C; char pad180[0x18]; char x198[1]; };
extern void func_001D3360(void *, void *);
void func_001464F8(struct Obj4F8 *obj) {
    obj->x170 = 1;
    obj->x17C->flags |= (unsigned long)1 << 57;
    obj->x164 = 0;
    func_001D3360(obj->x198, obj->x17C->x10);
}

typedef struct State548 {
    unsigned long pad0 : 32;
    unsigned long x4 : 8;
    unsigned long pad40 : 17;
    unsigned long b57 : 1;
    unsigned long pad58 : 4;
    unsigned long b62 : 1;
    unsigned long b63 : 1;
} State548;

typedef struct Obj548 {
    char pad0[0x17C];
    State548 *x17C;
    State548 *x180;
} Obj548;

void func_00146548(Obj548 *obj)
{
    obj->x17C->x4 = 0;
    obj->x17C->b62 = 0;
    obj->x17C->b63 = 0;
    if (obj->x180 != 0) {
        obj->x180->x4 = 0;
    }
    obj->x17C->b57 = 0;
}

typedef union Flags5D8 {
    unsigned long flags;
    struct { unsigned int x0, x4; } words;
} Flags5D8;

typedef struct DynState5D8 {
    Flags5D8 f;
    char pad8[8];
    Vec4 x10;
} DynState5D8;

typedef struct Hit5D8 {
    short x0;
    unsigned short x2;
    unsigned short x4;
    char pad6[2];
    float x8;
    int xC;
    Vec4 x10;
    Vec4 x20;
} Hit5D8;

typedef struct Group5D8 {
    Vec4 *x0;
    Vec4 *x4;
    int x8;
    float xC;
    float x10;
    int x14;
    int x18;
    int x1C;
    Hit5D8 *x20;
    int x24;
} Group5D8;

typedef struct Saved5D8 { char data[0xE0]; } Saved5D8;
typedef struct List5D8 { int count; int x4; int entries[16]; } List5D8;

typedef struct Obj5D8 {
    char pad0[0x10];
    InitBody body;
    Vec4 x110;
    char pad120[0x158 - 0x120];
    float x158;
    char pad15C[0x164 - 0x15C];
    int x164;
    int x168;
    char pad16C[4];
    int x170;
    char pad174[0x17C - 0x174];
    DynState5D8 *x17C;
    DynState5D8 *x180;
    float x184;
    float x188;
    float x18C;
    float x190;
    unsigned char x194;
    char pad195[3];
    char x198[8];
} Obj5D8;

extern int func_00146080(void *, void *, float);
extern List5D8 *func_0017D420(List5D8 *);
extern void func_0017D430(List5D8 *);
extern int func_0017D440(List5D8 *, int);
extern void func_001D3250(void *, void *);
extern void func_00211280(void *);
extern void func_00211610(Hit5D8 *, int, Hit5D8 **, int *);
extern void func_00231C10(void *);
extern void func_00231C20(void *, void *);
extern void func_00232B60(void *, void *);
extern Vec4 D_00373610;

struct ObjA40;
struct ObjAA0;
void func_00146A40(struct ObjA40 *);
void func_00146AA0(struct ObjAA0 *);
void func_00146CC8(Obj *, State *, void *, int);
void func_00146DC0(Obj *, State *, void *);
void func_00146E60(Obj *, State *, void *, int);

static inline void SubVector5D8(Vec4 *out, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
        : "=m" (*out) : "r" (a), "r" (b) : "memory");
}

static inline float Length5D8(Vec4 *v)
{
    float result;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
        : "=&f" (result) : "f" (v->x), "f" (v->y), "f" (v->z));
    return result;
}

int func_001465D8(void *arg)
{
    Obj5D8 *obj = arg;
    Saved5D8 saved;
    Vec4 before;
    Vec4 after;
    Group5D8 groups[2];
    Hit5D8 hits[2][8];
    Hit5D8 *sorted[12];
    Vec4 down;
    union { List5D8 list; Vec4 delta; } scratchList;
    List5D8 *listp;
    int n;
    int n2;
    InitBody *body;
    void *scratch;
    Vec4 *beforep;
    DynState5D8 *state;
    Vec4 *statePos;
    unsigned char special;
    unsigned long flags;
    unsigned long mask;
    int saved57;
    int i;
    Hit5D8 *hitBase;
    register Hit5D8 *hit;
    Hit5D8 *first;
    Vec4 *delta;
    float dist;

    if (obj->x170 == 0) {
        goto finish;
    }
    scratch = &obj->body;
    COPY_VECTOR(before, (Vec4 *)((InitBody *)scratch)->x34);
    body = (InitBody *)scratch;
    scratch = &before;
    beforep = (Vec4 *)scratch;
    special = func_00146080(scratch, &D_00373610, D_0029C584) ^ 1;
    if (special != 0 && func_00146080(beforep, &D_00373610, D_0029C588) == 0) {
        obj->x170 = 0;
    }
    if (obj->x170 == 0) {
        goto finish;
    }
            func_00231C10(body);
            func_00231C20(body, &obj->x110);
            func_00232B60(body, &saved);
            func_002327B0((Vec4 *)body, 1.0f / (float)func_00238F30());
            COPY_VECTOR(after, (Vec4 *)body->x34);
            statePos = &obj->x17C->x10;
            if (obj->x194 != 0) {
                
                n = 0;
                
                func_0017D420(&scratchList.list);
                func_0017D430(&scratchList.list);
                obj->x164++;
                state = obj->x17C;
                flags = state->f.flags;
                saved57 = (int)(flags >> 57) & 1;
                state->f.flags = flags & ~((unsigned long)1 << 57);
                hitBase = hits[0];

                groups[0].x0 = &before;
                groups[0].x4 = &after;
                groups[0].x8 = 0;
                groups[0].xC = 1.0f;
                groups[0].x20 = hits[0];
                groups[0].x10 = obj->x158;
                groups[0].x1C = 8;
                groups[0].x14 = 0;
                groups[0].x18 = 0;

                COPY_VECTOR(down, statePos);
                down.z -= 200.0f;
                groups[1].x0 = statePos;
                groups[1].x20 = hits[1];
                groups[1].x4 = &down;
                groups[1].x8 = 0;
                groups[1].xC = 1.0f;
                groups[1].x10 = 0.5f;
                groups[1].x1C = 8;
                groups[1].x14 = 0;
                groups[1].x18 = 0;

                
                func_00214BA0((SpatialBatchQuery *)(void *)(groups), 2);
                func_00211610(hitBase, groups[0].x24, sorted, &n);
                obj->x17C->f.flags = (obj->x17C->f.flags & ~((unsigned long)1 << 57)) |
                    ((unsigned long)saved57 << 57);

                if (n > 0) {
                    if (0.0f < hitBase->x10.w) {
                        goto positive_hit;
                    }
                    func_00146DC0((Obj *)obj, (State *)hitBase, &saved);
                }
collision_loop:
                for (i = 0; i < n; i++) {
                        hit = &hits[0][i];
                        if ((unsigned int)(hit->x2 - 0x1CE8) < 0x64U) {
                            if (func_0017D440(&scratchList.list, hit->x4) != 0) {
                                func_00146E60((Obj *)obj, (State *)hit, &saved, special);
                                func_001D3250(obj->x198, &obj->x17C->x10);
                            }
                        }
                        }
            }

            func_00146A40((struct ObjA40 *)obj);
            func_00146AA0((struct ObjAA0 *)obj);
            if (obj->x180 != 0 && obj->x180->f.words.x0 != 0) {
                if (obj->x194 != 0) {
                    n2 = 0;
                    func_00211610(hits[1], groups[1].x24, sorted, &n2);
                    if (n2 == 0) {
                        goto zero_distance;
                    }
                    first = sorted[0];
                    delta = &scratchList.delta;
                    SubVector5D8(&scratchList.delta, statePos, &down);
                    dist = Length5D8(&scratchList.delta);
                    obj->x184 = dist * first->x10.w - 1.0f;
                    goto distance_done;
positive_hit:
                    func_00146CC8((Obj *)obj, (State *)hitBase, &saved, special);
                    func_001D3250(obj->x198, &obj->x17C->x10);
                    goto collision_loop;
zero_distance:
                    obj->x184 = 0.0f;
distance_done:
                    ;
                }
                COPY_VECTOR(obj->x180->x10, &obj->x17C->x10);
                obj->x180->x10.z -= obj->x184;
            }
    if (obj->x170 == 0) {
        func_00146548((Obj548 *)obj);
        goto finish;
    } else {
        func_00211280(obj->x17C);
    }

finish:
    obj->x194 ^= 1;
    if (obj->x170 != 0) {
        if (obj->x194 != 0) {
            obj->x17C->f.flags |= (unsigned long)1 << 57;
        } else {
            obj->x17C->f.flags &= ~((unsigned long)1 << 57);
        }
    }
    return obj->x170 != 0;
}





typedef struct ObjA40 {
    char pad0[0x70];
    float velocity[4];
} ObjA40;

static inline float LengthSq(float *v)
{
    float result;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3"
        : "=f" (result) : "f" (v[0]), "f" (v[1]), "f" (v[2]) : "memory");
    return result;
}

void func_00146A40(ObjA40 *obj) {
    float *v = obj->velocity;
    float length;
    float limit;

    length = LengthSq(v);
    limit = D_0029C59C;
    if (limit * limit < length) {
        float root, scale;
        __asm__ ("sqrt.s %0, %1" : "=f" (root) : "f" (length));
        scale = limit / root;
        __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tmfc1 $2, %1\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0x70(%2)"
            : : "r" (v), "f" (scale), "r" (obj) : "$2", "memory");
    }
}

typedef struct ColorObj {
    char pad0[0x20];
    float x20;
    float x24;
    float x28;
    float x2C;
} ColorObj;

typedef struct ObjAA0 {
    char pad0[0xD0];
    Vec4 xD0;
    char padE0[0x130 - 0xE0];
    Vec4 x130;
    char pad140[0x17C - 0x140];
    ColorObj *x17C;
} ObjAA0;

extern float func_00220D98(void);

static inline void InterVector(Vec4 *out, Vec4 *a, float *b, float t)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)
\n\t"
                      "mfc1 $2, %3
\n\t"
                      "lqc2 $vf12, 0(%2)
\n\t"
                      "qmtc2.ni $2, $vf13
\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x
\n\t"
                      "vmaddax.xyz ACC, $vf12, $vf13x
\n\t"
                      "vmsubx.xyz $vf11, $vf11, $vf13x
\n\t"
                      "sqc2 $vf11, 0(%0)"
        : : "r" (out), "r" (a), "r" (b), "f" (t) : "$2", "memory");
}

void func_00146AA0(ObjAA0 *obj)
{
    Vec4 color;
    float scale = 0.026041666f;
    float *vel = &obj->xD0.x;
    float speed;
    float t;
    float k;
    Vec4 *pos;

    __asm__ volatile ("mula.s %1, %1
\n\tmadda.s %2, %2
\n\tmadd.s %0, %3, %3
\n\tsqrt.s %0, %0"
        : "=&f" (speed) : "f" (vel[0]), "f" (vel[1]), "f" (vel[2]));
    speed *= 0.0056818184f;
    __asm__ volatile ("min.s %0, %1, %2" : "=&f" (t) : "f" (speed), "f" (1.0f));
    k = func_00220D98();
    pos = &obj->x130;
    InterVector(&color, pos, &obj->x17C->x20, k * (t * t));
    pos->x = color.x;
    pos->y = color.y;
    pos->z = color.z;
    obj->x17C->x20 = obj->x130.x;
    obj->x17C->x24 = obj->x130.y;
    obj->x17C->x28 = obj->x130.z;
    obj->x17C->x2C = (color.x * 0.3f + color.y * 0.6f + color.z * 0.1f) * scale;
}

extern void func_00232A80(void *, Vec4 *, void *);
extern void func_00155E78(void *, int);


void func_00146BE0(void *unused, char *obj, char *data)
{
    Vec4 vectors[3];
    char *src = data + 0x10;
    char *target;
    char *vtbl;
    float length;
    float inverse;
    float x, y, z;
    float threshold;
    int bits;

    func_00232A80(obj + 0x40C0, &vectors[0], src);
    target = (char *)func_0017D3D0(*(unsigned short *)(data + 4));
    if (target != 0) {
        vtbl = *(char **)target;
        ((void (*)(void *, void *, Vec4 *))*(void **)(vtbl + 0x1C))(
            target + *(short *)(vtbl + 0x18), src, &vectors[2]);
        __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%0)"
            : : "r"(&vectors[0]), "r"(&vectors[2]) : "memory");
    }

    x = vectors[0].x;
    y = vectors[0].y;
    z = vectors[0].z;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\tlui %2, 0x3F80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
        : "=&f"(inverse), "=&f"(length), "=&r"(bits)
        : "f"(x), "f"(y), "f"(z));
    threshold = D_0029C5A4;
    vectors[1].x = x * inverse;
    vectors[1].y = y * inverse;
    vectors[1].z = z * inverse;
    if (threshold < length) {
        func_00155E78(obj + 0x3FD0, 2);
    }
}

static inline float Dot(Vec4 *a, Vec4 *b)
{
    float result;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
        : "=f" (result)
        : "f" (a->x), "f" (b->x), "f" (a->y), "f" (b->y), "f" (a->z), "f" (b->z));
    return result;
}

void func_00146CC8(Obj *obj, State *state, void *arg2, int arg3)
{
    Vec4 *pos = &obj->x10;
    float speed;

    func_00232BF8(pos, arg2);
    func_002327B0(pos, state->x10.w / (float)func_00238F30());
    speed = -Dot(&obj->xD0, &state->x20);
    func_002320B0(pos, &state->x10, &state->x20, obj->x188, obj->x18C, obj->x190);
    if (arg3 != 0 || D_0029C57C < speed) {
        obj->x170 = func_001460D0(speed, state, arg3);
    }
    obj->x168 = 0;
    obj->x164 = 0;
}

void func_00146DC0(Obj *obj, State *state, void *arg2) {
    Vec4 step;
    Vec4 *pos = &obj->x10;
    Vec4 *dir = &state->x20;

    func_00232BF8(pos, arg2);
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(step) : "r"(dir), "f"(state->x8) : "$2");
    func_00231A48(pos, &step);
    func_002320B0(pos, &state->x10, dir, obj->x188, obj->x18C, obj->x190);
    obj->x168++;
}



#if defined(__mips__)
extern void func_00231E00(Vec4 *, Vec4 *, Vec4 *, Vec4 *, float, float, float);
#else

#endif


void func_00146E60(Obj *obj, State *state, void *arg2, int arg3)
{
    Vec4 *pos = &obj->x10;
    float speed = -Dot(&obj->xD0, &state->x20);
    Vec4 *target = (Vec4 *)((char *)func_0017D3D0(*(unsigned short *)((char *)state + 4)) + 0x10);
    CallScalarBridge00231E00_2_func_00231E00(pos, target, &state->x10, &state->x20, obj->x188, obj->x18C, obj->x190);
    if (arg3 != 0 || D_0029C57C < speed) {
        obj->x170 = func_001460D0(speed, state, arg3);
    }
}

void func_00146F38(FlagsObj *obj) {
    int *flags = *obj->x17C;
    if (flags) {
        *flags = (*flags & (int)0xFFFC007F) | ((obj->x178 & 0x7FF) << 7);
    }
}

extern u128 D_0026E520[];
extern u128 D_0026E530[];
extern u128 D_00373620[];
extern int D_004B2840[];
extern float D_0029DD4C;
extern float D_0029DD50;
extern float D_0029DD54;
extern float D_0029DD58;

void func_00146F78(struct ResetObj *obj)
{
    obj->xD00 = 0;
    obj->xD18 = 0;
    obj->xD28 = 0;
    obj->xD30 = 0;
    if (D_004B2840[0] == 3) {
        D_0029DD5C = 0x22;
        D_0029DD4C = D_0029C570;
        D_0029DD50 = D_0029C578;
        D_0029DD54 = D_0029C590;
        D_0029DD58 = D_0029C598;
        D_00373620[0] = D_0026E520[0];
    } else {
        D_0029DD5C = 0xC;
        D_0029DD4C = D_0029C56C;
        D_0029DD50 = D_0029C574;
        D_0029DD54 = D_0029C58C;
        D_0029DD58 = D_0029C594;
        D_00373620[0] = D_0026E530[0];
    }
}

typedef struct AddEntry {
    char pad0[0x44];
    u128 *x44;
    char pad48[0x1A0 - 0x48];
} AddEntry;

typedef struct AddObj {
    AddEntry entries[8];
    int xD00;
    char padD04[0xD28 - 0xD04];
    unsigned int *xD28;
} AddObj;

extern Vec4 D_00373610;

void func_00147018(AddObj *obj, unsigned int *handle, Vec4 *pos, Matrix *matrix)
{
    float size;

    /* bits 7-17 of *handle: size, default 10 */
    if ((*handle & 0xFFFC0000) == 0x752C0000) {
        obj->xD28 = handle;
    } else {
        if ((*handle & 0x3FF80) == 0) {
            *handle = (*handle & 0xFFFC007F) | 0x500;
        }
        size = (*handle >> 7) & 0x7FF;
        func_001461F0((InitObj *)&obj->entries[obj->xD00], handle, pos, matrix, obj->xD00,
                      size * 0.5f);
        *handle &= 0xFFFC007F;
        if (obj->xD00 == 0) {
            u128 *src = obj->entries[0].x44;
            COPY_VECTOR(D_00373610, src);
        }
        obj->xD00++;
    }
}



void func_00147128(char *arg0, unsigned int *arg1) {
    unsigned int value = func_0017D3A0((void *)(arg0 + 0xD2C));
    *arg1 = (*arg1 & 0xFFFC007F) | ((value & 0x7FF) << 7);
}

typedef struct Slot170 {
    char pad0[0x180];
    int *x180;
    char pad184[0x10];
    char x194;
    char pad195[0xB];
} Slot170;

typedef struct Team170 {
    Slot170 slots[8];
    int count;
    int xD04;
    int xD08[4];
    int xD18;
    int xD1C;
    char padD20[4];
    int xD24;
    int xD28;
} Team170;

typedef struct World170 {
    char pad0[0x50130];
    int x50130[1];
} World170;

extern World170 D_002AF2C0;

static inline void SetIfPresent(int *p, int value)
{
    if (p != 0) {
        *p = value;
    }
}


static inline void SlotSetX180(Slot170 *slot, int value)
{
    if (slot->x180 != 0) {
        *slot->x180 = value;
    }
}

void func_00147170(Team170 *team)
{
    Slot170 *slot;
    int i;
    int j;
    int id;

    team->xD04 = 0;
    team->xD1C = 0;
    team->xD18 = 0;
    while ((id = D_002AF2C0.x50130[team->xD18]) != 0) {
        team->xD08[team->xD18++] = id;
    }
    team->xD24 = team->count == 0 ? 3 : 0;
    if (team->xD28 != 0) {
        for (i = 0; i < team->count; i++) {
            SlotSetX180(&team->slots[i], team->xD28);
        }
    }
    for (j = 0; j < team->count; j++) {
        slot = &team->slots[j];
        if (j < team->count / 2) {
            slot->x194 = 1;
        } else {
            slot->x194 = 0;
        }
    }
}

typedef struct Entry { char data[0x1a0]; } Entry;
typedef struct EntryList { Entry entries[8]; int count; } EntryList;

void func_00147268(EntryList *obj) {
 int i;
 for (i=0;i<obj->count;++i) func_00146F38((FlagsObj *)&obj->entries[i]);
}

typedef struct FarObj { char pad[0x792C]; char *x792C; } FarObj;
typedef struct EntryC8 { char data[0x1A0]; } EntryC8;
typedef struct GroupC8 {
    EntryC8 entries[8];
    int count;
    int unkD04;
    FarObj *ptrs[4];
    int limit;
    int index;
    int unkD20;
    int state;
    void *unkD28;
    int unkD2C;
    unsigned char flag;
} GroupC8;
extern int func_00146080(void *, void *, float);
extern int func_001465D8(void *);
void func_001472C8(GroupC8 *obj) {
    int i;
    int j;
    int k;
    int flag;
    switch (obj->state) {
    case 0:
        if (func_00146080(obj->ptrs[obj->index]->x792C + 0x10, &D_00373610, D_0029DD54) != 0) {
            for (i = 0; i < obj->count; i++) {
                func_00146490((void *)&obj->entries[i]);
            }
            obj->state = 1;
        }
        obj->index = obj->index + 1;
        if (obj->index >= obj->limit) {
            obj->index = 0;
        }
        break;
    case 1:
        if (func_00146080(obj->ptrs[obj->index]->x792C + 0x10, &D_00373610, D_0029DD58) != 0) {
            for (j = 0; j < obj->count; j++) {
                func_001464F8((void *)&obj->entries[j]);
            }
            obj->state = 2;
            obj->flag = 1;
        }
        obj->index = obj->index + 1;
        if (obj->index >= obj->limit) {
            obj->index = 0;
        }
        break;
    case 2:
        flag = 0;
        for (k = 0; k < obj->count; k++) {
            flag |= func_001465D8((void *)&obj->entries[k]) != 0;
        }
        if (flag == 0) {
            obj->state = 3;
        }
        break;
    }
}

void func_00147480(Group480 *arg0) {
    int var_s2;
    void *var_s0;

    if (arg0->unkD30 == 0) {
        var_s2 = 0;
        if (arg0->unkD00 > 0) {
            var_s0 = arg0;
            do {
                var_s2 += 1;
                func_00146490(var_s0);
                func_001464F8(var_s0);
                var_s0 += 0x1A0;
            } while (var_s2 < arg0->unkD00);
        }
        arg0->unkD24 = 2;
        arg0->unkD30 = 1U;
    }
}

float D_0029DD4C __attribute__((section(".sbss"))) = 0;
float D_0029DD50 __attribute__((section(".sbss"))) = 0;
float D_0029DD54 __attribute__((section(".sbss"))) = 0;
float D_0029DD58 __attribute__((section(".sbss"))) = 0;

extern char D_00284258[];
extern char D_00284290[];
extern void func_002318C0(void *);

/* static initializer of the unit's global objects (called by func_001475B8) */
void func_00147508(int initialize, int priority)
{
    Group *group;
    Elem *elem;
    int i;

    if (priority == 0xFFFF && initialize) {
        group = &D_00372650;
        elem = group->elems;
        for (i = 7; i != -1; i--) {
            elem->vtbl = D_00284258;
            func_002318C0(elem->x10);
            elem++;
        }
        group->xD2C = D_00284290;
        D_0029DD4C = D_0029C56C;
        D_0029DD50 = D_0029C574;
        D_0029DD54 = D_0029C58C;
        D_0029DD58 = D_0029C594;
    }
}
