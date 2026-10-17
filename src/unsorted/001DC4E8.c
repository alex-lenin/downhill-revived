/* UNIT 0x001DC4E8 0x001DC548 */
struct Object { char pad[0x25C4]; int count; };
extern void func_001DCCB0(void *);
void func_001DC4E8(struct Object *obj) {
 int i; char *p;
 for (i = 0; i < obj->count; i++) { func_001DCCB0((char *)obj + i * 0x60); }
}
