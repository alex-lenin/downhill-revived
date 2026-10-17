/* UNIT 0x001F9BF0 0x001F9C60 .rodata=0x296FD8:0x296FE8 */
struct Obj { char pad0[0x4A4]; unsigned int x4A4; };
struct Ref { struct Obj *x0; int x4; };

extern int D_0039E7F4[];

unsigned int func_001F9BF0(struct Ref *ref) {
    if (ref->x4 == 4) {
        unsigned int values[3] = { 36, 60, 60 };
        return values[D_0039E7F4[0]] / 12;
    }
    return ref->x0->x4A4;
}
