/* UNIT 0x00163418 0x001634C8 */
typedef struct { int x0; float x4, x8; int xC; float x10, x14, x18, x1C, x20, x24, x28, x2C; } Obj;
extern void func_00254204(void *, int, int);
extern void func_001634C8(Obj *);
void func_00163418(Obj *obj, int value) {
    func_00254204(obj, 0, 0x30);
    obj->x8 = 100.0f;
    obj->x24 = 1.0f;
    obj->x0 = value;
    obj->xC = 0;
    obj->x4 = 100.0f;
    obj->x10 = 0.1f;
    obj->x28 = 1.0f;
    obj->x14 = 0.2f;
    obj->x1C = 20.0f;
    obj->x18 = 0.45f;
    obj->x20 = 10.0f;
    func_001634C8(obj);
}
