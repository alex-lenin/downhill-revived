/* UNIT 0x0016D648 0x0016D8C0 */
extern char D_00308D40[];
extern char D_00412510[];

typedef struct Part Part;
struct Part {
    int x0;
    int x4;
    int x8;
    char padC[0x28 - 0xC];
    int x28;
    char pad2C[0x130 - 0x2C];
    void *x130;
    char pad134[0x230 - 0x134];
};

typedef struct {
    char pad0[0x200];
    void *x200;
    char pad204[0x214 - 0x204];
    int x214;
    char pad218[0x244 - 0x218];
    unsigned char x244;
    char pad245[0x250 - 0x245];
    Part x250;
    Part x480;
    char pad6B0[0x10];
    unsigned char x6C0;
} Obj;

typedef struct {
    char pad0[0x230];
    void *x230;
} Target130;

extern void func_00127098(void *);
extern void *func_00164170(void *, void *, void *);
extern int func_001272E0(void *, void *);
extern int func_00163D70(void *, void *);
extern void func_001224F0(void *, void *, void *, int, int);
extern void func_0016D488(void *, void *);
extern void func_0016EE40(void *);
extern void func_00126B20(void *, int);

void func_0016D648(Obj *obj) {
    char *inner;
    Part *part;
    void *t;
    void *x130;
    char *item;
    void *mgr;

    inner = (char *)obj + 0x10;
    part = (Part *)((char *)obj + 0x250);
    part->x28 = *(unsigned char *)(inner + 0x234);
    ((Part *)((char *)obj + 0x480))->x28 = *(unsigned char *)(inner + 0x234);
    if (*(int *)(inner + 0x204) == 1) {
        if (part->x0 || part->x4) {
            void *pos;
            int idx;
            int f;
            func_00127098(part);
            pos = (char *)*(void **)(inner + 0x1F0) + 0x10;
            mgr = D_00412510;
            t = func_00164170(mgr, pos, 0);
            if (t) {
                idx = func_001272E0(part, t);
                x130 = part->x130;
                item = (char *)(idx * 0xF0);
                item += 0x10;
                item = (char *)((unsigned)item + (unsigned)D_00308D40);
                f = func_00163D70(mgr, t);
                func_001224F0(part, x130, item, f, 10);
                ((Target130 *)part->x130)->x230 = part;
            }
        }
        part = &obj->x480;
        if (part->x0 || part->x4) {
            void *pos;
            int idx;
            int f;
            func_00127098(part);
            pos = (char *)obj->x200 + 0x10;
            mgr = D_00412510;
            t = func_00164170(mgr, pos, 0);
            if (t) {
                idx = func_001272E0(part, t);
                x130 = part->x130;
                item = (char *)(idx * 0xF0);
                item += 0x10;
                item = (char *)((unsigned)item + (unsigned)D_00308D40);
                f = func_00163D70(mgr, t);
                func_001224F0(part, x130, item, f, 10);
                ((Target130 *)part->x130)->x230 = part;
            }
        }
        func_0016EE40(obj);
        obj->x6C0 = 0;
    } else {
        Part *p2;
        func_0016D488(obj, part);
        if (part->x0 == 0 && part->x4 == 0) {
            switch (part->x8) {
            case 1:
            case 3:
            case 6:
            case 7:
                func_00126B20(&obj->x250, 2);
                break;
            }
        }
        p2 = &obj->x480;
        func_0016D488(obj, p2);
        if (p2->x0 == 0 && p2->x4 == 0) {
            switch (p2->x8) {
            case 1:
            case 3:
            case 6:
            case 7:
                func_00126B20(&obj->x480, 2);
                break;
            }
        }
    }
}
