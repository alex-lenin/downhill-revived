/* UNIT 0x001458E0 0x00145920 */
typedef struct Obj { char pad0[0x1A8]; int x1A8; int x1AC; int x1B0; } Obj;
extern void func_001457A8(Obj *);
extern void func_001450A0(Obj *, int);
void func_001458E0(Obj *obj) {
    func_001457A8(obj);
    if (obj->x1A8 > obj->x1B0) func_001450A0(obj, 2);
}


