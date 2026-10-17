/* UNIT 0x0013F4D8 0x0013F600 */
struct Obj {
    char pad0[0x18];
    char x18[0x400];
    int count;
    int x41C;
    char pad420[0x5C];
    int x47C;
    char pad480[0x3B];
    char x4BB;
    char pad4BC[0x44];
    int values[128];
    signed char ids[128];
};
extern char D_00361700[][0x40];
extern char D_0029C508[];
extern char D_0029C510[];
extern int D_0029DD30;
extern void func_00254D68(char *, char *, ...);
extern void func_0018D8A8(void *, char *);
extern void func_0018E5A8(struct Obj *);

void func_0013F4D8(struct Obj *obj)
{
    int count;
    int i;

    count = obj->count;
    for (i = 0; i < count; i++) {
        if (obj->values[i] > 0) {
            func_00254D68(D_00361700[i], D_0029C508, obj->values[i]);
        } else {
            func_00254D68(D_00361700[i], D_0029C510,
                          *(char **)(((obj->ids[i] + 0x26) << 2) + D_0029DD30));
        }
        func_0018D8A8(obj->x18 + i * 16, D_00361700[i]);
    }
    obj->x4BB = 1;
    obj->x47C = 1;
    obj->x41C = 12;
    func_0018E5A8(obj);
}
