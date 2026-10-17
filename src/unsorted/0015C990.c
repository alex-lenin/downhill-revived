/* UNIT 0x0015C990 0x0015CA50 */
typedef struct { char pad[0x10C]; int x10C; char pad110[0x24C]; char x35C; char pad35D[0x17]; char x374[4]; } State;
typedef struct { char pad[0x56]; char x56; char pad57[0xD6]; char x12D; } Other;
typedef struct { int pad[2]; char x8[4]; } Small;
extern Small D_00398C40;
extern Other D_003F52D0;
extern State D_004B25A0;
extern char D_004C5640[];
extern void func_001B8B10(void *);
extern int func_001DEDE0(void *);
extern void func_001DF638(void *);
extern void func_001C5AC8(void *);
extern void func_001E4120(void *);
extern void func_001C6628(void *);
static inline void *get_x8(Small *obj) { return obj->x8; }
void func_0015C990(void) {
    func_001B8B10(&D_004B25A0);
    D_004B25A0.x35C = 1;
    D_003F52D0.x56 = 0;
    D_003F52D0.x12D = 1;
    if (D_004B25A0.x10C != 17 && D_004B25A0.x10C != 11) {
        void *p = get_x8(&D_00398C40);
        if (func_001DEDE0(p)) func_001DF638(p);
        func_001C5AC8(D_004B25A0.x374);
    }
    {
        State *p = &D_004B25A0;
        if (p->x10C == 11) func_001E4120(D_004C5640);
        func_001C6628(p->x374);
    }
}
