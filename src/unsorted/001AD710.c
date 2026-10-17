/* UNIT 0x001AD710 0x001AD760 */
typedef struct { char pad[0x18]; unsigned char x18; unsigned char x19; char pad1a[2]; float x1c; char pad20[0xc]; float x2c; char pad30[4]; float x34; } Obj;
extern float fabsf(float);
void func_001AD710(volatile Obj *obj) {
    float value;
    if (obj->x19 != 1 || obj->x18 == 0) {
        value = fabsf(((Obj *)obj)->x1c);
        obj->x34 = 1.0f;
        obj->x2c = -1.0f;
        obj->x1c = value;
        obj->x18 = 1;
        obj->x19 = 1;
    }
}
