/* UNIT 0x00182838 0x00182920 */
typedef struct Obj {
    char pad0[0x4];
    int x4;
    int x8;
    char padC[0xA4 - 0xC];
    int xA4;
    float xA8;
} Obj;

extern int D_0029D9D4;
extern char D_004B25A0[];
extern char D_004D0380[];
extern char D_004D1A18[];
extern char D_004D30B0[];
extern char D_004D4748[];
extern void func_001829C0(Obj *, int, int);
extern void func_00182A88(Obj *, int);
extern void func_00182D78(Obj *, int);
extern int func_001BA6B8(void *);
extern void func_0020D0A0(void *);
extern void func_0020F058(int, int);
extern void func_0020F240(void);
extern void func_002215D8(float);

void func_00182838(Obj *obj, int a1, int a2, int a3, float f) {
    int v = D_0029D9D4;

    obj->xA8 = f;
    obj->x8 = 0;
    obj->x4 = 0;
    obj->xA4 = a2;
    func_0020D0A0(D_004D0380);
    func_0020D0A0(D_004D1A18);
    func_0020D0A0(D_004D30B0);
    func_0020D0A0(D_004D4748);
    func_0020F058(func_001BA6B8(D_004B25A0), 1);
    func_001829C0(obj, a1, v);
    func_00182A88(obj, a1);
    func_002215D8(f);
    func_00182D78(obj, a3);
    func_0020F240();
}
