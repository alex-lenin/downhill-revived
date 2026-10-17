/* UNIT 0x00122410 0x001224F0 */
struct Vec { float x0, x4, x8; };
struct Obj { int count; char pad4[0xC]; float x10, x14, x18; int pad1C; float x20, x24, x28; char pad2C[0x14]; short indices[48]; };
extern char D_00412510[];
extern struct Vec *func_00163D40(void *, int);
void func_00122410(struct Obj *obj, int index) {
    struct Vec *pos;
    float x, y, z, old;
    int count = obj->count;
    if (count < 48) {
        obj->indices[count] = index;
        obj->count = count + 1;
        pos = func_00163D40(D_00412510, index);
        x = pos->x0;
        if (x < obj->x10) obj->x10 = x;
        old = obj->x14;
        y = pos->x4;
        if (y < old) obj->x14 = y;
        old = obj->x18;
        z = pos->x8;
        if (z < old) obj->x18 = z;
        if (obj->x20 < pos->x0) obj->x20 = pos->x0;
        if (obj->x24 < pos->x4) obj->x24 = pos->x4;
        if (obj->x28 < pos->x8) obj->x28 = pos->x8;
    }
}
