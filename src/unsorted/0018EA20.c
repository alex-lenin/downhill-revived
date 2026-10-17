/* UNIT 0x0018EA20 0x0018EA60 */
typedef struct VEntry {
    short delta;
    short pad;
    void (*func)(void *self);
} VEntry;

typedef struct Target {
    char pad0[0x4F8];
    VEntry *vtbl;
} Target;

typedef struct Obj {
    int x0;
} Obj;

extern Target *func_0018EAC8(Obj *obj, int key);

void func_0018EA20(Obj *obj)
{
    Target *t = func_0018EAC8(obj, obj->x0);

    if (t != 0) {
        VEntry *e = &t->vtbl[2];
        e->func((char *)t + e->delta);
    }
}
