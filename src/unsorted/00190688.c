/* UNIT 0x00190688 0x001906A8 */
typedef struct Other {
    char pad0[0x4AC];
    int x4AC;
} Other;

typedef struct Obj {
    char pad0[0x150];
    Other *x150;
} Obj;

extern void func_001904B8(Obj *obj, int value);

void func_00190688(Obj *obj)
{
    func_001904B8(obj, obj->x150->x4AC);
}
