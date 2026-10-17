/* UNIT 0x00185A30 0x00185AE0 .sbss=0x29DE04:0x29DE08 */
struct Obj { int x0, x4, x8; char xC; };
struct Input { int x0, x4, x8, xC, x10; };
int D_0029DE04 __attribute__((section(".sbss"))) = 0;
extern char D_003CD500[];
extern int func_00246960(int, int, int, int, int, void *);
extern int func_002466A0(int, int *, int *);

int func_00185A30(struct Obj *obj, struct Input *input)
{
    int id;

    if (input->x0)
        return 1;
    switch (obj->x0) {
    case 0:
        func_00246960(input->x10, input->xC, obj->x8, 0, 1, D_003CD500);
        obj->x0++;
    case 1:
        id = 13;
        if (func_002466A0(1, &id, &D_0029DE04)) {
            obj->xC = D_0029DE04 > 0;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
