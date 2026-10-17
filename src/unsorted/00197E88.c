/* UNIT 0x00197E88 0x00197F10 */
typedef struct Toggle { char pad[4]; unsigned char x4; } Toggle;
typedef struct Obj { char pad[0x464]; int x464; char pad468[0x9C]; Toggle *x504; int x508; } Obj;
extern char D_00435500[];
extern void func_002182A8(Toggle *, int);
extern void func_00196DF0(void *);
extern void func_0018DC20(Obj *);
void func_00197E88(Obj *obj) {
 obj->x464 = 70;
 if (--obj->x508 < 0) {
  Toggle *p = obj->x504;
  if (p && p->x4) func_002182A8(p, 0); else func_002182A8(p, 1);
  obj->x508 = 30;
 }
 func_00196DF0(D_00435500);
 func_0018DC20(obj);
}
