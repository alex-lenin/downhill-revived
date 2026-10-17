/* UNIT 0x001DE8D0 0x001DE918 */
extern void func_001DF600(unsigned int *h);

struct Obj {
    char pad0[0x70];
    int x70;
    char pad74[0x178 - 0x74];
    unsigned int x178;
};

void func_001DE8D0(struct Obj *obj)
{
    if (obj->x178 != 0xFFFFFFFF && obj->x178 != 0) {
        func_001DF600(&obj->x178);
    }
    obj->x70 = 0;
}
