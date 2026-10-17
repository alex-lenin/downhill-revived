/* UNIT 0x00171E58 0x00171F10 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec x10;
    char pad20[0x30];
    Vec x50;
    char pad60[0x40];
    char xA0[0x10];
} Body;

typedef struct Game {
    char pad0[0x29AC];
    int x29AC;
    int x29B0;
} Game;

typedef struct Obj {
    char pad0[0x4];
    Body *x4;
    Game *x8;
    char padC[0x4];
    Vec x10;
    Vec x20;
    Vec x30;
    char pad40[0x10];
    Vec x50;
    Vec x60;
    Vec x70;
    char pad80[0x1F8 - 0x80];
    int x1F8;
    int x1FC;
} Obj;

extern void func_002225A8(void *out, Vec *dir);
extern int func_00222B08(int, int);

static inline void Cross(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vopmula.xyz ACC, $vf11, $vf12\n\t"
                      "vopmsub.xyz $vf11, $vf12, $vf11\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Copy(Vec *dst, Vec *src)
{
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(*dst) : "r"(src) : "$2");
}

static inline void Set(Vec *v, float x, float y, float z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

void func_00171E58(Obj *obj)
{
    Copy(&obj->x20, &obj->x4->x50);
    Set(&obj->x30, 0.0f, 0.0f, 1.0f);
    Cross(&obj->x10, &obj->x20, &obj->x30);
    Cross(&obj->x30, &obj->x10, &obj->x20);
    func_002225A8(obj->x4->xA0, &obj->x20);
    Copy(&obj->x50, &obj->x4->x10);
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x60));
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x70));
    obj->x1F8 = 0;
    obj->x1FC = func_00222B08(obj->x8->x29AC, obj->x8->x29B0);
}
