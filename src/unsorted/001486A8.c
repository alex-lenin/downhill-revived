/* UNIT 0x001486A8 0x00148700 */
struct Obj { char pad0[0x2b8]; int x2b8; };
extern void func_00148860(struct Obj *);
extern void func_00223348(void *, struct Obj *);
void func_001486A8(struct Obj *obj) {
    int state;
    state = obj->x2b8;
    if (state == 1 || state == 8 || state == 1) func_00148860(obj);
    func_00223348((char *)obj + 0xe0, obj);
}
