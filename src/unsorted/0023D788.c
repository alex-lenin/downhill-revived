/* UNIT 0x0023D788 0x0023D7C0 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union { u128 whole; unsigned int words[4]; } Vec;
typedef struct { Vec x0; char pad10[0x110]; } Obj;
extern Obj D_0075B3F0[];
void func_0023D788(int index) {
    Obj *p = &D_0075B3F0[index];
    p->x0.whole=0;
    p->x0.words[0]=0x60000011;
    p->x0.words[3]=0x6C110009;
}
