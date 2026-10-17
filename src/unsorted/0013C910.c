/* UNIT 0x0013C910 0x0013C970 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0x528];
    int x528;
} Obj;

extern char D_004B2A90[];

extern void func_001C57A8(char *, u128);
extern void func_0013C400(Obj *, int);

void func_0013C910(Obj *p)
{
    if (p->x528 == 0) {
        /* packed name "S_NEXT" */
        func_001C57A8(D_004B2A90, (u128)0x5458454E5F53L);
        func_0013C400(p, 1);
    }
}
