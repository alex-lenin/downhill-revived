/* UNIT 0x00114390 0x00114430 */
typedef struct Node { char pad[0x14]; unsigned short flags; } Node;
typedef struct Obj { char *x0; char pad4[0x6c]; Node *x70; Node *x74; char pad78[0x314]; int x38c; } Obj;
extern int func_00138560(void *);
extern int func_00114430(Obj *);
int func_00114390(Obj *obj) {
 int result = 0;
 if (!obj->x38c) {
  if ((func_00138560(obj->x0 + 0x40c0) & 255) && obj->x70 && obj->x74 && !((obj->x70->flags >> 11) & 1) && !((obj->x74->flags >> 11) & 1)) result = func_00114430(obj);
 }
 return result;
}
