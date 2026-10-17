/* UNIT 0x001122D8 0x00112360 */
struct Obj { char pad0[0x70]; int x70; char pad74[0x24C-0x74]; float x24C; char pad250[0x18]; float x268; char pad26C[0xC]; float x278; };
extern float func_00117ED0(void *, void *, int, int);
extern void func_002087C0(float *);
void func_001122D8(struct Obj *obj, int arg) {
    float angle;
    if (obj->x70) {
        obj->x278 = func_00117ED0((char *)obj + 0x10, (char *)obj + 0x30, arg, 0);
        angle = obj->x268 - obj->x24C;
        func_002087C0(&angle);
        if (angle < 0.0f) obj->x278 = -obj->x278;
    } else obj->x278 = 0.0f;
}
