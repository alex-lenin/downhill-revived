/* UNIT 0x0012C9B8 0x0012C9E0 */
typedef struct Obj { char pad0[0x7BA9]; unsigned char x7BA9; } Obj;
float func_0012C9B8(Obj *obj) {
    float value = 1.0f;
    if (obj->x7BA9 != 0) value = 0.5f;
    return value;
}
