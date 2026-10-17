/* UNIT 0x0021E3D8 0x0021E410 */
typedef struct { char pad0[0x20]; void *x20; char pad24[0x1c]; } Entry;
extern Entry D_0062F880[];
void *func_0021E3D8(int index) {
    int mask = 1 << index;
    int bits = *(int *)0x70000208;
    if (mask & bits) return ((Entry *)((index << 6) + (char *)D_0062F880))->x20;
    return 0;
}

