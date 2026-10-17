/* UNIT 0x001A9508 0x001A9568 */
typedef struct Obj { char pad[0x28]; int x28; char pad2[0x10]; int x3C; float x40; } Obj;
typedef struct Global { char pad[0x360]; int x360; } Global;
extern Global D_0037EA60;
extern void func_0023D898(int, float *, float *);
extern void func_0023D8D8(int, float, float);
extern void func_00148560(Global *, int);
void func_001A9508(Obj *obj) {
    float a, b;
    func_0023D898(0, &a, &b);
    func_0023D8D8(0, obj->x40, b);
    D_0037EA60.x360 = obj->x3C;
    func_00148560(&D_0037EA60, 1);
    obj->x28 = 0;
}
