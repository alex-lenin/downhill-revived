/* UNIT 0x001AA128 0x001AA140 */
typedef struct Obj {
    unsigned char x0;
    char pad1[3];
    int x4;
} Obj;

extern int D_0039E834[];

void func_001AA128(Obj *obj)
{
    D_0039E834[0] = obj->x4 != 0;
    obj->x0 = 0;
}
