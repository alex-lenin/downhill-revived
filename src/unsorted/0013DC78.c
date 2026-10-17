/* UNIT 0x0013DC78 0x0013DCF8 */
struct Obj { char pad0[0x418]; int x418; char pad41c[0xf4]; int x510; int x514[64]; int x614[64]; char pad714[0x5cc]; int xce0; };
extern int func_001FA300(int, int);
extern int func_001FA370(int, int);
extern void func_0018E408(struct Obj *, int, int);
void func_0013DC78(struct Obj *obj, int value, int arg) {
    int index = obj->x418;
    obj->x614[index] = value;
    obj->x514[index] = func_001FA300(obj->x510, value);
    func_0018E408(obj, func_001FA370(obj->x510, value), arg);
    obj->xce0 = 0;
}
