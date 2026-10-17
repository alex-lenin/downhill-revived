/* UNIT 0x0010D7B0 0x0010D838 */
struct Rider { char pad[0x4098]; unsigned long flags; };
struct Obj { struct Rider *x0; int x4; char *x8; };
extern int D_0039E820[];
extern float func_001385A8(void *);
extern void func_0010D838(struct Obj *);
void func_0010D7B0(struct Obj *obj) {
    if (D_0039E820[0] && !((unsigned char)(obj->x0->flags >> 2) & 1)) {
        if (obj->x8 && func_001385A8(obj->x8 + 0x40C0) > 0.2f)
            func_0010D838(obj);
    }
}
