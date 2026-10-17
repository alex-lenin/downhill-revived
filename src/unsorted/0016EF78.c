/* UNIT 0x0016EF78 0x0016EFF8 */
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Slot {
    int x0;
    char pad4[0x1C];
} Slot;

typedef struct Obj {
    char pad0[0x150];
    Slot x150[4];
    Vec4 x1D0;
    Vec4 x1E0;
    int x1F0[6];
    char pad208[8];
    int x210[9];
    unsigned char x234;
    unsigned char x235;
} Obj;

void func_0016EF78(Obj *obj)
{
    int i;

    for (i = 3; i >= 0; i--) {
        obj->x150[i].x0 = 0;
    }
    /* $vf0 is the constant (0, 0, 0, 1) */
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x1D0));
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x1E0));
    obj->x1D0.z = 1.0f;
    obj->x1F0[0] = 0;
    obj->x1F0[1] = 0;
    obj->x1F0[2] = 0;
    obj->x1F0[3] = 0;
    obj->x1F0[4] = 0;
    obj->x1F0[5] = 0;
    obj->x210[0] = 0;
    obj->x210[1] = 0;
    obj->x210[2] = 0;
    obj->x210[3] = 0;
    obj->x210[4] = 0;
    obj->x210[5] = 0;
    obj->x210[6] = 0;
    obj->x210[7] = 0;
    obj->x210[8] = 0;
    obj->x234 = 0;
    obj->x235 = 0;
}
