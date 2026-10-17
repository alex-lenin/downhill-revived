/* UNIT 0x0011B6E0 0x0011B778 */
typedef struct { float x, y, z; } Vec;
typedef struct { char pad[8]; Vec x8; Vec x14; } Obj;
extern void func_00221F20(void *);
extern void func_002226C8(void *, Vec *);
extern void func_00222460(void *, Vec *);
void func_0011B6E0(Obj *obj, Vec *pos, Vec *dir, void *matrix) {
    float x, y, z, dx, dy, dz;
    pos->z += 2.0f;
    x = obj->x8.x;
    z = obj->x8.z;
    y = obj->x8.y;
    pos->z = z;
    pos->y = y;
    pos->x = x;
    dx = obj->x14.x;
    dy = obj->x14.y;
    dz = obj->x14.z;
    dir->x = dx;
    dir->y = dy;
    dir->z = dz;
    func_00221F20(matrix);
    func_002226C8(matrix, dir);
    func_00222460(matrix, dir);
}
