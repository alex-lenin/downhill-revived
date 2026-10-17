/* UNIT 0x001B55F8 0x001B56D0 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char p0[3];
    unsigned char x3;
    char p4[0x1c];
    int x20;
    char p24[0x20];
    int x44;
    int x48;
} Obj;

typedef union Qword {
    u128 q;
    struct {
        unsigned int lo;
        unsigned int hi;
        unsigned long d;
    } w;
    unsigned char b;
} Qword;

typedef struct Packet {
    Qword q0;
    Qword q1;
    Qword q2;
} Packet;

extern int D_0029E110;
extern Packet D_004B2030;
extern int func_0023D5F0(int);
extern void func_0025A960(int);
extern void func_002433A8(int, int);
extern void func_0021F0A0(int, int, int);
extern int func_00243CE8(int);

void func_001B55F8(Obj *o) {
    if (o->x3 != 0) {
        o->x48 = func_0023D5F0(o->x20);
        func_0025A960(0);
        func_002433A8(0, 0);
        func_0021F0A0(o->x48, 0, 1);
        func_002433A8(0, 0);
        D_0029E110 = !func_00243CE8(0);
        o->x44 = !D_0029E110 ? 3 : 2;
        D_004B2030.q0.q = 0;
        D_004B2030.q1.q = 0;
        D_004B2030.q2.q = 0;
        D_004B2030.q0.w.lo = 0x70000002;
        D_004B2030.q1.w.d = 0xE;
        D_004B2030.q1.w.hi = 0x10000000;
        D_004B2030.q1.w.lo = 0x8001;
        D_004B2030.q2.w.d = 0x22;
        D_004B2030.q2.b = o->x44;
    }
}
