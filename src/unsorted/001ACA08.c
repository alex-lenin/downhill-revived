/* UNIT 0x001ACA08 0x001ACA78 */
struct Handle { int x0, x4, x8, xC; };
struct Obj { struct Handle *x0; char pad4[0x40]; float x44; char pad48[0x14]; float x5C; };
extern void func_00203380(struct Handle *, void *, float);
int func_001ACA08(struct Obj *obj) {
    struct Handle handle;
    obj->x44 += obj->x5C;
    if (obj->x44 <= 0.0f) {
        obj->x5C = 0.0f;
        obj->x44 = 0.0f;
        handle = *obj->x0;
        func_00203380(&handle, (char *)obj + 4, 0.0f);
        return 0;
    }
    return 1;
}
