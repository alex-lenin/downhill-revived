/* UNIT 0x001786A0 0x00178730 */
typedef struct { char pad0[0x1278]; unsigned int *x1278; } Obj;

void func_001786A0(Obj *obj, int index, unsigned int color, unsigned int alpha) {
    int i;

    color = (color & 0xffffff) | (alpha << 24);
    if (index == 10) {
        unsigned int *p = obj->x1278;
        for (i = 5; i >= 0; i--) *p++ = color;
    } else {
        unsigned int *q = obj->x1278 + 6;
        q = (unsigned int *)((char *)q + (index - 1) * 0x10);
        for (i = 3; i >= 0; i--) *q++ = color;
    }
}
