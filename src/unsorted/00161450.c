/* UNIT 0x00161450 0x001614D8 */
typedef struct {
    unsigned long x0;
    unsigned long x8;
} Obj;

extern void func_001614D8(Obj *obj, int kind);

void func_00161450(Obj *obj, int id, int kind)
{
    long bit;

    if ((unsigned int)(id - 0x332C) < 100) {
        if (kind != 1) {
            goto done;
        }
        bit = id - 0x332C;
        obj->x8 |= 1UL << (int)bit;
    } else {
        if (kind != 0) {
            goto done;
        }
        bit = id - 0x3390;
        obj->x0 |= 1UL << (int)bit;
    }
done:
    func_001614D8(obj, kind);
}
