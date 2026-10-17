/* UNIT 0x0011CF70 0x0011D048 */
typedef struct Items { int x0[30]; } Items;
typedef struct Obj { char pad0[0x4920]; int x4920[30]; char pad4998[0x80]; int x4A18[30]; int x4A90; } Obj;
extern int D_004B2828[];
extern char D_00383A10[];
extern int func_001591D8(unsigned int *);
extern void func_002182A8(int, int);
extern void func_00158120(void *, int, float);
void func_0011CF70(Obj *obj, unsigned int *source) {
 if (obj->x4A90 < 30 && D_004B2828[0] < 3) {
  Items *items = (Items *)obj->x4920;
  items->x0[obj->x4A90] = func_001591D8(source);
  func_002182A8(items->x0[obj->x4A90], 0);
  if (source) obj->x4A18[obj->x4A90] = (*source >> 7) & 0x7FF;
  func_00158120(D_00383A10, items->x0[obj->x4A90], 7.0f);
  obj->x4A90++;
 }
}
