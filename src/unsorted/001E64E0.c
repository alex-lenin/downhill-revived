/* UNIT 0x001E64E0 0x001E64F0 */
typedef struct Inner {
    char pad0[0x61E3];
    unsigned char x61E3;
} Inner;

typedef struct Obj {
    char pad0[0x40];
    Inner *x40;
} Obj;

unsigned char func_001E64E0(Obj *obj)
{
    return obj->x40->x61E3;
}
