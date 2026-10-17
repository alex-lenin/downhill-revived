/* UNIT 0x0010AFE0 0x0010B0E8 */
#include "vector.h"
typedef Vector4 Vec;

typedef struct Rider {
    char pad0[0x40C0];
    char x40C0[0x792C - 0x40C0];
    char *x792C;
} Rider;

typedef struct Obj {
    Rider *x0;
    char pad4[0x30 - 0x4];
    Vec x30;
    char pad40[0x70 - 0x40];
    int x70;
    int x74;
    int x78;
    char pad7C[0x1E0 - 0x7C];
    Vec x1E0;
} Obj;

extern char D_0029EA20[];
extern int func_00119790(void *, void *);
extern void func_0010B2B8(Obj *obj, int item);
extern float func_00222610(void *, void *);
extern void func_00221F50(Vec *m, Vec *rot);
extern void func_00231A78(void *, void *);
extern void func_00231A08(void *, void *);

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

void func_0010AFE0(Obj *obj, void *pos, void *mtxIn, int useMtx)
{
    Vec rot;
    Vec mtx[4];
    float angle;

    obj->x78 = 0;
    obj->x70 = 0;
    obj->x74 = func_00119790(D_0029EA20, pos);
    func_0010B2B8(obj, obj->x74);
    func_0010B2B8(obj, 0);
    if (useMtx) {
        func_00231A78(obj->x0->x40C0, mtxIn);
    } else {
        angle = func_00222610(obj->x0->x792C + 0x10, &obj->x30);
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = angle;
        Copy((Vec *)(obj->x0->x792C + 0xA0), &rot);
        func_00221F50(mtx, &rot);
        func_00231A78(obj->x0->x40C0, mtx);
    }
    func_00231A08(obj->x0->x40C0, pos);
    Set(&obj->x1E0, 0.0f, 1.0f, 0.0f);
}
