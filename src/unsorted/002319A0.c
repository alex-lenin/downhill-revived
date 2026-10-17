/* UNIT 0x002319A0 0x002319C8 */
typedef struct { float x, y, z; } Vec;
typedef struct { char pad0[0x10]; Vec x10; } Obj;
extern void func_00232A38(Obj *);
void func_002319A0(Obj *obj, float x, float y, float z) {
    Vec *pos = &obj->x10;
    pos->x = x;
    pos->y = y;
    pos->z = z;
    func_00232A38(obj);
}
