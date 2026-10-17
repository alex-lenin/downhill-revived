/* UNIT 0x0018E4D8 0x0018E4F0 */
typedef struct Link {
    char pad0[0x8];
    void *x8;
} Link;

typedef struct Obj {
    char pad0[0x4B0];
    Link *x4B0;
} Obj;

void func_0018E4D8(Obj *obj, Link *link)
{
    obj->x4B0 = link;
    if (link) {
        link->x8 = &obj->x4B0;
    }
}
