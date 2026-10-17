/* UNIT 0x001AA688 0x001AA6B8 */
typedef struct Obj {
    char pad0[0x20];
    void *x20;
} Obj;

extern void func_001AACA8(void *p);

void func_001AA688(Obj *obj)
{
    func_001AACA8(obj->x20);
    obj->x20 = 0;
}
