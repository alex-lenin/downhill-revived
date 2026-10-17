/* UNIT 0x0010A490 0x0010A528 */
typedef struct Obj {
    char pad0[0x40C0];
    char x40C0[0x78D0 - 0x40C0];
    int x78D0;
} Obj;

typedef struct Global {
    char pad0[0x50130];
    Obj *objs[1];
} Global;

typedef struct Arg {
    char pad0[8];
    int index;
} Arg;

extern Global D_002AF2C0;

extern void func_00231A08(void *, int);
extern void func_00231A78(void *, int);
extern void func_0012C130(Obj *, int, int);
extern void func_0012C450(Obj *);

void func_0010A490(int a, int b, Arg *arg)
{
    Obj *obj;
    void *sub;

    obj = D_002AF2C0.objs[arg->index];
    sub = obj->x40C0;
    func_00231A08(sub, a);
    func_00231A78(sub, b);
    func_0012C130(obj, arg->index, obj->x78D0);
    func_0012C450(obj);
}
