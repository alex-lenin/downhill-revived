/* UNIT 0x0017B3C0 0x0017B510 */
struct Obj {
    char pad0[0x1280];
    int x1280;
    char pad1284[0x12A4 - 0x1284];
    int x12A4;
    char pad12A8[0x12E8 - 0x12A8];
    int x12E8;
    int x12EC;
    int x12F0;
    int x12F4;
    int x12F8;
    int x12FC;
    int x1300;
    int x1304;
    int x1308;
};

extern int *D_0029DD78;
extern int D_004B2828[];

extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0017B520(struct Obj *, int, float, float);

void func_0017B3C0(struct Obj *obj)
{
    obj->x12F0--;
    if (obj->x12F0 <= 0) {
        obj->x12F0 = 0;
        if (obj->x12F8 > 0) {
            obj->x12E8 = obj->x12F8;
            obj->x12EC = obj->x12FC;
            obj->x12F0 = 180;
            obj->x12F4 = obj->x1300;
            obj->x1300 = 10;
            obj->x12F8 = 0;
            obj->x12FC = 0;
        }
    }
    if (obj->x12EC != 0) {
        obj->x1308++;
        if (obj->x1308 >= 16) {
            obj->x1308 = 0;
            obj->x1304 ^= 1;
        }
        if (obj->x1304 != 0) {
            return;
        }
    }
    if (obj->x12F0 > 0 && obj->x12A4 == 0) {
        if (D_004B2828[0] >= 2) {
            func_0020FB80(0, 0xE0, 0xC0, -0x5B, -0x20);
        } else {
            func_0020FB80(0, 0x140, 0xE0, -0x7B, -0x20);
        }
        func_0020F588(obj->x1280, 0, 0x6E, 0x6E, 0xC8, 0xB4);
        func_0017B520(obj, D_0029DD78[obj->x12E8], 1.0f, 0.13f);
    }
}
