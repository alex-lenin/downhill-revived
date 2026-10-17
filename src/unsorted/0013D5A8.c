/* UNIT 0x0013D5A8 0x0013D608 */
typedef struct {
    char pad0[0x51C];
    int x51C;
    int pad520;
    int x524;
    int pad528;
    int x52C;
    int pad530;
    int x534;
    int pad538;
    int x53C;
} Obj;

extern int func_0013AF60(int id);

void func_0013D5A8(Obj *o)
{
    o->x51C = func_0013AF60(0x258D);
    o->x524 = func_0013AF60(0x258C);
    o->x52C = func_0013AF60(0x258F);
    o->x534 = func_0013AF60(0x2590);
    o->x53C = func_0013AF60(0x258E);
}
