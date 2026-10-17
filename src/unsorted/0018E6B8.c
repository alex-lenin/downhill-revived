/* UNIT 0x0018E6B8 0x0018E700 */
typedef struct Sub { char pad[7]; unsigned char x7; } Sub;
typedef struct Obj { char pad[0x448]; Sub x448; } Obj;
extern void func_0018D898(Sub *, int, int);
extern void func_0018E700(Obj *);
void func_0018E6B8(Obj *obj, int a, int b) {
    Sub *sub = &obj->x448;
    func_0018D898(sub, a, b);
    if (sub->x7 == 0) {
        func_0018E700(obj);
    }
}

