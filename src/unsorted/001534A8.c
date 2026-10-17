/* UNIT 0x001534A8 0x001534E0 */
typedef struct Obj { char *x0; char pad4[0x10]; int x14; } Obj;
extern void func_001524B8(Obj *);
extern void func_001AC2E8(void *, int);
void func_001534A8(Obj *obj) {
    func_001524B8(obj);
    obj->x14 = 0;
    func_001AC2E8(obj->x0 + 0x1E40, 0);
}
