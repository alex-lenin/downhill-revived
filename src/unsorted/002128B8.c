/* UNIT 0x002128B8 0x002128F8 */
typedef struct Obj {
    char pad0[0x28];
    int x28;
    char pad2C[0x10];
    int x3C;
    char pad40[0x10];
    char x50[0x20];
    char x70[1];
} Obj;
extern void func_00211D68(void *);
extern void func_00212738(void *);
Obj *func_002128B8(Obj *obj) {
    obj->x28 = 0;
    obj->x3C = 0;
    func_00211D68(obj->x50);
    func_00212738(obj->x70);
    return obj;
}
