/* UNIT 0x001B20F8 0x001B2158 */
struct Obj { char pad[0x8080]; unsigned char x8080; char pad1[0x3f]; void *x80C0; };
struct Other { char pad[0x16d0]; int x16D0; };
extern char D_004A9E08[];
extern void func_0012F560(int, void *);
extern void func_00230DF8(struct Obj *, void *, unsigned char);
void func_001B20F8(struct Obj *obj, struct Other *other) {
 obj->x80C0 = other;
 func_0012F560(other->x16D0, D_004A9E08);
 func_00230DF8(obj, D_004A9E08, obj->x8080);
}
