/* UNIT 0x001743F0 0x00174478 .sdata=0x29C8EC:0x29C8F0 */
typedef struct Obj {
    char pad[0x5C];
    int count;
} Obj;

typedef struct Entry {
    char pad[0x14];
    short flags;
} Entry;

extern Entry *func_00163D40(Obj *, int);

int D_0029C8EC = -1;

Entry *func_001743F0(Obj *o) {
    int count = o->count;
    long x;

    D_0029C8EC++;
    while (count) {
        x = func_00163D40(o, D_0029C8EC)->flags;
        if ((((unsigned long)x >> 2) & 1L) == 1L) {
            break;
        }
        D_0029C8EC++;
        if (D_0029C8EC >= o->count) {
            D_0029C8EC = 0;
        }
        count--;
    }
    return func_00163D40(o, D_0029C8EC);
}
