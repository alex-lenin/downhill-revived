/* UNIT 0x00192B90 0x00192BA8 */
typedef struct Obj {
    char pad0[0xC];
    int xC;
} Obj;

void func_00192B90(Obj *obj, int value, int mode)
{
    if (mode == 2) {
        obj->xC = value;
    }
}
