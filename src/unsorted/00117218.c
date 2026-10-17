/* UNIT 0x00117218 0x00117278 */
typedef struct Obj {
    char pad0[0xB4];
    int xB4;
    int xB8;
} Obj;

extern int func_00238F60(void);

void func_00117218(Obj *p)
{
    int v;

    v = p->xB4 - func_00238F60();
    if (v < 0) v = 0;
    p->xB4 = v;
    v = p->xB8 - func_00238F60();
    if (v < 0) v = 0;
    p->xB8 = v;
}
