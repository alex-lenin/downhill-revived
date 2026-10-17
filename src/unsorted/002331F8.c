/* UNIT 0x002331F8 0x00233418 */
typedef struct Entry { char pad0[4]; unsigned short x4; char pad6[2]; float x8; short xC; char padE[2]; int x10; char x14[1]; } Entry;
typedef struct Table { char pad0[4]; unsigned int x4; char pad8[8]; Entry **x10; } Table;
typedef struct Obj { char pad0[0x1D8]; Table *x1D8; char pad1DC[0x10]; int x1EC; unsigned int x1F0; } Obj;
extern unsigned int func_00233658(Obj *);
extern void func_00234BD8(Obj *,void *,int);
int func_002331F8(Obj *obj,int arg) {
 unsigned int i;
 Table *table;
 float previous,current;
 if(!arg) arg++;
 obj->x1EC=arg;
 table=obj->x1D8;
 if(table) {
  for(i=0;i<table->x4;i++,table=obj->x1D8) {
   if(table->x10[i]->x4) {
    previous=obj->x1D8->x10[i]->x8*(float)(func_00233658(obj)-1);
    current=obj->x1D8->x10[i]->x8*(float)func_00233658(obj);
    obj->x1D8->x10[i]->xC=(int)(current-previous);
    previous-=(float)(int)previous;
    current=obj->x1D8->x10[i]->x8;
    current-=(float)(int)current;
    if(previous+current>=1.0f)
     obj->x1D8->x10[i]->xC++;
    if(obj->x1F0%obj->x1D8->x10[i]->x4==0)
     func_00234BD8(obj,obj->x1D8->x10[i]->x14,obj->x1D8->x10[i]->x10);
   } else table->x10[i]->xC=-1;
  }
 }
 return 0;
}
