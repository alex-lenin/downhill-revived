/* UNIT 0x001980B0 0x001980F0 */
typedef struct Obj { char pad0[0x504]; void *x504; } Obj;
extern void func_002182A8(void *, int);
extern void func_0018E468(Obj *);
void func_001980B0(Obj *obj) {
    if (obj->x504 != 0) func_002182A8(obj->x504, 1);
    func_0018E468(obj);
}
