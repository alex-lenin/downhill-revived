/* UNIT 0x00185370 0x00185488 */
typedef struct Obj {
    int state;
    char p4[0xc84];
    int xC88;
    char pC8C[4];
    int xC90;
    unsigned char xC94;
} Obj;
typedef struct Input { int x0; char p4[0x9d]; char xA1[1]; } Input;

extern int func_00185818(Obj *, Input *, int *);
extern int func_00185938(Obj *, char *);
extern int func_00185590(Obj *, Input *, unsigned char *, int *);
extern char D_0028C648[];

int func_00185370(Obj *o, Input *in) {
    unsigned char val;

    if (in->x0 != 0) {
        return 1;
    }
    o->xC94 = 0;
    switch (o->state) {
    case 0:
        if (func_00185818(o, in, &o->xC90)) {
            o->xC90 = 0;
            o->state++;
        }
        break;
    case 1:
        if (o->xC88 < 5) {
            return 1;
        }
        if (!func_00185938(o, D_0028C648) || !func_00185938(o, in->xA1)) {
            return 1;
        }
        o->state++;
    case 2:
        val = 0;
        if (func_00185590(o, in, &val, &o->xC90)) {
            o->xC90 = 0;
            o->xC94 = val;
            return 1;
        }
        break;
    }
    return in->x0 != 0;
}
