/* UNIT 0x00164270 0x001642D0 */
typedef struct Obj { char pad0[0x8340]; char *x8340[30]; int x83B8,x83BC; } Obj;
extern void func_001642D0(Obj *);
void func_00164270(Obj *obj) {
 char **slot=&obj->x8340[29];
 int count=29;
 char *p=(char *)obj+0x7EE0;
 do {
  *slot=p;
  count--;
  slot--;
  p-=0x460;
 } while(count>=0);
 obj->x83B8=0;
 obj->x83BC=0;
 func_001642D0(obj);
}
