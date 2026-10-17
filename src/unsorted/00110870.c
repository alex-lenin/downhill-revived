/* UNIT 0x00110870 0x00110940 */
struct Obj { char pad[0x2d8]; int x2D8; };
struct Global { char pad[0x5bb4]; int x5BB4; char pad2[0x24]; int x5BDC; };
extern struct Global D_00398C40;
extern int func_00110940(struct Obj *,int);
extern int func_00110C98(struct Obj *);
extern int func_001109B0(struct Obj *,int);
extern void func_00110DA0(struct Obj *,int,int);
void func_00110870(struct Obj *obj,int arg) {
 int result, value;
 if(func_00110940(obj,arg)) result=obj->x2D8;
 else {
 if(D_00398C40.x5BB4==0) value=D_00398C40.x5BDC/2;
 else if(D_00398C40.x5BB4==2) value=D_00398C40.x5BDC/2+50;
 else value=D_00398C40.x5BDC/2+25;
 if(value>=51) result=func_00110C98(obj);
 else result=func_001109B0(obj,99999);
 }
 if(result) func_00110DA0(obj,result,arg);
}
