/* UNIT 0x001BBEE8 0x001BBEF8 */
typedef struct Inner {
    char pad0[0x4584];
    char x4584;
} Inner;

typedef struct Obj {
    Inner *x0;
} Obj;

int func_001BBEE8(Obj *obj)
{
    obj->x0->x4584 = 0;
    return 0;
}
