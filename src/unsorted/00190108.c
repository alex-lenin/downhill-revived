/* UNIT 0x00190108 0x00190188 */
struct Obj { char pad[0x18c]; int x18c; };
struct VEntry { short delta; short pad; void (*func)(void *); };
struct Other { char pad[0x4f8]; struct VEntry *vtable; };
extern void func_001900C8(struct Obj *, struct Other *);
extern void func_001B4AF8(struct Obj *);
extern void func_00190660(struct Obj *, struct Other *);
extern void func_00190CE8(void *);
extern void func_001901C0(struct Obj *);
void func_00190108(struct Obj *obj, struct Other *other) {
 obj->x18c = 0;
 func_001900C8(obj, other);
 func_001B4AF8(obj);
 func_00190660(obj, other);
 func_00190CE8((char *)obj + 0x64);
 other->vtable[11].func((char *)other + other->vtable[11].delta);
 func_001901C0(obj);
 other->vtable[12].func((char *)other + other->vtable[12].delta);
}
