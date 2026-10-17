/* UNIT 0x0015F730 0x0015F838 .rodata=0x28A458:0x28A498 */
typedef struct Global {
    char pad0[0x20];
    int x20;
    char pad24[0x64 - 0x24];
    char x64[0x12D - 0x64];
    char x12D;
} Global;

typedef struct Obj {
    char pad0[0x45C];
    int x45C;
    char pad460[0x504 - 0x460];
    int x504;
    int x508;
} Obj;

extern Global D_003F52D0;
extern char D_004B28FC[];
extern void func_0023D650(int, int *, int *);
extern void func_0018DC20(Obj *);
extern void func_001B5AF0(Global *, int, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, const char *);
extern int func_00190D60(char *, int, int);

void func_0015F730(Obj *obj)
{
    Global *g = &D_003F52D0;
    int size[2];

    func_0023D650(g->x20, &size[0], &size[1]);
    size[0] /= 2;
    func_0018DC20(obj);
    func_001B5AF0(g, 1, 1);
    if (obj->x504) {
        func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
        func_0020FB80(0, 0x120, 0xA8, -0x58, -0x20);
        if (obj->x508) {
            func_0020F9F0(0, g->x20, size[0], 0xA0, "Controller Unplugged, Replay Stopped.");
        } else {
            func_0020F9F0(0, g->x20, size[0], 0xA0, "End of Recorded Replay");
        }
    } else if (func_00190D60(g->x64, 8, obj->x45C)) {
        D_004B28FC[0] = 1;
        g->x12D = 1;
    }
}
