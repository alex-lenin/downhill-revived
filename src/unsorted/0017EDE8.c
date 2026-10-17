/* UNIT 0x0017EDE8 0x0017EEF8 */
typedef struct Obj {
    char pad0[0x194];
    int x194[25];
    int x1F8[25];
    int x25C[25];
    int x2C0[25];
    int x324[57];
    char pad408;
    unsigned char x409[25];
} Obj;

extern int func_00238F60(void);

void func_0017EDE8(Obj *obj, int i)
{
    if (obj->x194[i] != 0) {
        if (obj->x25C[i] == 0 && obj->x1F8[i] == 0 && obj->x2C0[i] < 30 && obj->x409[i] == 0) {
            obj->x25C[i] = 1;
        }
        obj->x2C0[i] = 0;
        if (obj->x324[i] > 60) {
            obj->x409[i] = 1;
        } else {
            obj->x409[i] = 0;
        }
    } else {
        obj->x2C0[i] += func_00238F60();
        if (obj->x409[i] && obj->x2C0[i] > 30) {
            obj->x409[i] = 0;
        }
        obj->x25C[i] = 0;
    }
}
