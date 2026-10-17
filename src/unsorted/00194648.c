/* UNIT 0x00194648 0x001946E0 */
typedef struct Rider { char p[0x661C]; float x661C; char p2[0x796C-0x6620]; int x796C; char p3[0x1C]; int x798C; } Rider;
typedef struct State { char p[0x50130]; Rider *x50130[2]; } State;
typedef struct Obj { char p[0x12208]; int x12208, x1220C; } Obj;
extern State D_002AF2C0;
extern unsigned char D_0043FA34[];
extern char D_004C8508[];
extern int func_001FA168(void *, int, int);
void func_00194648(Obj *obj, int index) {
 if (!D_0043FA34[0]) {
  Rider *rider = D_002AF2C0.x50130[index];
  int count = (int)rider->x661C;
  int delta = func_001FA168(D_004C8508, rider->x798C, (rider->x796C >> 5)-1);
  int old = obj->x12208;
  obj->x1220C = old;
  obj->x12208 = old + (delta + count);
 }
}
