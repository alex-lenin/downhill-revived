/* UNIT 0x001AFDF0 0x001AFE00 */
typedef struct Obj {
    int x0;
    char pad4[0x74];
    int x78;
    char pad7C[0x74];
    int xF0;
} Obj;

void func_001AFDF0(Obj *obj)
{
    obj->xF0 = 0;
    obj->x0 = 0;
    obj->x78 = 0;
}
