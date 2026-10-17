/* UNIT 0x00184940 0x00184980 */
typedef struct Obj {
    int x0;
    void *x4;
    char pad8[0x8];
    char x10[1];
} Obj;

extern char D_002861F8[];
extern void func_00187378(void *p);

Obj *func_00184940(Obj *obj)
{
    obj->x0 = 0;
    obj->x4 = D_002861F8;
    func_00187378(obj->x10);
    return obj;
}
