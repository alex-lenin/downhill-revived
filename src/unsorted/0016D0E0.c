/* UNIT 0x0016D0E0 0x0016D2F8 .rodata=0x28AF90:0x28AFD0 */
typedef struct State { char p0[8]; int x8; char pC[4]; int x10; char p14[4]; volatile int x18; } State;
typedef struct Body { char p0[0x1F0]; char *x1F0; char p1F4[0x10]; int x204; char p208[0x2C]; unsigned char x234; } Body;
typedef struct Obj { int x0; char p4[0xC]; Body x10; char p248[8]; State x250; char p26C[0xC]; int x278; char p27C[0x204]; State x480; char p49C[0xC]; int x4A8; char p4AC[0x204]; char *x6B0; char p6B4[0xC]; unsigned char x6C0; } Obj;
extern void func_0016F420(Body *);
extern void func_0016D2F8(Obj *),func_0016D350(Obj *),func_0016D530(Obj *),func_0016D5C0(Obj *),func_0016D648(Obj *),func_0016DD40(Obj *),func_0016DE90(Obj *),func_0016DF48(Obj *),func_0016E080(Obj *),func_0016E110(Obj *),func_0016E260(Obj *),func_0016E298(Obj *),func_0016E328(Obj *),func_0016E3B8(Obj *),func_0016EE08(Obj *);

void func_0016D0E0(Obj *o)
{
    Body *body;
    State *a = &o->x250;
    State *b = &o->x480;
    float d;
    /* Always true. Reading it through a pointer hides the value from the first CSE pass
       (only gcse sees it), which gives retail's argument/copy order for &o->x10. */
    int enabled = 1;
    int *enabledp = &enabled;

    a->x10 = 0;
    b->x10 = 0;
    if (o->x0 == 14)
        return;
    func_0016F420(&o->x10);
    func_0016D5C0(o);
    if (o->x6C0 && *enabledp) {
        func_0016D648(o);
        return;
    }
    body = &o->x10;
    d = *(float *)(body->x1F0 + 0x18) - *(float *)(o->x6B0 + 0x4484);
    if (d < 0.0f) {
        a->x10 = 1;
        b->x10 = 1;
    }
    if (body->x204 != 2 && !(d < 0.0f))
        return;
    if (!a->x8 || !b->x8 || (a->x8 == 2 && a->x18 > 180) || (b->x8 == 2 && b->x18 > 180))
        func_0016D2F8(o);
    o->x278 = body->x234;
    o->x4A8 = body->x234;
    switch (o->x0) {
    case 1: func_0016D350(o); break;
    case 2: func_0016D530(o); break;
    case 3: func_0016DD40(o); break;
    case 4: func_0016DE90(o); break;
    case 5: func_0016DF48(o); break;
    case 6: case 7: func_0016E080(o); break;
    case 8: func_0016E110(o); break;
    case 9: func_0016E260(o); break;
    case 10: func_0016E298(o); break;
    case 11: func_0016E328(o); break;
    case 12: func_0016E3B8(o); break;
    case 13: func_0016EE08(o); break;
    case 0: case 14: default: break;
    }
}
