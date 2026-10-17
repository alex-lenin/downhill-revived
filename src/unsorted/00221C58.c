/* UNIT 0x00221C58 0x00221C78 */
typedef struct { float x0, x4, x8; } Vec;
void func_00221C58(Vec *out, Vec *in) {
    out->x0 = in->x4;
    out->x4 = -in->x0;
    out->x8 = 0.0f;
}
