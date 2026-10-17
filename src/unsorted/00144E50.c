/* UNIT 0x00144E50 0x00144E90 */
typedef struct Obj { char pad0[8]; int **x8; char padC[0x1A8]; int x1B4; } Obj;
void func_00144E50(Obj *obj) {
    int **ptr = obj->x8;
    if (ptr) {
        int *flags = *ptr;
        *flags = (*flags & (int)0xFFFC007F) | ((obj->x1B4 & 0x7FF) << 7);
    }
}

