/* UNIT 0x001BB8D0 0x001BB940 */
struct Obj { char *x0; char pad4[0x14]; int x18; };
extern void func_001ACFA0(void *, int);
extern void func_00136030(void *, int);
extern void func_001AD580(void *, int, float, float);
void func_001BB8D0(struct Obj *obj) {
    char *base = obj->x0;
    char *part = base + 0x4580;
    obj->x18 = 0;
    func_001ACFA0(part, 0);
    *(int *)(base + 0x48B0) = 1;
    func_00136030(base + 0x40C0, 3);
    func_001AD580(part, 0, 0.25f, 0.6f);
}
