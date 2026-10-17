/* UNIT 0x001FAD18 0x001FAD78 */
typedef struct Obj {
    char pad0[0x11C8];
    int x11C8;
    int x11CC;
    int x11D0;
    char pad1[0xBC];
    int x1290;
    int x1294;
    int x1298;
    char pad2[0x149];
    unsigned char x13E5;
    char pad3;
    unsigned char x13E7;
    char pad4[0x302C];
    unsigned char x4414;
    char pad5[0x7F];
    unsigned char x4494;
} Obj;

/* Writing result through an inlined pointer keeps it addressable during the first jump
   pass, so gcc does not turn "if (x != 2) result = 1" into a conditional move/store-flag. */
static __inline__ void set_true(int *r) { *r = 1; }

int func_001FAD18(Obj **arg) {
    Obj *p = *arg;
    int result = 0;
    int a, b;

    a = p->x13E5 ? p->x1290 : p->x11C8;
    if (a == 0) {
        b = p->x13E7 ? p->x1298 : p->x11D0;
        if (b == 0) goto end;
    }
    if (p->x4494 != 2 && p->x4414 != 2) {
        set_true(&result);
    }
end:
    return result;
}
