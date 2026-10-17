/* UNIT 0x0023E168 0x0023E188 */
typedef struct Obj {
    char pad0[0xC];
    volatile int xC;
} Obj;

void func_0023E168(Obj *obj)
{
    if (obj->xC > 0) {
        obj->xC--;
    }
}
