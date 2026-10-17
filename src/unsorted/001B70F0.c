/* UNIT 0x001B70F0 0x001B7198 */
struct Slot { char pad[0x150]; char x150[0x1380 - 0x150]; };
extern char D_00663B08[];
extern char D_003965E0[];
extern struct Slot D_003B2270[];
extern int D_004B1F40[];
extern void func_00223B00(void *);
extern void func_00238A68(int);
extern void func_00218038(void);
extern void func_0020E860(void);
extern void func_0021D770(void);
extern void func_00220CC0(void);
extern void func_0022F8C0(void);
extern void func_001764F8(void *);
extern void func_00166F20(void *);

void func_001B70F0(void)
{
    int i;
    struct Slot *slot;

    func_00223B00(D_00663B08);
    func_00238A68(60);
    func_00218038();
    func_0020E860();
    func_0021D770();
    func_00220CC0();
    func_0022F8C0();
    for (i = 0; i < 4; i++) {
        slot = &D_003B2270[i];
        func_001764F8(slot->x150);
    }
    func_00166F20(D_003965E0);
    D_004B1F40[0] = 0x1C20;
}
