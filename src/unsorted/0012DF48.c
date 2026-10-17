/* UNIT 0x0012DF48 0x0012DFD0 */
struct Obj { char pad0[0x1184]; int x1184; char pad1188[0x124C-0x1188]; int x124C; char pad1250[0x13D4-0x1250]; unsigned char x13D4; char pad13D5[0x4ECC-0x13D5]; void *x4ECC; };
extern int func_00138560(void *);
extern int func_001E64C8(void *);
float func_0012DF48(struct Obj *obj) {
    int value;
    float result;
    if ((unsigned char)func_00138560((char *)obj + 0x40C0) || func_001E64C8((char *)obj + 0x6610)) return 0.0f;
    if (obj->x4ECC == (char *)obj + 0x6298) return 1.0f;
    if (obj->x13D4) value = obj->x124C;
    else value = obj->x1184;
    result = 0.0f;
    if (value) result = 1.0f;
    return result;
}
