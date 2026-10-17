/* UNIT 0x0016F198 0x0016F408 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Node {
    Vec pos;
    short x10;
    char pad12[4];
    short x16;
} Node;

typedef struct Body {
    unsigned long flags;
    char pad8[8];
    Vec x10;
    char pad20[0x20];
    char x40[0x10];
    Vec x50;
    char pad60[0x40];
    char xA0[0x10];
} Body;

typedef struct Params {
    char pad0[0x4470];
    float x4470;
    float x4474;
    char pad4478[8];
    float x4480;
} Params;

typedef struct Motion {
    Vec pos;
    Vec x10;
    Vec x20;
    Vec x30;
    float x40;
    char pad44[0x10];
    float x54;
    char pad58[0x18];
} Motion;

typedef struct Path {
    void *root;
    Node *start;
    Node *cur;
    Node *next;
    char pad10[0x20];
    Vec dir;
    Vec x40;
    char pad50[0x30];
    float x80;
} Path;

typedef struct Obj {
    char pad0[0x10];
    Motion motion;
    Path path;
    char pad110[0xE0];
    Body *body;
    char pad1F4[8];
    Params *params;
    char pad200[0x10];
    float x210;
    float x214;
} Obj;

extern Node *func_00163DA0(void *, Node *, int);
extern void func_001DDBA0(Path *);
extern void func_0016F410(Obj *, int);
extern void func_00222578(void *, Node *, Node *);
extern void func_00221F50(void *, void *);
extern void func_001DD7E8(Motion *, Vec *, Vec *, Node *, Node *, float);
extern float func_00222B68(float, float);
extern void func_00170228(Obj *, float);
extern void func_001DCE20(Motion *);
extern void func_00211280(Body *);
extern int func_00238F30(void);
extern void func_00209248(Body *, Vec *, void *, float);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline float VecNormalize(Vec *out, float x, float y, float z)
{
    float length, inv;
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=f"(length) : "f"(x), "f"(y), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return length;
}

static inline float maxf(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline Node *Resolve(void *root, Node *node)
{
    if (node->x16 <= 0)
        return node;
    return func_00163DA0(root, node, 0);
}

static inline void PathBegin(Path *path, Node *node)
{
    void *root;

    path->start = node;
    root = path->root;
    if (node->x16 <= 0) {
        path->cur = node;
        func_001DDBA0(path);
    } else {
        path->cur = func_00163DA0(root, node, 0);
        func_001DDBA0(path);
    }
}

void func_0016F198(Obj *obj, Node *node)
{
    Path *path;
    Motion *motion;
    Body *body;
    Params *params;
    Vec diff;
    Vec zero;
    void *root;

    if (obj->body) {
        PathBegin(&obj->path, node);
        path = &obj->path;
        root = obj->path.root;
        if (path->cur->x16 <= 0)
            path->next = path->cur;
        else
            path->next = func_00163DA0(root, path->cur, 0);
        func_00222578(obj->body->xA0, path->start, path->cur);
        func_00221F50(obj->body->x40, obj->body->xA0);
        func_0016F410(obj, 0);
        path->x80 = 0.3333f;
        VecSub(&diff, &path->cur->pos, &path->start->pos);
        path->dir.w = VecNormalize(&obj->path.dir, diff.x, diff.y, diff.z);
        motion = &obj->motion;
        func_001DD7E8(motion, &obj->path.dir, &obj->path.x40, path->cur, path->next, path->cur->x10);
        __asm__ volatile ("sqc2 $vf0, %0" : "=m"(zero));
        COPY_VECTOR(obj->motion.pos, &path->start->pos);
        COPY_VECTOR(obj->motion.x10, &zero);
        COPY_VECTOR(obj->motion.x20, &obj->body->x50);
        COPY_VECTOR(obj->motion.x30, &zero);
        motion->x40 = 1.0f;
        params = obj->params;
        obj->x210 = params->x4470;
        obj->x214 = func_00222B68(params->x4470, params->x4474);
        func_00170228(obj, obj->x214);
        motion->x54 = maxf(1.0f / 60.0f, obj->params->x4480);
        func_001DCE20(motion);
        func_001DCE20(motion);
        COPY_VECTOR(obj->body->x10, motion);
        obj->body->flags |= 1UL << 58;
        func_00211280(obj->body);
        body = obj->body;
        func_00209248(obj->body, &body->x10, body->x40, 1.0f / (float)func_00238F30());
    }
}
