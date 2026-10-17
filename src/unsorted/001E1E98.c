/* UNIT 0x001E1E98 0x001E1EA8 */
typedef struct {
    char pad0[0x500];
    int x500;
    int x504;
} Obj;

void func_001E1E98(Obj *p)
{
    p->x500 = 0;
    p->x504 = 0;
}
