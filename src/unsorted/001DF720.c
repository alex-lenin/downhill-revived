/* UNIT 0x001DF720 0x001DF810 */
typedef struct Obj {
    char pad0[0x8C];
    int x8C;
    char pad90[0xB4 - 0x90];
    float xB4;
    float xB8;
    char padBC[0x188 - 0xBC];
    int x188;
    char pad18C[0x194 - 0x18C];
    int x194;
    int x198;
    char pad19C[0x1C8 - 0x19C];
    int handles[10];
    int x1F0;
    int x1F4;
} Obj;

extern int func_00238F48(void);
extern void func_00201DE8(int handle, int a, int volume, int b, int c, int d);
extern void func_002025A0(int handle);

void func_001DF720(Obj *obj)
{
    float elapsed = func_00238F48() - obj->x1F0;
    int volume = 0x400 - (int)(elapsed / (obj->xB8 * 60.0f) * 1024.0f);

    if (elapsed < obj->xB4 * 60.0f && volume > 0) {
        func_00201DE8(obj->handles[obj->x194], 1, volume, 0, 0, 0);
        return;
    }
    func_002025A0(obj->handles[obj->x194]);
    obj->handles[obj->x194] = 0;
    obj->x8C = 0;
    obj->x1F4 = 0;
    obj->x198 = 0;
    obj->x188 = 1;
}
