/* UNIT 0x001AF0C0 0x001AF0F8 */
typedef struct Obj { int x0; char pad4[0x14]; int x18; } Obj;
extern void func_001AF0F8(Obj *);
void func_001AF0C0(Obj *obj) {
    if (obj->x18 != 0 && obj->x0 == 0) func_001AF0F8(obj);
}
