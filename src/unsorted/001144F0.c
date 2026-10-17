/* UNIT 0x001144F0 0x001145F8 */
typedef struct Obj {
    int x0;
    char pad4[0x54];
    int x58;
    int x5C;
    char pad60[0x4C];
    int xAC;
    char padB0[0x20];
} Obj;

extern void func_00254204(void *dst, int value, int size);
extern void func_001161F8(Obj *);
extern void func_001145F8(Obj *);
extern void func_001146F0(Obj *);
extern void func_001148E8(Obj *);
extern void func_00115608(Obj *);
extern void func_00115758(Obj *);
extern void func_00115D78(Obj *);
extern void func_00115F20(Obj *);
extern void func_00115FA8(Obj *);
extern void func_00116170(Obj *);
extern void func_00116290(Obj *);
extern void func_00116368(Obj *);
extern void func_001162D0(Obj *);
extern void func_00116828(Obj *);
extern void func_00116868(Obj *);
extern void func_001168A0(Obj *);
extern void func_001169F8(Obj *);
extern void func_00116C48(Obj *);
extern int func_00222BD8(int min, int max);

void func_001144F0(Obj *obj, int a, int b)
{
    func_00254204(obj, 0, sizeof(Obj));
    obj->x0 = 4;
    obj->x58 = a;
    obj->x5C = b;
    func_001161F8(obj);
    func_001145F8(obj);
    func_001146F0(obj);
    func_001148E8(obj);
    func_00115608(obj);
    func_00115758(obj);
    func_00115D78(obj);
    func_00115F20(obj);
    func_00115FA8(obj);
    func_00116170(obj);
    func_00116290(obj);
    func_00116368(obj);
    func_001162D0(obj);
    func_00116828(obj);
    func_00116868(obj);
    func_001168A0(obj);
    func_001169F8(obj);
    func_00116C48(obj);
    obj->xAC = func_00222BD8(obj->xAC / 4, obj->xAC);
}
