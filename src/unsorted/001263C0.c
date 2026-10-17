/* UNIT 0x001263C0 0x00126400 .rodata=0x288DF0:0x288E20 .sdata=0x29C218:0x29C220 */
struct Obj { char pad0[8]; int x8; char padc[0x15c]; int x168; };
asm(".globl D_0029C218");
int func_001263C0(struct Obj *obj, int arg, int value) {
    /* state descriptor {magic, dispatch entry}: func_00126400 reads it */
    static void *state_desc[2] __asm__("D_0029C218") = { (void *)0xFFFF0000, &&dispatch };
    obj->x168 = value;
    for (;;) {
    dispatch:
        switch (obj->x8) {
            case 3: case 4: case 5: return 1;
            case 0: case 1: case 2: case 6: case 7: case 8:
            default: return 0;
        }
    }
}
