/* UNIT 0x001E9208 0x001E9250 */
struct Obj { char *x0; char pad[0x28]; int x2C; };
struct Entry { int x0; int x4; };
extern struct Entry D_002752B0[];
extern int func_001E6918(void *);
int func_001E9208(struct Obj *obj) {
    int result = func_001E6918(obj->x0 + 0x6610);
    struct Entry *entry = &D_002752B0[obj->x2C];
    if (result != 0) result = entry->x4;
    return result;
}
