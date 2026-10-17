/* UNIT 0x0019A3C0 0x0019A4D8 */
typedef struct Entry {
    char p0[0xc];
    short xC;
    char pE[2];
} Entry;
typedef struct Obj {
    char p0[0x10];
    Entry entries[1];
} Obj;
typedef struct State {
    char p0[0x11];
    unsigned char x11;
    unsigned char x12;
    unsigned char x13;
    char p14[0x38];
    int x4C;
} State;
typedef struct Preset {
    char p0[0xc];
} Preset;

extern State D_0043DCC0;
extern Preset D_0026F938[];
extern char D_0043CD50[];
extern char D_003F52D0[];
extern void func_0019BB30(State *, Preset *);
extern void func_0019A768(char *);
extern void func_00190660(char *, char *);

static inline int IsKind(Entry *e, int kind) {
    return e->xC == kind;
}

void func_0019A3C0(Entry *o, int index) {
    if (index < 4) {
        func_0019BB30(&D_0043DCC0, &D_0026F938[index]);
        D_0043DCC0.x11 = 1;
        D_0043DCC0.x12 = index;
        D_0043DCC0.x4C = 0;
    } else if (IsKind(o + index + 1, 0x81)) {
        D_0043DCC0.x4C = 1;
    } else if (IsKind(o + index + 1, 0x82)) {
        func_0019A768(D_0043CD50);
        func_00190660(D_003F52D0, D_0043CD50);
    }
    if (IsKind(o + index + 1, 0x80)) {
        D_0043DCC0.x13 = 0;
    } else {
        D_0043DCC0.x13 = 1;
    }
}
