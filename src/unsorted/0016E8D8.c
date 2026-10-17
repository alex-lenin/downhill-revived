/* UNIT 0x0016E8D8 0x0016E998 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Matrix {
    Vec4 row[4];
} Matrix;

typedef struct Transform {
    char pad0[0x10];
    Vec4 x10;
} Transform;

/* gcc 2.95 vtable entry: this-adjustment, then function */
typedef struct VEntry {
    short delta;
    short pad2;
    Transform *(*func)(void *);
} VEntry;

typedef struct VTable {
    char pad0[0x20];
    VEntry getTransform;
} VTable;

typedef struct Part {
    VTable *vtbl;
} Part;

typedef struct Obj {
    char pad0[0x130];
    Part *x130;
    char pad134[0x1D0 - 0x134];
    Vec4 x1D0;
} Obj;

extern void func_002226C8(Matrix *out, void *src);
extern void func_001269B8(Obj *obj);

static inline void MatApply(Vec4 *out, Matrix *m, Vec4 *v)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)
	"
                      "lqc2 $vf12, 0x0(%2)
	"
                      "lqc2 $vf13, 0x10(%2)
	"
                      "lqc2 $vf14, 0x20(%2)
	"
                      "vmulax.xyz ACC, $vf12, $vf11x
	"
                      "vmadday.xyz ACC, $vf13, $vf11y
	"
                      "vmaddz.xyz $vf11, $vf14, $vf11z
	"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "r"(m));
}

static inline void VecAdd(Vec4 *out, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)
	"
                      "lqc2 $vf12, 0x0(%2)
	"
                      "vadd.xyz $vf11, $vf11, $vf12
	"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

/* copy xyz, keep dst->w */
static inline void VecCopy3(Vec4 *dst, Vec4 *src)
{
    float w;

    w = dst->w;
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*dst) : "r"(src) : "$2");
    dst->w = w;
}

void func_0016E8D8(void *self, Obj *obj, void *rot)
{
    Matrix mtx;
    Vec4 offset;
    Vec4 pos;
    Vec4 *dir;
    Vec4 *base;
    Part *part;

    dir = &offset;
    offset.x = 10.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    func_002226C8(&mtx, rot);
    MatApply(&pos, &mtx, dir);
    part = obj->x130;
    if (part != 0) {
        base = &part->vtbl->getTransform.func((char *)part + part->vtbl->getTransform.delta)->x10;
    } else {
        base = 0;
    }
    VecAdd(&pos, base, &pos);
    VecCopy3(&obj->x1D0, &pos);
    func_001269B8(obj);
}
