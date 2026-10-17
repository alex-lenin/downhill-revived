/* UNIT 0x0010EC80 0x0010ED50 */
#include "vector.h"
typedef Vector4 Vec;

typedef struct Global {
    char pad0[0x501B4];
    int x501B4;
} Global;

typedef struct Frame {
    char pad0[0x10];
    Vec x10;
    Vec x20;
} Frame;

typedef struct Obj {
    char *x0;
    char pad4[0x1FC];
    Vec x200;
    Vec x210;
    char pad220[0x9C];
    float x2BC;
} Obj;

extern Global D_002AF2C0;
extern int func_00138560(char *p);
extern Frame *func_00138558(char *p);
extern float func_00238F38(void);
extern void func_0010ED50(Obj *obj);

/* quadword copy, as the game's vector-copy macro (sceVu0CopyVector style) */
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_0010EC80(Obj *obj) {
    float t;

    if (func_00138560(obj->x0 + 0x40C0) & 0xFF) {
        /* t = max(x2BC - 1 / rate, 0) */
        __asm__ ("max.s %0, %1, %2" : "=f"(t) : "f"(obj->x2BC - 1.0f / func_00238F38()), "f"(0.0f));
        obj->x2BC = t;
        if (*(int *)(obj->x0 + 0x797C) == D_002AF2C0.x501B4) {
            func_0010ED50(obj);
        }
    } else {
        obj->x2BC = 0.001f;
        COPY_VECTOR(obj->x200, &func_00138558(obj->x0 + 0x40C0)->x20);
        COPY_VECTOR(obj->x210, &func_00138558(obj->x0 + 0x40C0)->x10);
    }
}
