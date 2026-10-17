/* UNIT 0x0016B748 0x0016B8C8 */
extern void func_0011CF28(void *);
extern void func_00129110(void *);
extern void func_001449F8(void *);
extern void func_00147268(void *);
extern void func_001476F0(void *);
extern void func_00151EE0();
extern void func_00152088();
extern void func_00159418(void *);
extern void func_0016CA70(void *);
extern void func_001726F8(void *);
extern void func_00175100(void *);
extern void func_0017CAA8(void *);
extern void func_0017F560(void *);
extern void func_00182580(int);
extern void func_00183510(void *);
extern void func_00191748(void *);
extern void func_00193068();
extern void func_0019E9E8(void *);
extern void func_001AA1D8();
extern void func_001B1F98(void *);
extern void func_001DC4E8(void *);
extern void func_001E50C0();
extern void func_001FA9C0(void *);
extern void func_0020AB60(int);
extern void func_00229400();
extern int D_0029DA50;
struct Player { char pad0[0x3DE8]; char x3DE8[0x7BD0-0x3DE8]; };
struct World { char pad0[0x3AF0]; struct Player players[9]; char pad1[0x501A8-0x3AF0-9*0x7BD0]; int count; };
extern struct World D_002AF2C0;
extern char D_002FF6A0[];
extern char D_0032CA00[];
extern char D_0036F050[];
extern char D_00372650[];
extern char D_00373630[];
extern char D_00383D40[];
extern char D_003A6A10[];
extern char D_003AF940[];
extern char D_003B2270[];
extern char D_003B71D0[];
extern char D_003B7E00[];
struct Object { char pad[16]; };
extern struct Object D_003C6930;
extern char D_003F5460[];
extern char D_0043EEC0[];
extern char D_004BF950[];

void func_0016B748(void *arg0) {
    int index;

    func_00175100(D_003B2270);
    func_001E50C0();
    func_00147268(D_00372650);
    func_001476F0(D_00373630);
    func_0017CAA8(D_003B71D0);
    func_001DC4E8(D_004BF950);
    func_00182580(*(int *)((char *)arg0 + 0x5BF8));
    func_00193068();
    func_00129110(D_0032CA00);
    func_00159418(D_00383D40);
    func_0011CF28(D_002FF6A0);
    func_00191748(D_003F5460);
    func_001AA1D8();
    func_00229400();
    func_00151EE0();
    func_00152088();
    func_0019E9E8(D_0043EEC0);
    func_0017F560(D_003B7E00);
    func_0016CA70(D_003A6A10);
    func_001449F8(D_0036F050);
    func_001726F8(D_003AF940);
    for (index = 0; index < D_002AF2C0.count; index++) {
        func_001B1F98(&D_002AF2C0.players[index]);
        func_001FA9C0(D_002AF2C0.players[index].x3DE8);
    }
    func_0020AB60(D_0029DA50);
    func_00183510(&D_003C6930);
}
