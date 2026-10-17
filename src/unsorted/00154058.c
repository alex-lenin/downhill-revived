/* UNIT 0x00154058 0x00154E10 .sdata=0x29C674:0x29C6A8 */
#include "spatial.h"
/* Retains the EE argument setup; guarded native permutation remains. */
#include "object_registry.h"
#include "particle.h"
/* Retains retail call scheduling; guarded PC permutations remain where needed. */
#include "include_asm.h"

typedef struct State { char pad[0x6390]; int x6390; } State;
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Other { char pad0[0x4180]; Vec x4180; char pad4190[0x792C - 0x4190]; int x792C; char pad7930[0x260]; char x7B90; char pad7B91[3]; State *x7B94; } Other;
typedef struct Obj {
    State *x0;
    char pad4[0x24];
    char *x28;
    int x2C;
    Vec x30;
    Vec x40;
    char x50[0x40];
    int x90;
    float x94;
    Other *x98;
    int x9C;
} Obj;
typedef struct Body { char pad0[16]; Vec x10; char pad20[32]; char x40[16]; Vec x50; } Body;
typedef struct Rider { char pad0[0x4180]; Vec x4180; char pad4190[0x792C-0x4190]; Body *x792C; } Rider;
typedef struct ObjB90 { Rider *x0; char pad4[0x24]; Body *x28; char pad2C[4]; Vec x30; char pad40[0x50]; int x90; float x94; Rider *x98; Body *x9C; } ObjB90;

extern Other *func_00154E10(Obj *, float, float);
extern int func_00154F40(Obj *, float, float);
extern void func_001544C8(Obj *), func_00154130(Obj *), func_001544F8(Obj *);
extern void func_00209060(char *);

extern int func_00238F48(void);
extern int func_00238F30(void);
extern float func_00138580(void *);
extern float func_00222B68(float min, float max);
extern void func_00222040(void *, float *);
extern void func_00154B90(ObjB90 *);
extern float func_002487A0(float);
extern float func_00236088(float, float, float, float);

float D_0029C674 = 0.03f;
float D_0029C678 = 0.06f;
float D_0029C67C = 30.0f;
float D_0029C680 = 65.0f;
float D_0029C684 = 85.0f;
int D_0029C688 = 60;
float D_0029C68C = 4.0f;
float D_0029C690 = 100.0f;
float D_0029C694 = 0.25f;
float D_0029C698 = 0.3f;
float D_0029C69C = 1.65f;
float D_0029C6A0 = 0.3f;

void func_00154058(Obj *obj) {
 State *state=obj->x0; Other *other;
 if(obj->x2C<0) {
  if(state->x6390==4) {
   other=func_00154E10(obj,D_0029C67C*0.017453292f,D_0029C680);
   obj->x98=other;
   if(other) { other->x7B94=state; other->x7B90=1; obj->x9C=obj->x98->x792C; }
   else obj->x9C=0;
  }
 } else {
  obj->x2C++;
  func_001544C8(obj); func_00154130(obj); func_001544F8(obj);
  if(obj->x2C>D_0029C688) {func_00209060(obj->x28); obj->x28=0; obj->x2C=-1;}
 }
}

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void Sub(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline void Add(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline void Scale(Vec *out, Vec *v, float scale) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v),"f"(scale) : "$2");
}
static inline void ScaleAdd(Vec *out, Vec *a, Vec *b, float scale) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b),"f"(scale) : "$2");
}
static inline float Length(Vec *v) {
    float length;
    __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0" : "=&f"(length) : "f"(v->x),"f"(v->y),"f"(v->z));
    return length;
}
static inline float Normalize(Vec *out, Vec *v) {
    float len, inv, x = v->x, y = v->y, z = v->z;
    /* SDK length and reciprocal sequence, as in the promoted vector helpers. */
    __asm__ volatile ("mula.s %2, %2
	madda.s %3, %3
	madd.s %1, %4, %4
	sqrt.s %1, %1
	"
                      "lui $2, 0x3f80
	mtc1 $2, %0
	nop
	div.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len) : "f"(x), "f"(y), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}
static inline float NormV(Vec *out, float x, float y, float z) {
    float len, inv;
    int bits;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}
static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
    return r;
}

void func_00154130(Obj *obj)
{
    Vec delta;
    Vec work;
    Vec nvel;
    Vec tmp;
    Other *other;
    Vec *dir;
    float t;
    float fvar;
    float len;
    float inv;
    float dot;
    float speed;
    int dt;

    COPY_VECTOR(obj->x40, obj->x28 + 0x10);
    dt = func_00238F48() - obj->x90;
    t = (float)dt * 0.016666671f;
    fvar = D_0029C69C * t * t;
    if (D_0029C6A0 < fvar) {
        fvar = D_0029C6A0;
    }
    obj->x30.z -= fvar;
    dir = &obj->x30;
    if (obj->x9C != 0) {
        Sub(&delta, (Vec *)((char *)obj->x9C + 0x10), (Vec *)(obj->x28 + 0x10));
        len = Length(&delta);
        NormV(&work, dir->x, dir->y, dir->z);
        dot = Dot(&work, &delta);
        inv = 1.0f / len;
        if (dot * inv < func_002487A0(D_0029C67C) || D_0029C680 < len) {
            obj->x98 = 0;
            obj->x9C = 0;
        }
        if (obj->x9C != 0) {
            Sub(&work, (Vec *)((char *)obj->x9C + 0x10), (Vec *)(obj->x28 + 0x10));
            other = obj->x98;
            if (other != 0) {
                len = Length(&work);
                len /= obj->x94;
                speed = Length(&other->x4180);
                speed = (speed / (float)func_00238F30()) * len;
                Normalize(&nvel, &other->x4180);
                Scale(&tmp, &nvel, speed);
                Add(&work, &work, &tmp);
            }
            NormV(&delta, work.x, work.y, work.z);
            NormV(&nvel, obj->x30.x, dir->y, dir->z);
            tmp.x = func_00236088(nvel.x, delta.x, D_0029C674, 0.001f);
            tmp.y = func_00236088(nvel.y, delta.y, D_0029C674, 0.001f);
            tmp.z = func_00236088(nvel.z, delta.z, D_0029C678, 0.001f);
            Scale(&obj->x30, &tmp, obj->x94);
        }
    }
    {
        char *base = obj->x28;
        char *p10 = base + 0x10;
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0x10(%0)" : : "r"(base), "r"(p10), "r"(dir) : "memory");
    }
}

void func_001544C8(Obj *obj) { void *p = obj->x28 + 0x40; func_00221930(p, p, obj->x50); }


typedef struct F8Effect {
    char pad0[0x281C];
    int x281C;
} F8Effect;
typedef struct F8State {
    char pad0[0x6390];
    int x6390;
    char pad6394[0x792C - 0x6394];
    char *x792C;
    char pad7930[0x797C - 0x7930];
    int x797C;
    char pad7980[0x7A44 - 0x7980];
    F8Effect *x7A44;
    char pad7A48[0x7A90 - 0x7A48];
    char x7A90[0x40];
    char pad7AD0[0x7BD0 - 0x7AD0];
} F8State;
typedef struct Hit {
    int x0;
    unsigned short x4, x6;
    int x8, xC;
    Vec x10, x20;
} Hit;
typedef struct Vtbl {
    char pad[0x10];
    short delta;
    short pad2;
    void (*fn)(void *, Vec *, F8State *);
} Vtbl;
typedef unsigned int Quad __attribute__((mode(TI)));

extern struct {
    F8State objects[10];
    char pad[0x501A8 - 10 * 0x7BD0];
    int count;
} D_002AF2C0;


extern int func_00161868(int, void *);
/* Declared with the float before the last two arguments: same registers under the
   EE ABI ($f12 for the float), but the argument moves are emitted in this order,
   which retail's scheduling of `mov.s $f12, $f20` needs. */
extern void func_001D4788(int, int);
extern void func_001B6488(Vec *, Vec *, Vec *, int, int);
extern void func_0012E940(F8State *, int, Vec *, F8State *);
extern char D_00398C40[];

static inline void SubVec(Vec *out, void *a, void *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b) : "memory");
}

static inline void AddVec(Vec *out, void *a, void *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b) : "memory");
}

static inline void ScaleVec(Vec *out, void *v, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline void AddScaled(Vec *out, void *a, void *b, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

static inline float NormalizeF8(Vec *out, Vec *v) {
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len) : "f"(x), "f"(y), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

void func_001544F8(Obj *obj)
{
    Vec diff;
    Vec scaled;
    Vec bounce;
    Vec v30;
    Hit hit;
    Vec dir;
    F8State *world;
    F8State *best;
    Vtbl *vt;
    void *target;
    float minDist;
    float dist;
    float dot;
    int i;
    int effect;
    unsigned short type;

    world = (F8State *)obj->x0;
    best = 0;
    minDist = 999999.9f;
    for (i = 0; i < D_002AF2C0.count; i++) {
        F8State *p = &D_002AF2C0.objects[i];
        if (i != world->x797C) {
            float dx, dy, dz, ax, az, lim;
            /* Zero-byte labels (same device as src/unsorted/001AD2B8.c): they keep each
               component live past its abs.s (so |d| gets its own register) and keep the
               limit load below the first abs.s, as retail does. */
            SubVec(&diff, p->x792C + 0x10, obj->x28 + 0x10);
            dx = diff.x;
            ax = __builtin_fabsf(dx);
            __asm__ volatile ("L001544F8_dx:" : : "f"(ax));
            lim = D_0029C68C;
            if (ax < lim) {
                dy = diff.y;
                if (__builtin_fabsf(dy) < lim) {
                    dz = diff.z;
                    az = __builtin_fabsf(dz);
                    __asm__ volatile ("L001544F8_dz:" : : "f"(az), "f"(dz));
                    if (az < lim) {
                        /* horizontal distance (x, y only) */
                        __asm__ volatile ("mula.s %1,%1\n\tmadd.s %0,%2,%2\n\tsqrt.s %0,%0"
                                          : "=f"(dist) : "f"(dx), "f"(dy));
                        if (dist < minDist) {
                            minDist = dist;
                            best = p;
                        }
                    }
                }
            }
        }
    }

    {
        float k;
        Vec *vp = &obj->x30;
        k = D_0029C690;
        ScaleVec(&scaled, vp, k);
        if (best != 0) {
            SubVec(&v30, best->x792C + 0x10, world->x792C + 0x10);
            NormalizeF8(&v30, &v30);
            ScaleVec(&scaled, &v30, k * 5.0f);
            func_0012E940(best, 4, &scaled, world);
            SubVec(&bounce, best->x792C + 0x10, best->x792C + 0x90);
            AddVec(&bounce, &bounce, &obj->x30);
            ScaleVec(&bounce, &bounce, 0.5f);
            effect = func_00161868(0x80C0C0C0, best->x7A90);
            func_0022CDC0((Vector4 *)(obj->x28 + 0x10), 0x35, (Matrix4 *)(0), (int)(effect), 1.0f, (Vector4 *)(&bounce));
            {
                char *base = D_00398C40;
                func_001D4788((int)(base + 0x240), (int)(obj->x28 + 0x10));
            }
            func_00209060(obj->x28);
            obj->x28 = 0;
            obj->x2C = -1;
        } else if (QuerySpatialHitsFloatFirst((Vector4 *)(&obj->x40), (Vector4 *)(obj->x28 + 0x10), 1.0f, 0.0f, 0.0f, (SpatialHit *)(&hit), 1) != 0) {
            target = func_0017D3D0(hit.x4);
            if (target != 0) {
                Vtbl *vtab;
                ScaleVec(&scaled, &scaled, 2.0f);
                vtab = *(Vtbl **)target;
                vtab->fn((char *)target + vtab->delta, &scaled, world);
                ScaleVec(&bounce, &obj->x30, 0.25f);
            } else {
                float *v = (float *)&obj->x30;
                __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
                                  : "=f"(dot) : "f"(hit.x20.x), "f"(v[0]), "f"(hit.x20.y),
                                    "f"(v[1]), "f"(hit.x20.z), "f"(v[2]));
                AddScaled(&bounce, &obj->x30, &hit.x20, dot * -2.0f);
                ScaleVec(&bounce, &bounce, 0.25f);
                effect = world->x7A44->x281C;
                type = (unsigned short)(*(unsigned long *)&hit >> 53) & 0x3F;
                *(Quad *)&v30 = *(Quad *)&hit.x10;
                *(Quad *)&dir = *(Quad *)&hit.x20;
                func_001B6488(&v30, &bounce, &dir, effect, type);
            }
            effect = func_00161868(0x80C0C0C0, world->x7A90);
            func_0022CDC0((Vector4 *)(&hit.x10), 0x35, (Matrix4 *)(0), (int)(effect), 1.0f, (Vector4 *)(&bounce));
            {
                char *base = D_00398C40;
                func_001D4788((int)(base + 0x240), (int)(obj->x28 + 0x10));
            }
            func_00209060(obj->x28);
            obj->x2C = -1;
            obj->x9C = 0;
            obj->x98 = 0;
            obj->x28 = 0;
        } else {
            char *p = (char *)obj->x9C;
            if (p != 0) {
                float x, y, z, ax, ay, az;
                SubVec(&diff, p + 0x10, obj->x28 + 0x10);
                x = diff.x;
                ax = __builtin_fabsf(x);
                __asm__ volatile ("L001544F8_x:" : "+f"(x) : "f"(ax));
                if (ax < 4.0f) {
                    y = diff.y;
                    ay = __builtin_fabsf(y);
                    __asm__ volatile ("L001544F8_y:" : "+f"(y) : "f"(ay));
                    if (ay < 4.0f) {
                        z = diff.z;
                        az = __builtin_fabsf(z);
                        __asm__ volatile ("L001544F8_z:" : "+f"(z) : "f"(az));
                        if (az < 4.0f) {
                            target = func_0017D3D0((*(*(unsigned int **)p) >> 7) & 0x7FF);
                            if (target != 0) {
                                ScaleVec(&scaled, &scaled, 2.0f);
                                vt = *(Vtbl **)target;
                                vt->fn((char *)target + vt->delta, &scaled, world);
                                ScaleVec(&bounce, &obj->x30, 0.25f);
                                effect = func_00161868(0x80C0C0C0, world->x7A90);
                                SpawnParticleScaleBeforeEffect((Vector4 *)(obj->x28 + 0x10), 0x35, (Matrix4 *)(0), 1.0f, (int)(effect), (Vector4 *)(&bounce));
                                {
                                    char *base = D_00398C40;
                                    func_001D4788((int)(base + 0x240), (int)(obj->x28 + 0x10));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}



void func_00154A70(Obj *obj)
{
    float vec[4];
    char *world;
    Other *other;

    obj->x2C = 0;
    world = (char *)obj->x0;
    obj->x90 = func_00238F48();
    obj->x94 = D_0029C684 * 1.4666667f + func_00138580(world + 0x40C0);
    obj->x94 *= 1.0f / func_00238F30();
    vec[0] = func_00222B68(-D_0029C694, D_0029C694);
    vec[1] = func_00222B68(-D_0029C694, D_0029C694);
    vec[2] = func_00222B68(-D_0029C694, D_0029C694);
    func_00222040(obj->x50, vec);
    other = func_00154E10(obj, D_0029C67C * 0.017453292f, D_0029C680);
    obj->x98 = other;
    if (other != 0) {
        obj->x9C = other->x792C;
    } else {
        obj->x9C = func_00154F40(obj, D_0029C67C * 0.017453292f, D_0029C680);
    }
    func_00154B90((ObjB90 *)obj);
}

void func_00154B90(ObjB90 *o) {
    Vec delta, pos, dir, tmp;
    Rider *rider, *self;
    float distance, time, speed;
    self=o->x0;
    if (o->x9C) {
        COPY_VECTOR(pos,&o->x9C->x10);
        if (o->x98) Sub(&dir,&o->x9C->x10,&o->x28->x10);
    } else {
        COPY_VECTOR(pos,&self->x792C->x10);
        ScaleAdd(&pos,&pos,&self->x792C->x50,50.0f);
    }
    Sub(&delta,&pos,&o->x28->x10);
    distance=Length(&delta);
    rider=o->x98;
    if (rider) {
        time=distance/o->x94;
        speed=Length(&rider->x4180);
        speed=(speed/(float)func_00238F30())*time;
        Normalize(&dir,&rider->x4180);
        Scale(&tmp,&dir,speed);
        Add(&pos,&pos,&tmp);
    }
    COPY_VECTOR(dir,&pos);
    dir.z+=distance*D_0029C698;
    Sub(&delta,&dir,&o->x28->x10);
    Normalize(&tmp,&delta);
    Scale(&o->x30,&tmp,o->x94);
    o->x90=func_00238F48();
}
