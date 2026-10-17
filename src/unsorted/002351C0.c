/* UNIT 0x002351C0 0x00235218 */
typedef struct {
    void *x0;
    int x4;
} Obj;

extern void func_001FD3B8(void *p);
extern void func_001FD340(Obj *p);

void func_002351C0(Obj *p, int flags)
{
    if (p->x0) {
        func_001FD3B8(p->x0);
    }
    p->x0 = 0;
    p->x4 = 0;
    if (flags & 1) {
        func_001FD340(p);
    }
}
