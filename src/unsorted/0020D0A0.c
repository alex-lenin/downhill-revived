/* UNIT 0x0020D0A0 0x0020D0F8 */
typedef struct Entry { int x0, x4, x8; char pad[0x2c]; int x38; short x3C; short pad3E; int x40; char x44; char pad45[3]; } Entry;
typedef struct Object { Entry entries[80]; int x1680, x1684, x1688, x168C, x1690; } Object;
void func_0020D0A0(Object *obj) {
 int i;
 Entry *p;
 obj->x1684 = 0;
 obj->x1688 = 0;
 obj->x1680 = 0;
 obj->x168C = 0;
 obj->x1690 = 0;
 i = 0;
 p = obj->entries;
 do {
  p->x0 = 0; p->x4 = 0; p->x8 = 0;
  p->x38 = 0; p->x3C = 0; p->x40 = i; p->x44 = 0;
  i++; p++;
 } while (i < 80);
}
