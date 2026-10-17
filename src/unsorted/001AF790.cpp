/* UNIT 0x001AF790 0x001AF8B0 */
#include "camera.h"
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

static inline float InnerProduct(Vec4 *a, Vec4 *b) {
    float result;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(result) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return result;
}

struct Board {
    char pad0[0x1E40];
    char x1E40[0x4180 - 0x1E40];
    Vec4 x4180;
    char pad4190[0x7984 - 0x4190];
    int x7984;
    int x7988;
};

struct Entry {
    char pad0[0x20];
    Vec4 x20;
    char pad30[0x380 - 0x30];

    float Dot(Vec4 *v) { return InnerProduct(&x20, v); }
};

struct Obj {
    float x0;
    float x4;
    Board *x8;
};

extern "C" Entry D_0037EA60[];
extern "C" float func_00236088(float cur, float target, float rate, float min);


/* max.s/min.s come from g++'s >? and <? operators. */
extern "C" void func_001AF790(Obj *obj) {
    Board *board;
    char *sub;
    float dot;
    float delta;

    board = obj->x8;
    if (board == 0) {
        return;
    }
    sub = board->x1E40;
    if (board->x7984 != 0) {
        dot = D_0037EA60[board->x7988].Dot(&board->x4180);
    } else {
        dot = D_0037EA60[0].Dot(&board->x4180);
    }
    delta = dot - obj->x4;
    delta = (delta >? -2.0f) <? 2.0f;
    obj->x0 = func_00236088(obj->x0, delta, 0.01f, 0.005f);
    func_001B3028((RiderCamera *)(sub + 0x16F0), 8, obj->x0);
    obj->x4 = dot;
}
