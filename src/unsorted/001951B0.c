/* UNIT 0x001951B0 0x001952A0 */
typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Settings {
    char pad0[0x1B8];
    Vec3 x1B8;
} Settings;

typedef struct Elem {
    Vec3 pos[2];
    Vec3 x18[2];
    unsigned char x30[3];
    unsigned char x33;
    unsigned char x34;
    unsigned char x35;
    unsigned char x36;
    unsigned char x37;
    short x38;
    short x3A;
    int x3C;
    int x40;
    int x44;
    int x48;
} Elem;

extern Settings D_004B25A0;
extern void func_001B9860(Settings *settings, int a, float *x, float *y, float *z);

void func_001951B0(Elem *elem)
{
    int i;

    for (i = 0; i < 2; i++) {
        func_001B9860(&D_004B25A0, 0, &elem->pos[i].x, &elem->pos[i].y, &elem->pos[i].z);
        elem->x18[i] = D_004B25A0.x1B8;
        elem->x30[i] = 0;
    }
    elem->x33 = 0;
    elem->x34 = 0;
    elem->x35 = 0;
    elem->x36 = 0;
    elem->x37 = 0;
    elem->x38 = 0;
    elem->x3A = 0;
    elem->x3C = 0;
    elem->x40 = 0;
    elem->x44 = 0;
    elem->x48 = 0;
}
