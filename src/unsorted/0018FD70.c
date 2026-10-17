/* UNIT 0x0018FD70 0x0018FDB0 */
typedef struct Obj {
    char pad0[0xC];
    void *xC;
} Obj;

extern void func_00218498(void *p, int a);

void func_0018FD70(Obj *obj, void *p)
{
    func_00218498(p, 0);
    obj->xC = p;
}
