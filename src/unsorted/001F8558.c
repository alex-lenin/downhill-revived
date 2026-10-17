/* UNIT 0x001F8558 0x001F8628 */
typedef struct Handle {
    int x0;
    int x4;
    int x8;
    int xC;
} Handle;

typedef struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
    int x508;
    int x50C;
} Obj;

extern int D_004B2830[];
extern void func_00203590(Handle *, int, int, int);
extern void func_00202E38(Handle *, int, int, float);

int func_001F8558(Obj *obj)
{
    Handle handle;

    obj->x504 = 50;
    obj->x508 = 1;
    obj->x500 = 0;
    obj->x50C = 60;
    handle.x8 = 0;
    func_00203590(&handle, D_004B2830[0] + 0x10, 0, 0xF8);
    func_00202E38(&handle, 1, 1, 300.0f);
    func_00203590(&handle, 0x25EE, 0, 0xF8);
    func_00202E38(&handle, 1, 1, 300.0f);
    func_00203590(&handle, 0x384, 0, 0xF8);
    func_00202E38(&handle, 1, 1, 300.0f);
    return 0;
}
