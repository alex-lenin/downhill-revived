/* UNIT 0x00144E90 0x00145098 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Point { Vec pos; short x10; char pad12[4]; short x16; } Point;
typedef struct Pose { char pad0[0x10]; u128 x10; char pad20[0x30]; u128 x50; char pad60[0x40]; Vec xA0; } Pose;
typedef struct Config { char pad0[0x1544]; float x1544; char pad1548[8]; float x1550; } Config;
typedef struct Track { u128 xA0, xB0, xC0, xD0; float xE0; char padE4[0x10]; float xF4; char padF8[0x18]; } Track;
typedef struct State { void *x0; Point *x4, *x8, *xC; char pad10[0x20]; Vec dir; Vec x40; char pad50[0x30]; float x80; } State;
typedef struct Obj {
    char pad0[8];
    Pose *x8;
    Config *xC;
    char pad10[0x30];
    Track track;
    State state;
    char pad134[0x5C];
    float x190, x194;
} Obj;

extern Point *func_00163DA0(void *, Point *, int);
extern void func_001DDBA0(State *);
extern void func_001450A0(Obj *, int);
extern void func_00222578(Vec *, Point *, Point *);
extern void func_00221F50(void *, Vec *);
extern void func_001DD7E8(Track *, Vec *, Vec *, Point *, Point *, float);
extern void func_00145E08(Obj *, float);
extern void func_001DCE20(Track *);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline float Normalize(Vec *out, Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=f"(len) : "f"(x), "f"(y), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

static inline float maxf(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline void SetNextPoint(Obj *obj, State *state)
{
    Point *next;
    void *list;

    next = state->x8;
    list = obj->state.x0;
    if (next->x16 <= 0) {
        state->xC = next;
    } else {
        state->xC = func_00163DA0(list, next, 0);
    }
}

void func_00144E90(Obj *obj, Point *point)
{
    if (obj->x8) {
        State *state;
        void *p;
        void *list;
        Vec delta;
        u128 zero;
        Track *track;

        p = &obj->state;
        ((State *)p)->x4 = point;
        state = p;
        list = obj->state.x0;
        p = &point->x16;
        if (*(short *)p <= 0) {
            state->x8 = point;
            func_001DDBA0(state);
        } else {
            state->x8 = func_00163DA0(list, point, 0);
            func_001DDBA0(state);
        }
        SetNextPoint(obj, state);
        func_00222578(&obj->x8->xA0, state->x4, state->x8);
        func_00221F50((char *)obj->x8 + 0x40, &obj->x8->xA0);
        func_001450A0(obj, 0);
        state->x80 = 0.3333f;
        VecSub(&delta, &state->x8->pos, &state->x4->pos);
        state->dir.w = Normalize(&obj->state.dir, &delta);
        track = &obj->track;
        func_001DD7E8(track, &obj->state.dir, &obj->state.x40, state->x8, state->xC, state->x8->x10);
        __asm__ volatile ("sqc2 $vf0, %0" : "=m"(zero));
        COPY_VECTOR(obj->track.xA0, state->x4);
        COPY_VECTOR(obj->track.xB0, &zero);
        COPY_VECTOR(obj->track.xC0, &obj->x8->x50);
        COPY_VECTOR(obj->track.xD0, &zero);
        track->xE0 = 1.0f;
        obj->x190 = obj->xC->x1544;
        obj->x194 = obj->xC->x1544;
        func_00145E08(obj, obj->x194);
        track->xF4 = maxf(1.0f / 60.0f, obj->xC->x1550);
        func_001DCE20(track);
        func_001DCE20(track);
        COPY_VECTOR(obj->x8->x10, &track->xA0);
    }
}
