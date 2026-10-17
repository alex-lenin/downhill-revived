/* UNIT 0x001B0A50 0x001B0AA0 */
typedef struct { char pad0[9]; unsigned char x9; short xa; char padc[0x58]; char *x64; } Obj;
extern void func_001B0AA0(Obj *);
extern void func_001AC7B0(Obj *, void *);
extern void func_001AC810(Obj *, float);
void func_001B0A50(Obj *obj, char *arg1) {
    obj->x64 = arg1;
    func_001B0AA0(obj);
    obj->xa = 0;
    func_001AC7B0(obj, obj->x64 + 0x2d04);
    func_001AC810(obj, 0.0f);
    obj->x9 = 4;
}
