/* UNIT 0x001118A8 0x001119F8 */
typedef struct Rider {
 char pad0[0x11EC]; int x11EC; char pad11F0[0x60]; int x1250; char pad1254[0x181]; unsigned char x13D5; char pad13D6[0x2CEA]; char x40C0[0x38CC]; int x798C;
} Rider;
typedef struct Obj { Rider *x0; int x4; Rider *x8; char padC[0x3FC]; int x408; char pad40C[0x30]; int x43C; } Obj;
extern float func_001385A8(void *);
extern int func_001119F8(Obj *, int);
extern unsigned char D_0029EA20[];
extern int D_0043EEC8[];
int func_001118A8(Obj *obj) {
 int count,enabled,result=1,limit,denominator,current;
 Rider *rider=obj->x0;
 if(rider->x13D5) count=rider->x1250; else count=rider->x11EC;
 enabled=count!=0;
 if(obj->x8 && func_001385A8(obj->x8->x40C0)>0.01f) {
  if(D_0029EA20[0x1089F]) {
   if(obj->x8->x798C < obj->x0->x798C) {
    denominator=obj->x43C; limit=300;
    if(denominator) limit=300-(D_0043EEC8[0]*300)/denominator;
    current=obj->x408;
    if(limit < current) result=0;
    else result=current>=1 ? (enabled ? result : 0) : result;
   } else result=func_001119F8(obj,enabled);
  } else if(func_001385A8(obj->x0->x40C0)<0.2f) result=1;
 } else result=func_001119F8(obj,enabled);
 return result;
}
