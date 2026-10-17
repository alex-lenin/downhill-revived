/* UNIT 0x00141CD8 0x00141D10 */
typedef struct Obj { char pad[0x168]; int x168; } Obj;
extern void func_00141CA8(Obj *);
void func_00141CD8(Obj *obj, int value) {
    func_00141CA8(obj);
    obj->x168 = value;
}
