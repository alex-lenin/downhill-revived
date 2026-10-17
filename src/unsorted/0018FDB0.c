/* UNIT 0x0018FDB0 0x0018FE08 */
struct List { char pad[8]; unsigned char x8; char pad9[3]; void *xC[1]; };
struct Obj { int x0; signed char x4; char pad5[7]; struct List *xC; };
extern void func_00218498(void *, int);
void func_0018FDB0(struct Obj *obj, int value) {
    struct List *list;
    if (value) {
        list = obj->xC;
        if (list) func_00218498(list->xC[list->x8], 0);
    }
    obj->x4 = value;
}

