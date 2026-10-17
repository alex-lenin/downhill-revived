/* UNIT 0x0018E4C0 0x0018E4D8 */
typedef struct Link {
    char pad0[0x8];
    void *x8;
} Link;

typedef struct Obj {
    char pad0[0x4AC];
    Link *x4AC;
} Obj;

void func_0018E4C0(Obj *obj, Link *link)
{
    obj->x4AC = link;
    if (link) {
        link->x8 = &obj->x4AC;
    }
}
