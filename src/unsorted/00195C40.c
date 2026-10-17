/* UNIT 0x00195C40 0x00195CA8 */
struct Obj {
    int x0;
    int x4;
    int x8;
    char padC[0x27 - 0xC];
    unsigned char x27;
};

extern char D_004B2914[];
extern void func_00201A30(int flag);
extern void func_001C6220(void *p, int a);
extern void func_001C6258(void *p, int a, int b);
extern void func_001C65F0(void *p, int a);

void func_00195C40(struct Obj *obj)
{
    char *target = D_004B2914;

    func_00201A30(obj->x27 == 0);
    func_001C6220(target, obj->x8);
    func_001C6258(target, obj->x4, 0);
    func_001C65F0(target, obj->x0);
}
