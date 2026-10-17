/* UNIT 0x001BBEF8 0x001BBF30 */
typedef struct Obj { char pad0[0x5FC]; void *x5FC; char pad600[0xC]; int x60C; } Obj;
extern char D_00287F68[];
extern void func_00235420(Obj *);
Obj *func_001BBEF8(Obj *obj) {
    func_00235420(obj);
    obj->x5FC = D_00287F68;
    obj->x60C = 0;
    return obj;
}
