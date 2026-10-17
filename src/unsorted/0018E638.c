/* UNIT 0x0018E638 0x0018E680 */
typedef struct Sub { char pad[7]; unsigned char x7; } Sub;
typedef struct Obj { char pad[0x438]; Sub x438; } Obj;
extern void func_0018D898(Sub *, int, int);
extern void func_0018E680(Obj *);
void func_0018E638(Obj *obj, int a, int b) {
    Sub *sub = &obj->x438;
    func_0018D898(sub, a, b);
    if (sub->x7 == 0) {
        func_0018E680(obj);
    }
}
