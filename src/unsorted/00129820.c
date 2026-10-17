/* UNIT 0x00129820 0x00129860 */
typedef struct Obj { char pad0[4]; int **x4; char pad8[0x188]; int x190; } Obj;
void func_00129820(Obj *obj) {
    int **ptr = obj->x4;
    if (ptr) {
        int *flags = *ptr;
        *flags = (*flags & (int)0xFFFC007F) | ((obj->x190 & 0x7FF) << 7);
    }
}
