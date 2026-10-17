/* UNIT 0x00148310 0x00148520 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Obj {
 u128 x0,x10,x20,x30,x40,x50,x60,x70;
 Vec x80,x90,xA0,xB0,xC0,xD0; float xE0[4]; Vec xF0,x100,x110,x120,x130,x140;
 char pad150[0x160]; int x2B0,x2B4; char pad2B8[4]; int x2BC,x2C0;
 float x2C4,x2C8,x2CC,x2D0,x2D4,x2D8,x2DC,x2E0,x2E4,x2E8,x2EC,x2F0,x2F4,x2F8,x2FC,x300,x304,x308,x30C,x310;
 char pad314[0x30];
 int x344,x348,x34C,x350,x354,x358,x35C,x360,x364,x368;
 char x36C,x36D,x36E,x36F,x370,x371,x372,x373,x374,x375;
} Obj;
extern void func_00221F20(Obj*);
extern void func_00148560(Obj*,int);
extern void func_0014BEB8(Obj*,int);
extern void func_0014D798(Obj*,Vec*,float*);
extern void func_0014D740(Obj*);
void func_00148310(Obj *obj,int index) {
 float value; Vec *pos; float *vec;
 func_00221F20(obj);
 /* Sony sceVu0CopyMatrix sequence. */
 __asm__ volatile("lq $8, 0x0(%1)
	lq $9, 0x10(%1)
	lq $10, 0x20(%1)
	lq $11, 0x30(%1)
	sq $8, 0x40(%1)
	sq $9, 0x50(%1)
	sq $10, 0x60(%1)
	sq $11, 0x70(%1)" : "=m"(obj->x40) : "r"(obj) : "$8", "$9", "$10", "$11", "memory");
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->x80));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->x90));
 pos=(Vec*)((unsigned int)obj+0xA0);
 __asm__ volatile("sqc2 $vf0, 0xA0(%1)" : "=m"(*pos) : "r"(obj));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->xB0));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->xC0));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->xD0));
 vec=(float*)((unsigned int)obj+0xE0);
 vec[0]=0; vec[1]=0; vec[2]=0; vec[3]=1.0f;
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->xF0));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->x100));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->x110));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->x120));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->x130));
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(obj->x140));
 obj->x2B0=0; obj->x2B4=0;
 func_00148560(obj,1);
 obj->x2DC=1.0f;
 obj->x2E0=1.0f;
 obj->x2CC=0;
 obj->x2D0=0;
 obj->x2D4=0;
 obj->x2D8=0;
 obj->x2E4=0;
 obj->x2EC=0;
 obj->x2E8=0;
 obj->x2F0=0;
 obj->x2C4=0.01f;
 obj->x2C8=2.5f;
 obj->x2F4=0.04f;
 obj->x2F8=0.05f;
 obj->x2FC=-5.5f;
 obj->x300=0.0075f;
 obj->x304=0.2f;
 obj->x308=0.001f;
 obj->x30C=1000.0f;
 obj->x310=0.002f;
 obj->x2BC=2;
 obj->x2C0=2;
 func_0014BEB8(obj,1);
 obj->x35C=index;
 obj->x348=0;
 obj->x354=0;
 obj->x358=0;
 obj->x364=0;
 obj->x368=0;
 obj->x360=0;
 obj->x36C=0;
 obj->x36E=0;
 obj->x36F=0;
 obj->x370=0;
 obj->x371=0;
 obj->x372=0;
 obj->x373=0;
 obj->x374=0;
 obj->x375=0;
 obj->x36D=obj->x350=obj->x34C=obj->x344=1;
 func_0014D798(obj,pos,&value);
 obj->x2CC=value;
 func_0014D740(obj);
}
