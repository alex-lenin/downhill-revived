/* UNIT 0x0017BA30 0x0017BA78 */
struct Obj { int x0; char pad4[0xc7c]; char *xc80; char padc84[0x5fc]; int x1280; };
extern char D_00513B00[];
extern void func_002103D8(void *, int, void *);
void func_0017BA30(struct Obj *obj) {
    obj->x0 = ((((int)(obj->xc80 - (char *)obj)) >> 4) - 1) | 0x60000000;
    func_002103D8(D_00513B00, obj->x1280, obj);
}

