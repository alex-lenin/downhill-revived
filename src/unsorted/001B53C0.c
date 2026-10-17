/* UNIT 0x001B53C0 0x001B55F8 .sdata=0x29D408:0x29D410 */
typedef struct Obj {
    char pad0[0x40];
    int x40;
    char pad44[4];
    int x48;
    int x4C;
    int x50;
    unsigned char x54;
} Obj;

typedef struct Entry {
    int x0;
    int x4;
    unsigned char x8;
    char pad9[3];
} Entry;

int D_0029D408 = 0;
int D_0029D40C = 0;

extern char D_003F52D0[];
extern int D_0029DAFC;
extern int D_0029DB00;
extern unsigned int D_0029E10C;
extern Entry D_0028E960[];

extern void func_00190358(void *);
extern unsigned int func_00190D60(void *, int, unsigned int);
extern void func_001B56D0(Obj *);
extern void func_001B57F8(Obj *);
extern void func_0020BDA0(int);
extern void func_0021F0A0(int, int, int);
extern int func_00223BB0(int, int);
extern void func_00223C60(int);
extern int func_002433A8(int, int);
extern void func_0025A960(int);
extern void *memcpy(void *, const void *, unsigned int);

void func_001B53C0(Obj *o) {
    Entry list[27];
    int flag;

    func_00190358(D_003F52D0);
    if (D_0029DAFC != 0) {
        flag = 0;
        if (func_00190D60(D_003F52D0 + 0x64, 8, 1) != 0 ||
            func_00190D60(D_003F52D0 + 0x64, 4, 1) != 0) {
            flag = 1;
        }
        o->x54 = flag;
        if (D_0029DAFC != 0) {
            D_0029D408 = 1;
        }
    }
    if (D_0029D408 != 0 && D_0029DB00 != 0) {
        if (o->x50 != 0) {
            D_0029D408 = 0;
            o->x4C = 1;
        } else {
            D_0029D408 = 0;
            func_001B57F8(o);
            return;
        }
    }
    func_001B56D0(o);
    memcpy(list, D_0028E960, sizeof(list));
    if (o->x4C != 0) {
        Entry *e = &list[o->x50];
        func_00223BB0(e->x0, e->x8);
        o->x4C = 0;
        o->x50 = list[o->x50].x4;
    }
    if (D_0029DAFC != 0) {
        func_00223C60(o->x54 ^ 1);
        o->x54 = 0;
    }
    func_0025A960(0);
    func_002433A8(0, 0);
    if (D_0029E10C < (unsigned int)o->x40) {
        goto done;
    }
    func_0020BDA0(2);
done:
    func_0021F0A0(o->x48, 0, 1);
    func_002433A8(0, 0);
}
