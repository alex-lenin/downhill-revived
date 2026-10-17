/* UNIT 0x0023BC10 0x0023BC60 */
typedef struct { char pad0[0x10]; int x10, x14; char pad18[0x28]; void *x40; } Obj;
extern void func_0025A720(void *);
extern void func_0025A700(void *);
int func_0023BC10(Obj *obj) {
    int size;
    func_0025A720(obj->x40);
    size = (obj->x10 << 11) + obj->x14;
    func_0025A700(obj->x40);
    return size;
}
