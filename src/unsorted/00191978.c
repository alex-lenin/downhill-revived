/* UNIT 0x00191978 0x001919E8 */
struct Vec { float x, y, z; };
struct Obj { int x0, x4, x8, xc, x10, x14, x18, x1c, x20, x24, x28, x2c, x30, x34, x38, x3c; char pad40[16]; struct Vec x50; int x5c; float x60, x64, x68, x6c, x70, x74, x78, x7c; int x80; };
void func_00191978(struct Obj *obj) {
 struct Vec *vec = &obj->x50;
 obj->x4 = 0; obj->x8 = 0; obj->xc = 0; obj->x10 = 0; obj->x14 = 0; obj->x18 = 0; obj->x1c = 0;
 obj->x28 = 0; obj->x2c = 0; obj->x38 = 0; obj->x3c = 0;
 vec->x = 0; vec->y = 0; vec->z = 1;
 obj->x60 = 0; obj->x64 = 0; obj->x68 = 0; obj->x6c = 1; obj->x70 = 1;
 obj->x74 = 0; obj->x78 = 0; obj->x7c = 0; obj->x80 = 0;
}
