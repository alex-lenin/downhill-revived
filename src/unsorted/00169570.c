/* UNIT 0x00169570 0x001696A8 */
typedef struct Obj {
 char pad0[0x500]; int x500[8]; int x520; char pad524[12]; int x530;
 char pad534[12]; short x540; char pad542[0x1A]; unsigned int *x55C; int *x560, *x564;
} Obj;
extern char D_00513B00[], D_00397180[];
extern void func_002103D8(void *, int, Obj *);
extern void func_00167F60(void *, int, int, unsigned int, int);
void func_00169570(Obj *obj) {
 int i;
 int *p;
 if (obj->x540 > 0) {
  if (obj->x540 >= 61) {
   if (*obj->x55C < 150U) {
    *obj->x55C += 2;
    *obj->x560 += 2;
    *obj->x564 += 1;
   }
  } else {
   if (*obj->x55C != 0) {
    *obj->x55C -= 2;
    *obj->x560 -= 2;
    *obj->x564 -= 1;
   }
  }
  func_002103D8(D_00513B00, 0, obj);
  func_00167F60(D_00397180, obj->x500[0], 80, *obj->x55C, obj->x530);
  obj->x540--;
 } else {
  if (obj->x520 >= 2) {
   i = 0;
   if (obj->x520 > 0) {
    p = obj->x500;
    do {
     *p = p[1];
     i++;
     p++;
    } while (i < obj->x520);
   }
   obj->x540 = 180;
  }
  if (obj->x520 > 0) obj->x520--;
 }
}
