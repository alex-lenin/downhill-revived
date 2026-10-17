/* UNIT 0x00169EB0 0x00169ED8 */
typedef struct Obj {
    int x0;
    int x4;
    float x8;
    int xC;
    float x10[4];
} Obj;

Obj *func_00169EB0(Obj *obj)
{
    obj->xC = 0;
    obj->x8 = 1.0f;
    obj->x0 = 0;
    /* x10 = vf0 = (0, 0, 0, 1) */
    __asm__ volatile ("sqc2 $vf0, 0x10(%0)" : : "r"(obj) : "memory");
    obj->x4 = 0;
    return obj;
}
