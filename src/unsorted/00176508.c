/* UNIT 0x00176508 0x00176568 */
struct Obj { char pad[0xFA4]; int *xFA4; int *xFA8; int *xFAC; };
void func_00176508(struct Obj *obj) {
    int *p;
    int *last;
    p = obj->xFA4;
    if (p) *p = (*p & ~0x3F) | 1;
    p = obj->xFA8;
    if (p) *p = (*p & ~0x3F) | 1;
    last = obj->xFAC;
    if (last) *last = (*last & ~0x3F) | 1;
}
