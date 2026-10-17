/* UNIT 0x001F9C98 0x001F9CF8 */
struct Object { char pad[0x84]; int count; };
extern void func_001F9C78(struct Object *, void *);
void func_001F9C98(struct Object *obj) {
 int i;
 for (i=0; i<obj->count; i++) func_001F9C78(obj, (char *)obj + 0x30 + i*12);
}
