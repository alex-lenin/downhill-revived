/* UNIT 0x001E6548 0x001E65D0 */
struct Obj {
    int x0;
    char pad4[0x10 - 0x4];
    float x10;
    float x14;
    float x18;
    float x1C;
    char pad20[0x718 - 0x20];
    int x718;
    int x71C;
};

extern int func_001E5E58(struct Obj *obj, int a);
extern float func_001E51C0(struct Obj *obj, int a);
extern float func_001E51E8(struct Obj *obj, int a);

int func_001E6548(struct Obj *obj)
{
    float total;

    total = obj->x1C + (float)func_001E5E58(obj, obj->x0) + obj->x10 + obj->x14 + obj->x18;
    total *= func_001E51C0(obj, obj->x718);
    total *= func_001E51E8(obj, obj->x71C);
    return (int)total;
}
