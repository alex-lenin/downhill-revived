/* UNIT 0x001AD8D8 0x001AD940 */
struct Obj {
    int state;
    char pad4[0xC - 0x4];
    int xC;
    char pad10[0xF8 - 0x10];
    int xF8;
};

extern void func_001ADBC0(struct Obj *obj);
extern void func_001AD940(struct Obj *obj);

void func_001AD8D8(struct Obj *obj)
{
    if (obj->xF8 != 0) {
        switch (obj->state) {
        case 0:
            func_001ADBC0(obj);
            break;
        case 1:
            func_001AD940(obj);
            break;
        }
        obj->xC = 0;
    }
}
