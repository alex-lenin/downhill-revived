/* UNIT 0x0011E528 0x0011E568 */
typedef struct Bits {
    unsigned int lo : 7;
    unsigned int x7 : 11;
    unsigned int hi : 14;
} Bits;

typedef struct Inner {
    Bits *bits;
} Inner;

typedef struct Obj {
    char pad0[0x10];
    Inner *x10;
    char pad14[0x254];
    short x268;
} Obj;

void func_0011E528(Obj *obj) {
    if (obj->x10) {
        obj->x10->bits->x7 = obj->x268;
    }
}
