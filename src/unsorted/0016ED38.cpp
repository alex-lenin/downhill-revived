/* UNIT 0x0016ED38 0x0016EE08 */
class Base { public: virtual void v1(); virtual void v2(); virtual void v3(); virtual unsigned long *v4(); };
struct Part { char pad0[8]; int x8; char padC[0x124]; Base *x130; char pad134[0xfc]; };
struct Obj { char pad0[0x250]; Part x250; Part x480; char pad6B0[12]; char x6BC; char x6BD; };
extern "C" void func_00126B20(Part *, int);
void func_0016ED38(Obj *obj) {
    Part *part = &obj->x250;
    obj->x6BC = 0;
    obj->x6BD = 0;
    if (part->x8 != 1) {
        *part->x130->v4() |= (unsigned long)0x8000 << 42;
        func_00126B20(part, 1);
    }
    part = &obj->x480;
    if (part->x8 != 1) {
        *part->x130->v4() |= (unsigned long)0x8000 << 42;
        func_00126B20(part, 1);
    }
}
