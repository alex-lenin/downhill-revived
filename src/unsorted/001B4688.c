/* UNIT 0x001B4688 0x001B4718 */
typedef struct Global {
    char pad0[0x288];
    int x288;
} Global;

typedef struct Obj {
    int x0;
    int x4;
    int x8;
} Obj;

extern Global D_004B25A0;

extern void func_00220018(int index, int a, int b, int c);

int func_001B4688(Obj *obj)
{
    int i;

    if (obj->x4 == 0) {
        return 0;
    }
    if (--obj->x8 < 0) {
        for (i = 0; i < D_004B25A0.x288; i++) {
            func_00220018(i, 0, 0, 0);
        }
        return 1;
    }
    return 0;
}
