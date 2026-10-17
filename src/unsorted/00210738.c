/* UNIT 0x00210738 0x00210760 */
struct Vector { float x, y, z; };
struct Obj { struct Vector pos; char pad[0x28]; int x34; };
void func_00210738(struct Obj *obj, int value, struct Vector *pos) {
    float x, y, z;
    obj->x34 = value;
    if (pos != 0) {
        x = pos->x;
        y = pos->y;
        z = pos->z;
        obj->pos.x = x;
        obj->pos.y = y;
        obj->pos.z = z;
    }
}
