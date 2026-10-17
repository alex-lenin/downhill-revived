/* UNIT 0x0022F8C0 0x0022FAE8 .sdata=0x29DBC8:0x29DBF8 */
typedef struct Slot {
    char pad00[0x0C];
    float x0C;
    char pad10[0x0C];
    float x1C;
    int x20;
    int x24;
    int x28;
    float x2C;
    int x30;
    int x34;
    int x38;
    int x3C;
    int x40;
    float x44;
    int x48;
    int x4C;
    int x50;
    float x54;
    float x58;
    float x5C;
    float x60;
    float x64;
    float x68;
    float x6C;
    float m70[16];
    float mB0[16];
    float mF0[16];
} Slot;

extern Slot D_00734680[32];
extern float D_00736CC0[4];
extern float D_00736CD0[4];
extern float D_00736CE0[4];
extern float D_00736CF0[4];
extern float D_00736D00[4];
extern int D_0029E318;
extern int D_0029E31C;
extern int D_0029E320;
extern int D_0029E324;
extern int D_0029E328;
extern int D_0029E32C;
extern float D_0029E330;
extern float D_0029E334;
int D_0029DBC8 = 0;
int D_0029DBCC = 0;
int D_0029DBD0 = 128;
int D_0029DBD4 = 0;
int D_0029DBD8 = 128;
int D_0029DBDC = 0;
int D_0029DBE0 = 128;
int D_0029DBE4 = 0;
int D_0029DBE8 = 0;
int D_0029DBEC = 0;
char D_0029DBF0[8] = ".Kanji.";

extern void func_00221F20(float *);
extern void func_0022FAE8(int);
extern void func_0022FE60(void);

void func_0022F8C0(void)
{
    int i;
    Slot *s;

    for (i = 0; i < 32; i++) {
        s = &D_00734680[i];
        s->x20 = 0;
        s->x24 = 0;
        s->x28 = 0;
        s->x2C = 100.0f;
        s->x30 = 0;
        s->x34 = 0;
        s->x38 = 0;
        s->x3C = 0;
        s->x40 = 0;
        s->x44 = 1.0f;
        s->x48 = 0;
        s->x4C = 0;
        s->x54 = 1.0f;
        s->x58 = 1.0f;
        s->x5C = 100.0f;
        s->x60 = 1.0f;
        s->x64 = -1.0f;
        s->x68 = 1.0f;
        s->x6C = -1.0f;
        s->x0C = 45.0f;
        s->x1C = 45.0f;
        func_00221F20(D_00734680[i].m70);
        func_00221F20(D_00734680[i].mB0);
        func_00221F20(D_00734680[i].mF0);
        func_0022FAE8(i);
    }

    D_0029E318 = 0;
    D_0029E31C = 0;
    D_0029E320 = 0;
    D_0029DBC8 = 0;
    D_0029E324 = 10;
    D_0029E328 = 0;
    D_0029E32C = 0;
    D_0029E334 = 255.0f;
    D_0029E330 = -45.0f;
    D_00736CF0[0] = 0.0f;
    D_00736CF0[1] = 0.0f;
    D_00736CF0[2] = 0.0f;
    D_00736D00[0] = 45.0f;
    D_00736D00[1] = 45.0f;
    D_00736D00[2] = 45.0f;
    D_00736CC0[0] = 128.0f;
    D_00736CC0[1] = 128.0f;
    D_00736CC0[2] = 128.0f;
    D_00736CD0[0] = 128.0f;
    D_00736CD0[1] = 128.0f;
    D_00736CD0[2] = 128.0f;
    D_00736CE0[0] = 128.0f;
    D_00736CE0[1] = 128.0f;
    D_00736CE0[2] = 128.0f;
    func_0022FE60();
}
