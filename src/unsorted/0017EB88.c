/* UNIT 0x0017EB88 0x0017EC90 */
typedef struct Obj {
    char pad0[0x4];
    void *x4[25][4];
    int x194[25];
} Obj;

extern int func_00220618(void *p, int value);

void func_0017EB88(Obj *obj, int index, int value)
{
    int i;

    if (obj->x194[index] != 0) {
        obj->x194[index] = 0;
        for (i = 0; i < 4 && obj->x194[index] == 0; i++) {
            if (obj->x4[index][i] != 0 && func_00220618(obj->x4[index][i], value)) {
                obj->x194[index] = 1;
            }
        }
    } else {
        obj->x194[index] = func_00220618(obj->x4[index][0], value);
        for (i = 1; i < 4 && obj->x194[index] != 0; i++) {
            if (obj->x4[index][i] != 0) {
                obj->x194[index] = func_00220618(obj->x4[index][i], value);
            }
        }
    }
}
