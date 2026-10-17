/* UNIT 0x001E4878 0x001E4900 */
typedef struct Obj {
    char pad0[0x418];
    int x418;
    char pad41C[0x8C];
    int x4A8;
} Obj;

extern char *D_00271878[];
extern int D_0029DE68;

extern void func_0018E408();
extern void func_0018E6B8();
extern void func_0018DB08();

void func_001E4878(Obj *obj)
{
    int i;

    obj->x418 = 0;
    for (i = 0; i < 15; i++) {
        func_0018E408(obj, D_00271878[i], 1);
    }
    obj->x4A8 = 5;
    func_0018E6B8(obj, 0xA6, &D_0029DE68);
    func_0018DB08(obj);
}
