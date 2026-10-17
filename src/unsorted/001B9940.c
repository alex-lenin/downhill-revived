/* UNIT 0x001B9940 0x001B9978 */
typedef struct Vec { int x0, x4, x8; } Vec;
void func_001B9940(char *obj, int source, int dest) {
    Vec *src = (Vec *)(obj + (source * 12 + 0x11C));
    Vec *dst = (Vec *)(obj + (dest * 12 + 0x14C));
    dst->x0 = src->x0;
    dst->x4 = src->x4;
    dst->x8 = src->x8;
}
