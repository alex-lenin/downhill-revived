/* UNIT 0x001657F8 0x00165850 .sdata=0x29C7EC:0x29C7F0 */
struct Obj { char pad[0x190]; int x190; int x194; int x198; char pad19C[0x274]; char x410[16]; };
int D_0029C7EC = 30;
extern void func_00202E38(void *, int, int, float);
void func_001657F8(struct Obj *obj) {
    int count = obj->x198 + 1;
    obj->x198 = count;
    if (D_0029C7EC < count) {
        obj->x198 = -1;
        obj->x190 = 1;
        func_00202E38(obj->x410, 1, 1, 200.0f);
    }
}
