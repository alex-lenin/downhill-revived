/* UNIT 0x001F8E30 0x001F8E90 */
typedef struct Obj { char pad[0x4A8]; int x4A8; } Obj;
typedef struct State { char pad[0x10C]; int x10C; } State;
extern State D_003F52D0;
extern void func_00190358(State *);
extern void func_0018E098(Obj *), func_001F8F50(Obj *), func_0018DCE8(Obj *), func_0018E1C0(Obj *);
void func_001F8E30(Obj *p) {
    p->x4A8 = 5;
    D_003F52D0.x10C = 15;
    func_00190358(&D_003F52D0);
    func_0018E098(p);
    func_001F8F50(p);
    func_0018DCE8(p);
    func_0018E1C0(p);
}
