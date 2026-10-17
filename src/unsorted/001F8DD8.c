/* UNIT 0x001F8DD8 0x001F8E30 */
struct Msg {
    int x0;
    int x4;
    int x8;
    int xC;
};

struct Obj {
    char pad0[0x500];
    char x500[1];
};

extern void func_00203590(struct Msg *, int, int, int);
extern void func_002039B0(struct Msg *, int);
extern void func_0018E468(struct Obj *);
extern void func_001AC880(void *, float);

void func_001F8DD8(struct Obj *obj)
{
    struct Msg msg;

    msg.x8 = 0;
    func_00203590(&msg, 0x25B1, 0, 2);
    func_002039B0(&msg, 1);
    func_0018E468(obj);
    func_001AC880(obj->x500, 0.0f);
}
