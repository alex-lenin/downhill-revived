/* UNIT 0x00129C68 0x00129DC0 .rodata=0x288FD0:0x289000 */
typedef struct Obj { int x0; char *x4; char pad8[0x28-8]; int state; char pad2C[0x70-0x2C]; int x70; char pad74[0x140-0x74]; char x140[0x34]; int x174; char pad178[0x194-0x178]; int x194; } Obj;
extern int func_00238F60(void);
extern int D_004B2828[];
extern int func_00222BD8(int,int);
extern void func_00129DC0(Obj *,int);
extern void func_00129860(Obj *,int);
extern void func_00129FC8(Obj *);
extern void func_0012A3E8(Obj *);
extern void func_0012A4D0(Obj *);
extern void func_0012A570(Obj *);
extern void func_0012A6D8(Obj *);
extern void func_0012A790(Obj *);
extern void func_0012A9B8(Obj *);
extern void func_0012AA58(Obj *);
extern void func_001CFB60(void *,void *,int,int);
void func_00129C68(Obj *obj) {
 obj->x174 += func_00238F60(); obj->x194 += func_00238F60();
 switch(obj->state) {
 case 0: if(obj->x4) { if(D_004B2828[0]==1 || func_00222BD8(0,1)) {func_00129DC0(obj,obj->x70); func_00129860(obj,1);} else func_00129860(obj,10); } break;
 case 1: func_00129FC8(obj); break;
 case 2: func_0012A3E8(obj); break;
 case 3: func_0012A4D0(obj); break;
 case 4: func_0012A570(obj); break;
 case 5: func_0012A6D8(obj); break;
 case 6: func_0012A790(obj); break;
 case 7: func_0012A9B8(obj); break;
 case 8: func_0012AA58(obj); break;
 case 9: default: break;
 }
 func_001CFB60(obj->x140,obj->x4+0x10,obj->state,6);
}
