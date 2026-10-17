/* UNIT 0x00169920 0x00169BD0 */
typedef struct Rider { char pad0[0x796C]; int x796C; } Rider;
typedef struct Event { char pad0[0x33]; unsigned char x33; char tail[0x1C]; } Event;
typedef struct Record { Event events[21]; char tail[4]; } Record;
typedef struct Save { Record rec[18]; char pad7668[0x12110 - 0x7668]; int x12110; char pad12114[0x6C]; int x12180; int x12184; int x12188; } Save;
typedef struct GameState { char pad0[0xA0C]; unsigned char xA0C[23]; char padA23; int xA24; int xA28; int xA2C; int xA30; int xA34; int xA38; } GameState;
typedef struct Triple { int x0; int x4; int x8; } Triple;
typedef struct Font { long x[20]; } Font;
extern struct { char pad[0x50130]; Rider *x50130; } D_002AF2C0;
extern Save D_004225A0;
extern GameState D_00380D00;
extern char D_00380770[];
extern int D_004B26AC[];
extern Font D_004C8508[];
extern Triple D_0028AEE8;
extern Triple D_0028AEF8;
extern void func_00150E30(void *, int, int);
extern void func_00194290(void *, int);
extern void func_001F9C78(void *, void *);
void func_00169920(void) {
    int index; int v12188; int flags; int i; unsigned char *p; Triple a; Triple b;
    Font *pfont; Font *dst; Font *src; int mode; int base;
    flags = D_004225A0.x12110;
    v12188 = D_004225A0.x12188;
    mode = D_004B26AC[0] * 0x50;
    index = (D_002AF2C0.x50130->x796C >> 5) - 1;
    base = index * 0x694 + (int)&D_004225A0;
    ((Event *)(mode + base))->x33 = 1;
    for (i = 0; i < 23; i++) { D_00380D00.xA0C[i] = 0; }
    D_00380D00.xA24 = 0; D_00380D00.xA28 = 0; D_00380D00.xA2C = 0;
    D_00380D00.xA34 = 0;
    D_00380D00.xA38 = 0;
    func_00150E30(D_00380770, (D_002AF2C0.x50130->x796C >> 5) - 1, 0);
    if (((flags >> 8) & 1) == 0) {
        int mask; int idx2;
        D_00380D00.xA28 |= 0x100;
        func_00194290(&D_004225A0, 8);
        D_00380D00.xA34 = 1;
        a = D_0028AEE8;
        pfont = &D_004C8508[0];
        func_001F9C78(pfont, &a);
        D_00380D00.xA38 = 1;
        dst = pfont;
        src = pfont;
        *dst = *src;
        b = D_0028AEF8;
        func_001F9C78(pfont, &b);
        idx2 = (D_002AF2C0.x50130->x796C >> 5) - 1;
        if (((D_004225A0.x12180 >> idx2) & 1) == 0) {
            mask = 1 << idx2;
            D_004225A0.x12180 |= mask;
            D_00380D00.xA24 |= mask;
        }
    }
    if ((v12188 & 0x200) == 0) { D_00380D00.xA2C |= 0x200; D_004225A0.x12188 |= 0x200; }
    if ((v12188 & 0x2000) == 0) { D_00380D00.xA2C |= 0x2000; D_004225A0.x12188 |= 0x2000; }
}
