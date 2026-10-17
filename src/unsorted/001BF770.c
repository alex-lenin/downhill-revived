/* UNIT 0x001BF770 0x001BF7E8 */
typedef struct { int x0; int x4, x8, xC; } Obj;
typedef struct { char pad0[0x5350]; char x5350[16]; } Global;
extern Global D_00398C40;
extern int func_001BC618(void *, int);
int func_001BF770(Obj *p) {
    Global *base = &D_00398C40;
    void *part = (char *)base + 0x5350;
    int result = func_001BC618(part, p->xC);
    if (result == 5 && func_001BC618(part, p->x0) == result) return 0;
    return 1;
}
