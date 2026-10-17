/* UNIT 0x00141130 0x00141188 */
struct Obj { char pad[0x130]; int x130; };
extern char D_002AF2C0[];
extern void func_00119E60(void *);
extern void func_00206A20(int);
void func_00141130(struct Obj *obj) {
 int i = 0;
 func_00119E60(D_002AF2C0);
 do { func_00206A20(obj->x130 + i); i++; } while (i < 5);
}
