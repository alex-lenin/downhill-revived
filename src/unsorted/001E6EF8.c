/* UNIT 0x001E6EF8 0x001E6F58 */
struct Object { int x0; char pad[0x9B8]; int slots[20]; char pad2[8]; int xA14; char pad3[2]; char xA1A; };
extern void func_001E7440(struct Object *);
void func_001E6EF8(struct Object *obj, int value) {
 int i;
 func_001E7440(obj);
 obj->x0 = value; obj->xA14 = 0; obj->xA1A = 0;
 for (i=19; i>=0; i--) obj->slots[i] = 0;
}
