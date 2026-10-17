/* UNIT 0x001517D0 0x00151AB0 */
typedef struct Obj {
    char pad0[0x500];
    char x500[0x500];
    int *xA00;
    int *xA04;
    volatile int *xA08; /* volatile: the store keeps its own result copy, which sets the arg order before func_002103D8 */
    char padA0C[0xA30 - 0xA0C];
    int xA30;
    char padA34[0xA3C - 0xA34];
    int xA3C;
    int xA40;
    char *xA44[15];
    char *xA80[15];
} Obj;

extern int D_004B2828[];
extern char D_00513B00[];
extern void func_0020FB80(int, int, int, int, int);
extern int func_0020F860(int, char *);
extern void func_0023D650(int, int *, int *);
extern void func_00151AD0(void *, void *, int, int, int, int);
extern int func_001FD238(float);
extern void func_002103D8(void *, int, void *);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);

void func_001517D0(Obj *obj)
{
    int w1;
    int w2;
    int width;
    int height;
    int cx;
    int cy;
    void *packet;
    float fade;
    float level;
    int *alpha;

    if (obj->xA40 < obj->xA30) {
        if (D_004B2828[0] >= 2)
            func_0020FB80(0, 320, 224, -123, -32);
        else
            func_0020FB80(0, 384, 288, -123, -32);
        w1 = 0;
        if (obj->xA44[obj->xA40] != 0)
            w1 = func_0020F860(0, obj->xA44[obj->xA40]) >> 4;
        w2 = 0;
        if (obj->xA80[obj->xA40] != 0)
            w2 = func_0020F860(0, obj->xA80[obj->xA40]) >> 4;
        packet = obj->x500;
        if (w1 >= w2)
            w2 = w1;
        func_0023D650(0, &width, &height);
        cx = width / 2;
        cy = height - height / 4;
        func_00151AD0(obj, packet, cx - (w2 >> 1) - 6, cy - 2, w2 + 14, 40);
        if (obj->xA3C >= 241)
            fade = (float)(300 - obj->xA3C) * 0.016666668f;
        else if (obj->xA3C < 60)
            fade = (float)obj->xA3C * 0.016666668f;
        else
            fade = 1.0f;
        level = fade * 127.0f;
        alpha = obj->xA00;
        *alpha = func_001FD238(level);
        *obj->xA04 = func_001FD238(level);
        *obj->xA08 = func_001FD238(fade * 60.0f);
        func_002103D8(D_00513B00, 0, (char *)obj + 0x500);
        func_0020F588(0, 0, 0x63, 0x68, 0xCE, *obj->xA00);
        func_0020F9F0(0, 0, cx, cy, obj->xA44[obj->xA40]);
        func_0020F9F0(0, 0, cx, cy + 15, obj->xA80[obj->xA40]);
        if (--obj->xA3C <= 0) {
            if (obj->xA40 < obj->xA30) {
                obj->xA40++;
                obj->xA3C = 300;
            }
        }
    }
}
