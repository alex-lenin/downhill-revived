/* UNIT 0x0011FC70 0x0011FE08 */
typedef struct Sound { int x0,x4,x8,xC; } Sound;
typedef struct Obj {
 char pad0[0x10]; unsigned int **x10;
 char pad14[0x48]; Sound x5C;
 char pad6C[0xF4]; float x160[4]; float x170[4];
 char pad180[0xDC]; short x25C; char pad25E[0xA]; unsigned long x268;
} Obj;
extern void func_0011EDA8(Obj *);
extern float func_00238F40(void);
extern void func_00236558(void *,void *,float);
extern void func_00120CE8(Obj *);
extern int func_00222BD8(int,int);
extern void func_0011E568(Obj *,int);
extern void func_00203590(void *,unsigned int,int,int);
extern void func_00203DC8(Sound *,int);
extern void func_00202E38(void *,int,int,float);
void func_0011FC70(Obj *obj) {
 float buf[4]; Sound sound; void *pos;
 func_0011EDA8(obj);
 pos = obj->x160;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(buf) : "r"(obj->x170),"f"(-1.0f) : "$2");
 func_00236558(pos,buf,func_00238F40());
 obj->x268 &= 0xFFFBFFFFFFFFFFFFUL;
 func_00120CE8(obj);
 if ((**obj->x10 & 0xFFFC0000) == 0x13180000 && obj->x25C >= 181) {
  if (func_00222BD8(0,1)) func_0011E568(obj,3);
  else {
   if (func_00222BD8(0,1)) func_00203590(&obj->x5C, **obj->x10 >> 18, (short)obj->x268,1);
   else func_00203590(&obj->x5C, **obj->x10 >> 18, (short)obj->x268,2);
   sound = obj->x5C;
   func_00203DC8(&sound,1);
   func_00202E38(&obj->x5C,1,1,600.0f);
   obj->x25C=0;
  }
 }
}
