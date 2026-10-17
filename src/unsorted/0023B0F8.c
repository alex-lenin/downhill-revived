/* UNIT 0x0023B0F8 0x0023B140 */
typedef struct Obj { unsigned int x0; int x4; int x8; } Obj;
unsigned int func_0023B0F8(Obj *p, unsigned int value) {
    if (value == (((p->x8 * 16) + p->x4 + 16) & 0x0FFFFFFF)) return 0;
    return (value - p->x0) >> 11;
}
