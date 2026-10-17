/* UNIT 0x001DDD30 0x001DDD78 */
extern void func_001DF560(void *obj, void *name, void *out);
extern char D_002935A0[];
extern char D_002935B0[];

struct Obj {
    char pad0[0x16C];
    int x16C;
    int x170;
    int x174;
};

void func_001DDD30(struct Obj *obj)
{
    func_001DF560(obj, D_002935A0, &obj->x16C);
    func_001DF560(obj, D_002935B0, &obj->x170);
    obj->x174 = 0;
}
