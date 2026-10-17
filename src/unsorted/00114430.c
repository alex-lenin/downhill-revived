/* UNIT 0x00114430 0x001144F0 */
struct Rider { char pad[0x4564]; short x4564; };
struct Obj {
    struct Rider *x0;
    char pad[0x2b8];
    float x2BC;
    char pad2[8];
    float x2C8;
    int x2CC;
    char pad3[0xbc];
    int x38C;
    char pad4[0xb0];
    unsigned long flags;
};
extern int func_00138560(void *);

int func_00114430(struct Obj *obj) {
    int result = 0;

    if (obj->x38C == 0 && (func_00138560((char *)obj->x0 + 0x40c0) & 255)) {
        if ((obj->flags & ((unsigned long)0x8000 << 40)) || obj->x0->x4564 < 20) {
            if (1.1f < obj->x2BC) {
                result = 1;
            }
        } else if (1.75f < obj->x2BC) {
            result = 1;
        }
    }
    if (result) {
        obj->x2C8 = obj->x2BC;
        obj->x2CC = 0;
    }
    return result;
}
