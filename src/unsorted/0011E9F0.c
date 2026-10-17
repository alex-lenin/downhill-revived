/* UNIT 0x0011E9F0 0x0011EBC8 .rodata=0x288CA0:0x288CD0 */
typedef struct Model { unsigned long flags; char pad8[8]; float pos[4]; } Model;
typedef struct Rider { char pad0[0x792c]; Model *x792C; } Rider;
typedef struct Obj { char pad0[4]; int x4; unsigned int x8; char padC[4]; Model *x10; char pad14[0x24]; Rider *x38; char pad3C[0xc4]; void *x100; char pad104[0xa4]; char x1A8[0xb4]; unsigned short x25C; char pad25E[0xc]; unsigned short x26A; } Obj;
extern int func_00238F60(void);
extern int D_004B2828[];
extern int func_00222BD8(int,int);
extern void func_0011EBC8(Obj *,void *);
extern void func_0011E568(Obj *,int);
extern void func_0011EDA8(Obj *);
extern void func_0011F900(Obj *);
extern void func_0011FB70(Obj *);
extern void func_0011FC70(Obj *);
extern void func_0011FF38(Obj *);
extern void func_00120208(Obj *);
extern void func_00120838(Obj *);
extern void func_001208D8(Obj *);
extern void func_001CFB60(void *,float *,unsigned int,int);
extern float func_002223F0(float *,float *);
void func_0011E9F0(Obj *obj) {
 obj->x25C+=func_00238F60();
 obj->x26A+=func_00238F60();
 switch(obj->x8) {
 case 0:
  if(obj->x10) {
   if(D_004B2828[0]==1 || func_00222BD8(0,1)) {
    func_0011EBC8(obj,obj->x100); func_0011E568(obj,1);
   } else func_0011E568(obj,10);
  }
  break;
 case 1: func_0011EDA8(obj); break;
 case 2: func_0011F900(obj); break;
 case 3: func_0011FB70(obj); break;
 case 4: func_0011FC70(obj); break;
 case 5: func_0011FF38(obj); break;
 case 6: func_00120208(obj); break;
 case 7: func_00120838(obj); break;
 case 8: func_001208D8(obj); break;
 case 9: default: break;
 }
 func_001CFB60(obj->x1A8,obj->x10->pos,obj->x8,obj->x4);
 if(obj->x10) {
  if(obj->x38 && func_002223F0(obj->x38->x792C->pos,obj->x10->pos)<15.0f)
   obj->x10->flags|=(unsigned long)1<<57;
  else obj->x10->flags&=~((unsigned long)1<<57);
 }
}
