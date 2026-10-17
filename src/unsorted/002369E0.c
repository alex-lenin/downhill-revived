/* UNIT 0x002369E0 0x00236A30 */
typedef struct { int x0, x4; } Pair;
typedef struct { Pair x0, x8; } Obj;
extern void func_00263BA8(Pair *);
Obj *func_002369E0(Obj *obj) {
    Pair *p;
    func_00263BA8(&obj->x0);
    p = &obj->x8;
    func_00263BA8(p);
    obj->x0.x4 = 0;
    obj->x0.x0 = 0;
    p->x4 = 0;
    p->x0 = 0;
    return obj;
}

