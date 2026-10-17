/* UNIT 0x00173358 0x001733C0 */
typedef struct Obj {
    char pad0[0xB0];
    float xB0;
    float xB4;
    float xB8;
} Obj;

extern void func_001731C8(Obj *obj);
extern void func_00173A18(Obj *obj);

void func_00173358(Obj *obj)
{
    obj->xB0 = obj->xB8 * 61.363636f + 20.0f;
    if (obj->xB4 < obj->xB0) {
        func_001731C8(obj);
        func_00173A18(obj);
    }
}
