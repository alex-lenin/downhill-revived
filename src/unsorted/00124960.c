/* UNIT 0x00124960 0x00124B40 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

/* collision filter pushed with func_00210940 */
typedef struct QueryFlags {
    unsigned int x0;
    union {
        unsigned int word;
        unsigned short x4;
        struct { unsigned int low : 16, a : 1, b : 1, c : 1, other : 13; } bits;
    } x4;
    int pad8[2];
} QueryFlags;

typedef struct Hit { unsigned short found; char pad2[14]; Vec pos, normal; } Hit;

typedef struct Obj {
    char pad0[0x24];
    int x24;
    int x28;
    char pad2C[0xC4];
    Vec xF0;
    char pad100[0x10];
    Vec plane;      /* normal xyz, distance w */
    Vec x120;
} Obj;

extern float D_0029C1CC[], D_0029C1D0[];
extern QueryFlags *func_00210940(QueryFlags *);
extern int func_00211BE0(Vec *, Hit *, float, float);
extern int func_00238F60(void);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Init(QueryFlags *f) { f->x0 = -1; f->x4.x4 = 0xffff; f->x4.bits.a = 0; f->x4.bits.b = 0; f->x4.bits.c = 0; }
static inline void Enable(QueryFlags *f, int value) { f->x0 = 0; f->x4.bits.a = value; }
static inline float Dot(Vec *a, Vec *b)
{
    float out;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(out) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return out;
}
static inline void Set(Vec *v, float x, float y, float z, float w) { v->x = x; v->y = y; v->z = z; v->w = w; }
static inline float Height(Vec *v, Vec *pos)
{
    if (v->z != 0.0f)
        return ((v->x * pos->x + v->y * pos->y) - v->w) / -v->z;
    return D_0029C1D0[0];
}
static inline void Query(Vec *pos, Hit *hit) { func_00211BE0(pos, hit, 16.0f, -48.0f); }

/* uses VU0/FPU inline asm */
float func_00124960(Obj *obj, Vec *pos, int force)
{
    Hit hit;
    QueryFlags flags, *old;
    float distance = D_0029C1CC[0], height;
    Vec *plane;

    if (force || obj->x28) {
        Init(&flags);
        Enable(&flags, 1);
        old = func_00210940(&flags);
        Query(pos, &hit);
        func_00210940(old);
        if (hit.found) {
            COPY_VECTOR(obj->plane, &hit.normal);
            obj->plane.w = Dot(&hit.normal, &hit.pos);
            distance = pos->z - hit.pos.z;
            COPY_VECTOR(obj->x120, &hit.pos);
            __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->xF0));
        }
        if (distance > 48.0f) {
            Set(&obj->plane, 0.0f, 0.0f, 1.0f, D_0029C1D0[0]);
            obj->x120 = *pos;
        }
        obj->x24 = 0;
    } else {
        plane = &obj->plane;
        height = pos->z;
        distance = height - Height(plane, pos);
        obj->x24 += func_00238F60();
    }
    return distance;
}
