/* UNIT 0x001576B8 0x001576D0 */
typedef struct Inner {
    char pad0[0x4098];
    unsigned long x4098;
} Inner;

typedef struct Obj {
    Inner *x0;
} Obj;

int func_001576B8(Obj *obj)
{
    return (unsigned char)(obj->x0->x4098 >> 2) & 1;
}
