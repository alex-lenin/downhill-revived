/* UNIT 0x0019A768 0x0019A7C0 */
typedef struct Table {
    char pad0[0x12];
    signed char x12;
    char pad13;
    int x14[1];
} Table;

typedef struct Obj {
    char pad0[0x504];
    int x504;
} Obj;

extern Table D_0043DCC0;
extern void func_0019AE30(Obj *obj);

void func_0019A768(Obj *obj)
{
    int i;

    for (i = 0; i < 4; i++) {
        D_0043DCC0.x14[D_0043DCC0.x12] = 0;
    }
    obj->x504 = 0;
    func_0019AE30(obj);
}
