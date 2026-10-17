/* UNIT 0x002083E8 0x00208440 */
typedef struct Object { char pad[0xc0]; int xC0; } Object;
extern void func_00208360(void *);
void func_002083E8(Object *obj) {
 char *p = (char *)obj;
 int count = 11;
 do { count--; func_00208360(p); p += 16; } while (count >= 0);
 obj->xC0 = 0;
}
