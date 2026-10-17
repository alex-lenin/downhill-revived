/* UNIT 0x001E68C0 0x001E6918 */
struct Inner { char pad[0x7a5c]; int x7a5c; };
struct Obj { int x0; int x4; char pad8[0x38]; struct Inner *x40; char pad44[0x10a8]; int x10ec; };
extern int func_001E6970(struct Obj *, int);
int func_001E68C0(struct Obj *arg0) {
    int state = arg0->x40->x7a5c;
    int result = 0;
    if (state == 1) result = func_001E6970(arg0, 0);
    else if (state == 2) result = func_001E6970(arg0, 0);
    else { int value = arg0->x10ec; if (value) { result = value; arg0->x4 = value; } }
    return result;
}
