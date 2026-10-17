/* UNIT 0x0013E9C0 0x0013EC38 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Entry {
    char pad0[0xC];
    short xC;
    char padE[2];
} Entry;

typedef struct { int x0; int x4; int x8; } Tbl;

typedef struct { char pad[0x40]; short x40; short pad42; void (*x44)(void *, int); } Vtbl;

typedef struct Obj {
    char pad0[0x10];
    Entry entries[66];
    char pad430[0x43C - 0x430];
    short x43C;
    char pad43E[0x4F8 - 0x43E];
    Vtbl *x4F8;
    char pad4FC[0x5AC - 0x4FC];
    int x5AC;
    int x5B0;
    int x5B4;
    unsigned char x5B8;
} Obj;

typedef struct {
    char pad[0x10000];
    char x10000;
    char pad10001[0x12208 - 0x10001];
    int x12208;
} Global;

extern Global D_004225A0;
extern char D_004B2A90[];
extern Tbl *D_00365D74[];

extern void func_001C57A8(void *, u128);
extern int func_0013A258(void);
extern int func_00194008(Global *, int);
extern int func_00194050(Global *, int);
extern int func_00194098(Global *, int);
extern void func_001941B8(Global *, int);
extern void func_00194200(Global *, int);
extern void func_00194248(Global *, int);
extern int func_001FA300(int, int);
extern int func_0013A088();
extern void func_0013E760(Obj *, int);
extern void func_0013E8F0(Obj *, int);

static __inline__ int IsType(Entry *e, int t) { return e->xC == t; }

void func_0013E9C0(Obj *obj, int index)
{
    int t;
    int off = index * 16;

    if (IsType(&obj->entries[index], 0x1A) || ((t = obj->x43C) ^ 0x60) == 0 || ((t ^ 0x66) == 0)) {
        Vtbl *vtab = obj->x4F8;
        vtab->x44((char *)obj + vtab->x40, off);
        /* packed name "S_BACK" */
        func_001C57A8(D_004B2A90, (u128)0x4B4341425F53L);
        return;
    } else if (IsType(&obj->entries[index], 0x19)) {
        int half = 0;
        Tbl *e = &D_00365D74[0][func_0013A258()];
        int s2 = e->x0;
        int v;
        int extra;

        switch (obj->x5AC) {
        case 1:
            half = func_00194008(&D_004225A0, s2) >= 5;
            e->x0 = obj->x5B0;
            if (obj->x5B8 != 0)
                func_001941B8(&D_004225A0, obj->x5B0);
            break;
        case 2:
            s2 = e->x4;
            half = func_00194050(&D_004225A0, s2) >= 5;
            e->x4 = obj->x5B0;
            if (obj->x5B8 != 0)
                func_00194200(&D_004225A0, obj->x5B0);
            break;
        case 3:
            s2 = e->x8;
            half = func_00194098(&D_004225A0, s2) >= 5;
            e->x8 = obj->x5B0;
            if (obj->x5B8 != 0)
                func_00194248(&D_004225A0, obj->x5B0);
            break;
        }
        v = func_001FA300(obj->x5AC, s2) / 2;
        extra = half ? 0 : v;
        func_0013A088(0, D_004225A0.x12208 = (D_004225A0.x12208 / 20 - obj->x5B4 + extra) * 20);
        if (half != 0)
            func_0013E8F0(obj, s2);
        else
            func_0013E760(obj, s2);
    }
}
