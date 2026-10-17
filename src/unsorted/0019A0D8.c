/* UNIT 0x0019A0D8 0x0019A138 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Camera {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x30];
    Vec4 x50;
} Camera;

typedef struct State {
    char pad0[0x792C];
    Camera *x792C;
} State;

typedef struct Obj {
    char pad0[0x28];
    State *x28;
} Obj;

extern Vec4 D_0043C820;
extern float func_00222640(void *);

void func_0019A0D8(Obj *obj) {
    Vec4 *src = &obj->x28->x792C->x10;

    /* 128-bit vector copy (libvu0-style inline asm, as in func_0016F6E0) */
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(D_0043C820) : "r"(src) : "$2");
    D_0043C820.w = func_00222640(&obj->x28->x792C->x50) * 57.29578f;
}
