/* UNIT 0x001FA878 0x001FA9C0 */
typedef struct Target {
    char pad0[0x50];
    int x50;
    char pad54[0xCC - 0x54];
    int xCC;
} Target;

typedef struct Obj {
    Target *x0;
    Target *x4;
    Target *x8;
    Target *xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
} Obj;

extern char D_00297098[];
extern char D_002970C0[];
extern char D_002970E8[];
extern char D_00297110[];
extern char D_004C8508[];

extern void func_00254D08(char *);
extern int func_001F9E50(char *, int *);
extern int func_001FA7D8(char *, int);
extern void func_00218498(int, int);

void func_001FA878(Obj *obj) {
    if (obj->x30 >= 0x23 && obj->x30 <= 0x25) {
        return;
    }
    if (obj->x0 != 0 && obj->x10 != 0) {
        obj->x1C = obj->x0->xCC;
        obj->x0->xCC = obj->x10;
    } else {
        func_00254D08(D_00297098);
    }
    if (obj->x4 != 0 && obj->x14 != 0) {
        obj->x20 = obj->x4->xCC;
        obj->x4->xCC = obj->x14;
    } else {
        func_00254D08(D_002970C0);
    }
    if (obj->x8 != 0 && obj->x18 != 0) {
        obj->x24 = obj->x8->x50;
        obj->x8->x50 = obj->x18;
    } else {
        func_00254D08(D_002970E8);
    }
    if (obj->xC != 0 && obj->x18 != 0) {
        obj->x28 = obj->xC->x50;
        obj->xC->x50 = obj->x18;
    } else {
        func_00254D08(D_00297110);
    }
    if (obj->x2C != 0) {
        if (func_001F9E50(D_004C8508, &obj->x30) != 0) {
            func_00218498(obj->x2C, func_001FA7D8(D_004C8508, obj->x30));
        } else {
            func_00218498(obj->x2C, 0);
        }
    }
}
