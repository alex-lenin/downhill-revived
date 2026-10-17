/* UNIT 0x0012BEF0 0x0012BF88 */
struct Part { char pad0[0x34]; void *x34; void *x38; char pad3C[0x158-0x3C]; void *x158; };
struct Target { char pad[0xBC]; int xBC; };
struct Obj { char pad0[0x40C0]; struct Part x40C0; char pad421C[0x792C-0x421C]; struct Target *x792C; char pad7930[0x7A44-0x7930]; int x7A44; int x7A48; };
extern void func_00221F20(void *);
extern void func_00135178(struct Part *);
extern void func_00135100(struct Part *);
extern int func_00161960(struct Target *);
extern int func_00239038(void);
extern int D_004B26B8[];
void func_0012BEF0(struct Obj *obj, struct Target *target) {
    struct Part *part;
    obj->x792C = target;
    if (target) {
        part = &obj->x40C0;
        func_00221F20((char *)target + 0x40);
        part->x34 = (char *)obj + 0x4270;
        part->x38 = (char *)obj + 0x4280;
        func_00135178(part);
        part->x158 = obj;
        func_00135100(part);
        obj->x792C->xBC = 1;
        if (!D_004B26B8[0]) {
            obj->x7A44 = func_00161960(obj->x792C);
            obj->x7A48 = func_00239038();
        } else {
            obj->x7A44 = 0;
            obj->x7A48 = 0;
        }
    }
}
