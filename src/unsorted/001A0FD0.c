/* UNIT 0x001A0FD0 0x001A1168 */
typedef unsigned int U128 __attribute__((mode(TI)));
typedef union Vector { U128 full; float f[4]; } Vector;
struct Item { char pad[4]; char x4; };
struct Obj { char pad[0x510]; struct Item *x510,*x514; struct { signed char x518; char pad519[2]; signed char x51B; } __attribute__((packed)) state; int x51C; };
struct Global { char pad[0xB190]; int xB190,xB194; };
extern struct Global D_004B25A0;
extern void func_00218278(struct Item *,int);
extern void func_00218498(int,int);
extern void func_001A1168(struct Obj *,struct Item *,Vector *);
extern void func_00218720(struct Item *,int,int,int);
extern void func_002433A8(int,int);
void func_001A0FD0(struct Obj *o,int a,int b,int c) {
 Vector pos;
 if (o->state.x51B) return;
 if (o->state.x518!=1) return;
 {
  if (D_004B25A0.xB194) {
   func_00218278(o->x510,D_004B25A0.xB194);
   pos.full=0; pos.f[0]=-4.0f; pos.f[1]=12.0f; pos.f[2]=22.2f;
   func_00218498(D_004B25A0.xB194,o->x51C+1);
   func_001A1168(o,o->x510,&pos);
   func_00218720(o->x510,(a^1)&1,b,c);
   o->x510->x4=0; func_002433A8(0,0);
  }
  if (D_004B25A0.xB190) {
   func_00218278(o->x514,D_004B25A0.xB190);
   pos.full=0; pos.f[0]=3.0f; pos.f[1]=12.0f; pos.f[2]=22.4f;
   func_00218498(D_004B25A0.xB190,o->x51C+1);
   func_001A1168(o,o->x514,&pos);
   func_00218720(o->x514,(a^1)&1,b,c);
   o->x514->x4=0; func_002433A8(0,0);
  }
 }
}
