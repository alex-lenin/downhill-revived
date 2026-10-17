/* UNIT 0x00170850 0x00170BC8 .sdata=0x29C8B8:0x29C8C0 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Matrix { Vec r[4]; } Matrix;
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
/* 64-byte matrix copy (as sceVu0CopyMatrix) */
#define COPY_MATRIX(dst, src) __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\tsq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
    : "=m"((dst).r[0]), "=m"((dst).r[1]), "=m"((dst).r[2]), "=m"((dst).r[3]) : "r"(src) : "$8", "$9", "$10", "$11")

struct Vtable { char pad0[0x20]; short delta; short pad22; int (*call)(); };
/* the vtable pointer is at +4; slot at +0x20 is {delta, function} */
struct Mode { int x0; struct Vtable *vt; };
#define VCALL(o) ((o)->vt->call((char *)(o) + (o)->vt->delta))

struct Body { char pad0[0x10]; Vec x10; char pad20[0x20]; Matrix x40; };
struct Target { char pad0[0x10]; Vec x10; Vec x20; };
struct Sub { char pad0[0x16D4]; struct Body *x16D4; };
struct Game {
    char pad0[0x1E40];
    struct Sub x1E40;
    char pad1E40[0x40C0 - 0x1E40 - sizeof(struct Sub)];
    char x40C0[0x4ED0 - 0x40C0];
    char x4ED0[0x5E40 - 0x4ED0];
    struct Mode x5E40;
    char pad5E48[0x62E0 - 0x5E48];
    struct Mode x62E0;
    char pad62E8[0x792C - 0x62E8];
    struct Body *x792C;
    char pad7930[0x7BA4 - 0x7930];
    struct Target *x7BA4;
};
struct Cam { struct Game *x0; char pad4[0xC]; Vec x10; char pad20[0x10]; Vec x30; Vec x40; };

float D_0029C8B8 = 0.1f;
float D_0029C8BC = 0.0f;

extern void func_001AC578(void *);
extern void func_0012FE38(struct Game *, void *);
extern void func_00135708(void *, Vec *);
extern void func_00231A08(void *, Vec *);
extern void func_00222F60(Vec *, Vec *, Vec *);
extern void func_00223168(Vec *, Vec *);
extern void func_002235B0(Vec *, Vec *, Vec *);
extern void func_00170C28(struct Cam *, Matrix *, Vec *, Vec *);
extern void func_00231A78(void *, Matrix *);
extern void func_00170BC8(struct Cam *);

static inline void VAdd(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VSub(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VScale(Vec *dst, Vec *a, float f)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a), "f"(f) : "$2");
}

static inline void VMadd(Vec *dst, Vec *a, Vec *b, float f)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a), "r"(b), "f"(f) : "$2");
}

static inline float Dot(Vec *a, Vec *b)
{
    float ax = a->x, bx = b->x, ay = a->y, by = b->y, az = a->z, bz = b->z, result;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(result) : "f"(ax), "f"(bx), "f"(ay), "f"(by), "f"(az), "f"(bz));
    return result;
}

static inline float SquaredLength(Vec *v)
{
    float result;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(result) : "f"(v->x), "f"(v->y), "f"(v->z));
    return result;
}

void func_00170850(struct Cam *cam)
{
    Vec diff, aim, pos, target, dir, proj;
    Matrix m;
    Vec a, b, q;
    struct Game *game;
    struct Sub *sub;
    struct Target *t;
    float k;
    struct Mode *mode;

    game = cam->x0;
    sub = &game->x1E40;
    t = game->x7BA4;
    func_001AC578(sub);
    if (t == 0) {
        return;
    }
    mode = &game->x62E0;
    if (VCALL(mode) != 0 || (mode = &game->x5E40, VCALL(mode) != 0)) {
        func_0012FE38(game, mode);
    } else if (game->x7BA4 == 0) {
        func_0012FE38(game, game->x4ED0);
    }
    func_00135708(game->x40C0, &pos);
    COPY_VECTOR(target, &t->x20);
    target.z += D_0029C8BC;
    VSub(&diff, &pos, &target);
    VScale(&dir, &t->x10, 1000.0f);
    k = Dot(&dir, &diff) / SquaredLength(&dir);
    VScale(&proj, &dir, k);
    VAdd(&aim, &target, &proj);
    VSub(&cam->x10, &aim, &pos);
    VMadd(&game->x792C->x10, &game->x792C->x10, &cam->x10, D_0029C8B8);
    VMadd(&sub->x16D4->x10, &sub->x16D4->x10, &cam->x10, D_0029C8B8);
    func_00231A08(game->x40C0, &game->x792C->x10);
    COPY_VECTOR(a, &game->x792C->x40.r[1]);
    COPY_VECTOR(b, &game->x792C->x40.r[2]);
    func_00222F60(&q, &cam->x30, &a);
    if (q.w < 0.999f) {
        q.w = 0.999f;
        func_00223168(&q, &q);
        func_002235B0(&a, &q, &a);
    } else {
        COPY_VECTOR(a, &cam->x30);
    }
    func_00222F60(&q, &cam->x40, &b);
    if (q.w < 0.999f) {
        q.w = 0.999f;
        func_00223168(&q, &q);
        func_002235B0(&b, &q, &b);
    } else {
        COPY_VECTOR(b, &cam->x40);
    }
    func_00170C28(cam, &m, &a, &b);
    COPY_MATRIX(game->x792C->x40, &m);
    COPY_MATRIX(sub->x16D4->x40, &m);
    func_00231A78(game->x40C0, &m);
    func_00170BC8(cam);
}
