/* UNIT 0x001E3DC0 0x001E3FC0 */
struct Obj { char pad[0x500]; int x500; int x504; };
struct G1 { char pad[0x20]; int x20; };
struct G2 { char pad[0x500]; int x500; char pad504[20]; unsigned char x518; };
struct Rider { char pad[0x661C]; float x661C; };
struct G3 { char pad[0x50130]; struct Rider *rider; };
struct Target { char pad[0xA10]; };

extern struct G1 D_003F52D0;
extern struct G2 D_004C5640;
extern struct G3 D_002AF2C0;
extern struct Target D_004C4C30;
extern char D_00294718[], D_00294728[], D_00294748[], D_00294760[], D_0029D728[];

extern void func_0023D650(int, int *, int *);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, const char *);
extern void func_001E3AA0(struct Target *, int, int, int, int, int);
extern void func_00254D68(char *, const char *, ...);
extern void func_0018DC20(struct Obj *);

void func_001E3DC0(struct Obj *p) {
    char buf[48];
    int width, height, handle, half;

    handle = D_003F52D0.x20;
    func_0023D650(handle, &width, &height);
    half = width / 2;
    if (p->x500 < 190) {
        p->x500 += 3;
    } else {
        p->x500 = 0;
        p->x504++;
    }
    func_0020FB80(0, 384, 288, -123, -32);
    func_0020F588(handle, 0, 221, 219, 10, p->x504 == 0 ? p->x500 : 190);
    if (D_004C5640.x518) {
        func_0020F9F0(0, handle, half, 10, D_00294718);
        func_0020F9F0(0, handle, half, 24, D_00294728);
    } else {
        func_0020F9F0(0, handle, half, 10, D_0029D728);
        func_0020F9F0(0, handle, half, 24, D_00294748);
    }
    if (p->x504 > 0) {
        struct Target *target;
        int other;
        target = &D_004C4C30;
        other = D_003F52D0.x20;
        func_001E3AA0(target, other, 50, 0, 0, p->x504 == 1 ? p->x500 : 190);
        if (D_004C5640.x500 != 0 && D_004C5640.x500 < 11) {
            func_00254D68(buf, D_00294760, (int)D_002AF2C0.rider->x661C);
            func_0020F9F0(0, handle, half, 80, buf);
        }
    }
    func_0018DC20(p);
}
