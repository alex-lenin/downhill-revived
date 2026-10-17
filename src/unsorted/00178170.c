/* UNIT 0x00178170 0x001781E0 */
struct Vec { int x0,x4,x8,xC; };
struct Obj { char pad[0xC84]; struct Vec xC84; char pad2[0x25C]; struct Vec *xEF0[4]; char pad3[0x264]; int x1164; char pad4[0x1A4]; int x130C; };
void func_00178170(struct Obj *obj) {
 struct Vec *src; struct Vec **dst; int count;
 if (obj->x1164) {
  obj->x130C = 0;
  if(obj->xEF0[0]) {
   src = &obj->xC84; dst = obj->xEF0; count = 3;
   do { count--; (*dst)->x0 = obj->xC84.x0; (*dst)->x4 = src->x4; (*dst)->x8 = src->x8; (*dst)->xC = src->xC; dst++; } while(count >= 0);
  }
 }
}
