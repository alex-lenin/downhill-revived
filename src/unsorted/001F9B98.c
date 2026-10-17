/* UNIT 0x001F9B98 0x001F9BF0 .rodata=0x296FC8:0x296FD8 */
struct Obj { char *x0; int x4; };
extern char D_00275D20[], D_00275D48[], D_00275D88[];
extern int D_0039E7F4[];
char *func_001F9B98(struct Obj *arg0) {
    if (arg0->x4 == 4) {
        int index = D_0039E7F4[0];
        char *values[3] = { D_00275D20, D_00275D48, D_00275D88 };
        return values[index];
    }
    return arg0->x0;
}


