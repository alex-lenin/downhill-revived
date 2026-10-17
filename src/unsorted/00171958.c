/* UNIT 0x00171958 0x00171A18 */
typedef struct { int x0, x4, x8, xC; } Handle;
typedef struct { int x0; char pad4[0x7C]; Handle x80, x90; char padA0[0x169]; char x209; } Obj;
extern void func_00203CB8(Handle *);
extern void func_00171E58(Obj *);
void func_00171958(Obj *obj, int state) {
    Handle h;
    if (obj->x0 != state) {
        if (obj->x80.x0) {
            h = obj->x80;
            func_00203CB8(&h);
        }
        if (obj->x90.x0) {
            h = obj->x90;
            func_00203CB8(&h);
        }
    }
    obj->x0 = state;
    if (state == 1) {
        func_00171E58(obj);
        obj->x209 = state;
        return;
    }
    obj->x209 = 0;
}
