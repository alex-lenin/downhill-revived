/* UNIT 0x001843B8 0x00184418 */
struct Owner { char pad0[0x11BC]; void *x11BC; char pad1[0x1284-0x11C0]; void *x1284; char pad2[0x13E2-0x1288]; unsigned char x13E2; };
struct Obj { struct Owner *x0; };
extern int func_00184418(struct Obj *);
int func_001843B8(struct Obj *obj) {
    int result = 0;
    void *p;
    struct Owner *owner;
    if (func_00184418(obj)) goto found;
    owner = obj->x0;
    p = owner->x13E2 ? owner->x1284 : owner->x11BC;
    if (p) {
found:
        result = 1;
    }
    return result;
}
