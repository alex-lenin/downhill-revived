/* UNIT 0x0012C2E8 0x0012C398 */
typedef struct { unsigned long flags; } Item;
typedef struct { char pad[4]; unsigned char x4; } State;
typedef struct { char pad[0x16D4]; int x16D4; char pad2[0xA88]; unsigned char x2160; } Sub;
typedef struct { char pad[0x1E40]; Sub sub; char pad2[0x3988]; Item *x792C; State *x7930; int x7934, x7938, x793C; } Obj;
extern int func_0021D638(Item *, float);
extern void func_0021D738(int, int);
void func_0012C2E8(Obj *obj) {
    Sub *sub = &obj->sub;
    int result;
    if (sub->x2160) {
        result = func_0021D638(obj->x792C, 9.0f);
        obj->x793C = result;
        if (result) func_0021D738(result, sub->x16D4);
    } else {
        obj->x793C = func_0021D638(obj->x792C, 3.0f);
    }
    if (obj->x7930) obj->x7930->x4 = 0;
    obj->x792C->flags &= ~((unsigned long)1 << 56);
}
