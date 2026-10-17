/* UNIT 0x0018DB20 0x0018DBA0 */
struct Obj { int x0; char pad[0x43b]; unsigned char x43f; char pad440[15]; unsigned char x44f; char pad450[20]; int x464; };
extern int D_003F52F0[];
extern void func_0023D650(int, int *, int *);
extern void func_0018E680(struct Obj *);
extern void func_0018E700(struct Obj *);
void func_0018DB20(struct Obj *obj) {
 int a, b;
 obj->x0 = 0;
 if (obj->x464 < 0) {
  func_0023D650(D_003F52F0[0], &a, &b);
  obj->x464 = a / 2;
  if (!obj->x43f) func_0018E680(obj);
  if (!obj->x44f) func_0018E700(obj);
 }
}
