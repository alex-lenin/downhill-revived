/* UNIT 0x001BA080 0x001BA368 */
typedef struct Alloc {
    long x0;
    float x8;
    float xC;
} Alloc;

typedef struct Sub {
    int x0;
    char pad4[0x1C];
    Alloc *x20;
    char pad24[0xC];
} Sub;

typedef struct Obj {
    char pad0[0xA0C];
    int entries[1][10];
    char padA34[0x1380 - 0xA34];
    int x1380;
    char pad1384[0x1690 - 0x1384];
    Sub x1690[1][10];
    char pad1870[0xAC90 - 0x1870];
    int xAC90[1];
    char padAC94[0xAF38 - 0xAC94];
    int xAF38;
    char padAF3C[0xB190 - 0xAF3C];
    int xB190[4];
} Obj;

extern void *func_00209108(void);
extern void *func_00208FB8(void);
extern void func_00218278(void *, int);
extern const float D_0029D49C[];

void func_001BA080(Obj *obj, int id, int val)
{
    int lo;
    int hi;
    Sub *s2;

    if ((unsigned)(id - 631) < 4U) {
        int x = id + 9000;
        if (x == 9631) {
            obj->xB190[0] = val;
            return;
        } else if (x == 9632) {
            obj->xB190[1] = val;
            return;
        } else if (x == 9633) {
            obj->xB190[2] = val;
            return;
        } else if (x == 9634) {
            obj->xB190[3] = val;
            return;
        }
        return;
    }
    lo = id / 10;
    hi = id % 10;
    obj->xAC90[lo] += 1;
    obj->xAF38 += 1;
    obj->entries[lo][hi] = val;
    obj->x1690[lo][hi].x0 = id;
    s2 = &obj->x1690[lo][hi];
    if ((unsigned)(lo - 4) < 14U && (unsigned)(hi - 2) < 2U) {
        s2->x20 = func_00209108();
        func_00218278(s2->x20, obj->entries[lo][hi]);
        ((char *)s2->x20)[4] = 1;
        return;
    }
    if (lo == 60 && hi == 5) {
        s2->x20 = func_00209108();
        func_00218278(s2->x20, obj->x1380);
        ((char *)s2->x20)[4] = 1;
        s2->x20->x0 &= 0xEFFFFFFFFFFFFFFFL;
        return;
    }
    if ((lo == 0 && (hi == 5 || hi == 7)) || (lo == 1 && hi == 1) || (lo == 60 && (hi == 1 || hi == 7))) {
        s2->x20 = func_00208FB8();
        func_00218278(s2->x20, obj->entries[lo][hi]);
        ((char *)s2->x20)[4] = 1;
        if ((lo == 0 || lo == 60) && hi == 7) {
            s2->x20->x8 = D_0029D49C[0];
            s2->x20->xC = D_0029D49C[0];
            s2->x20->x0 &= 0xEFFFFFFFFFFFFFFFL;
        }
    }
}
