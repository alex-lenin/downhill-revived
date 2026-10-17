/* UNIT 0x0012C990 0x0012C9B8 */
typedef struct Obj { char pad0[0x7BAA]; unsigned char x7BAA; } Obj;
float func_0012C990(Obj *obj) {
    float value = 1.0f;
    if (obj->x7BAA != 0) value = 2.0f;
    return value;
}
