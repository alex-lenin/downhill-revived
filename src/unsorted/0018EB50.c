/* UNIT 0x0018EB50 0x0018EB90 */
typedef long long u128;
struct Obj { int x0; char pad[0x48c]; u128 x490; char pad4a0[0x90]; unsigned char x530; };
extern u128 func_0018EAC8(struct Obj *, int);
u128 func_0018EB50(struct Obj *obj) {
 u128 result = func_0018EAC8(obj, obj->x0);
 if (!obj->x530) { result = 0; obj->x490 = result; }
 return result;
}
