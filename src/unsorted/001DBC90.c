/* UNIT 0x001DBC90 0x001DBCC0 */
typedef struct Obj { char pad[8]; float x8; } Obj;
extern void func_001DBCC0(Obj *);
void func_001DBC90(Obj *obj) {
    func_001DBCC0(obj);
    obj->x8 = 150.0f;
}
