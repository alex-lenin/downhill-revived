/* UNIT 0x001442D0 0x00144520 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Node { Vec pos; short x10; char pad12[4]; short x16; } Node;
typedef struct Body { unsigned *x0; char pad4[12]; Vec x10; Vec pad20[2]; Vec x40, x50; char pad60[0x40]; Vec xA0; } Body;
typedef struct Rider { char pad0[0x78C]; float x78C, x790, x794, x798, x79C; } Rider;
typedef struct State { int x0; Node *x4, *x8, *xC; char pad10[0x2C]; float x3C; char pad40[0x40]; float x80; } State;
typedef struct Cam { Vec x0, x10, x20, x30; float x40, x44; char pad48[0xC]; float x54; char pad58[0x18]; } Cam;
typedef struct Obj { Body *x0; Rider *x4; char pad8[8]; Cam cam; State x80; Vec xB0_unused; } Obj;

extern Node *func_00163DA0(int, Node *, int);
extern void func_001DDBA0(State *);
extern void func_00222578(Vec *, Node *, Node *);
extern void func_00221F50(Vec *, Vec *);
extern void func_00221F20(Vec *);
extern void func_001DD7E8(Cam *, Vec *, Vec *, Node *, Node *, float);
extern void func_001DCE20(Cam *);
extern float func_00222B68(float, float);

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

static inline float Max(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

void func_001442D0(Obj *obj, Node *node)
{
    Vec delta, zero;
    Vec *dir;
    State *state;
    Cam *cam;
    float speed;
    int id;
    int t; /* scratch: first &obj->x80, then node->x16 */

    if (obj->x0) {
        t = (int)&obj->x80;
        ((State *)t)->x4 = node;
        state = (State *)t;
        id = obj->x80.x0;
        t = node->x16;
        if (t <= 0) {
            state->x8 = node;
            func_001DDBA0(state);
        } else {
            state->x8 = func_00163DA0(id, node, 0);
            func_001DDBA0(state);
        }
        node = state->x8;
        id = obj->x80.x0;
        t = node->x16;
        if (t <= 0) {
            state->xC = node;
        } else {
            state->xC = func_00163DA0(id, node, 0);
        }
        func_00222578(&obj->x0->xA0, state->x4, state->x8);
        func_00221F50(&obj->x0->x40, &obj->x0->xA0);
        state->x80 = 0.3333f;
        VecSub(&delta, &state->x8->pos, &state->x4->pos);
        dir = (Vec *)((char *)obj + 0xB0);
        state->x3C = Normalize(dir, &delta);
        func_001DD7E8(&obj->cam, dir, (Vec *)((char *)obj + 0xC0), state->x8, state->xC, state->x8->x10);
        cam = &obj->cam;
        __asm__ volatile ("sqc2 $vf0, %0" : "=m"(zero));
        COPY_VECTOR(obj->cam.x0, state->x4);
        COPY_VECTOR(obj->cam.x10, &zero);
        COPY_VECTOR(obj->cam.x20, &obj->x0->x50);
        if ((*obj->x0->x0 & 0xFFFC0000) == 0x13EC0000)
            func_00221F20(&obj->x0->x40);
        COPY_VECTOR(obj->cam.x30, &zero);
        cam->x40 = 1.0f;
        speed = func_00222B68(obj->x4->x78C, obj->x4->x790) * 40.909092f * 1.4666667f * 1.5f;
        cam->x44 = Max(0.0f, speed);
        cam->x54 = Max(0.016666668f, obj->x4->x79C);
        func_001DCE20(cam);
        func_001DCE20(cam);
        COPY_VECTOR(obj->x0->x10, cam);
    }
}
