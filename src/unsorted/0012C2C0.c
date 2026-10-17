/* UNIT 0x0012C2C0 0x0012C2E8 */
typedef struct Sub {
    char pad0[0x20];
    int x20;
} Sub;

typedef struct Obj {
    char pad0[0x793C];
    Sub *x793C;
} Obj;

extern void func_0021D910(int a);

void func_0012C2C0(Obj *obj)
{
    if (obj->x793C != 0) {
        func_0021D910(obj->x793C->x20);
    }
}
