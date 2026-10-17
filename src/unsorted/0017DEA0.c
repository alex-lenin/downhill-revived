/* UNIT 0x0017DEA0 0x0017DF00 */
typedef struct Obj {
    char pad0[0x194];
    int active[25];   /* 0x194 */
    char pad1F8[0x12C];
    int timer[25];    /* 0x324 */
} Obj;

extern int func_00238F60(void);

void func_0017DEA0(Obj *obj)
{
    int i;

    for (i = 0; i < 25; i++) {
        if (obj->active[i] != 0) {
            obj->timer[i] += func_00238F60();
        } else {
            obj->timer[i] = 0;
        }
    }
}
