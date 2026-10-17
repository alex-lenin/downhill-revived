/* UNIT 0x0017EC90 0x0017ED50 */
typedef struct Obj {
    int x0;
    void *x4[25][4];
    int x194[25];
} Obj;

extern int func_00220618(void *p, int arg);

int func_0017EC90(Obj *obj, int idx, int arg)
{
    int ok;
    int i;

    i = 1;
    ok = func_00220618(obj->x4[idx][0], arg) != 0;
    for (; i < 4 && ok; i++) {
        if (obj->x4[idx][i]) {
            obj->x194[idx] = func_00220618(obj->x4[idx][i], arg);
        }
    }
    return ok;
}
