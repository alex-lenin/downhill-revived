/* UNIT 0x00190CE8 0x00190D60 */
struct Entry { int x0, x4, x8; };
struct Obj { char pad0[4]; struct Entry x4[14]; char padAC[8]; char xB4[4]; };
extern void *func_002205A0(int);
extern void func_0021F880(void *);
void func_00190CE8(struct Obj *obj) {
    int i, j;
    for (j = 13; j >= 0; j--) obj->x4[j].x0 = 0;
    for (i = 0; i < 4; i++) {
        func_0021F880(func_002205A0(i));
        obj->xB4[i] = 0;
    }
}
