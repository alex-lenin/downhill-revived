/* UNIT 0x001539F8 0x00153A30 */
typedef struct Obj { char *x0; } Obj;
extern void func_00152768(Obj *);
extern void func_001AC2E8(void *, int);
void func_001539F8(Obj *obj) {
    func_00152768(obj);
    func_001AC2E8(obj->x0 + 0x1E40, 0);
}
