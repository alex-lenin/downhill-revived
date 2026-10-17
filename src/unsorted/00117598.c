/* UNIT 0x00117598 0x00117658 */
struct Obj {
    char pad[0x10030];
    int x10030;
    int x10034;
    float x10038;
    float x1003c;
    float x10040;
    float x10044;
    float x10048;
    int x1004c;
    int x10050;
    int x10054;
    char pad58[0xC];
    int x10064;
    char x10068;
    char x10069;
};

extern int func_0023CF58(int);
extern int func_0023CF78(int);
extern void func_001177D8(struct Obj *);

void func_00117598(struct Obj *obj, int value)
{
    obj->x10050 = value;
    obj->x1004c = 0;
    obj->x10030 = func_0023CF58(value);
    obj->x10034 = func_0023CF78(value);
    obj->x10054 = 0;
    obj->x10068 = 1;
    obj->x10038 = 1000000.0f;
    obj->x10069 = 1;
    obj->x1003c = 0.75f;
    obj->x10040 = 0.375f;
    obj->x10044 = 0.5f;
    obj->x10048 = 0.0008f;
    obj->x10064 = 0;
    func_001177D8(obj);
}
