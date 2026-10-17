/* UNIT 0x001F9CF8 0x001F9D58 */
struct Object { char pad[0x2C]; int count; };
extern void func_001F9C78(struct Object *, void *);
void func_001F9CF8(struct Object *obj) {
 int i;
 for (i=0; i<obj->count; i++) func_001F9C78(obj, (char *)obj + 0x8 + i*12);
}
