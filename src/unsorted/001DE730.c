/* UNIT 0x001DE730 0x001DE740 */
typedef struct {
    char pad0[0x70];
    int x70;
    int x74;
} Obj;

void func_001DE730(Obj *p, int value)
{
    p->x74 = value;
    p->x70 = 1;
}
