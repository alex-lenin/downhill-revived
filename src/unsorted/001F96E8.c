/* UNIT 0x001F96E8 0x001F97A8 */
typedef struct Vec3 {
    int x, y, z;
} Vec3;

extern int D_004B26AC[];
extern Vec3 D_00277BC8[];
extern Vec3 D_00277C28[];
extern Vec3 D_00277C88[];
extern Vec3 D_00277CE8[];
extern Vec3 D_00277E18[];

void func_001F96E8(Vec3 *out, int index)
{
    int mode = D_004B26AC[0];
    Vec3 *src = 0;

    if (mode == 0) {
        src = &D_00277BC8[index];
    } else if (mode == 1) {
        src = &D_00277C28[index];
    } else if (mode == 2) {
        src = &D_00277C88[index];
    } else if (mode == 3) {
        src = &D_00277CE8[index];
    } else if (mode == 4) {
        src = &D_00277E18[index];
    }
    if (src != 0) {
        out->x = src->x;
        out->y = src->y;
        out->z = src->z;
    }
}
