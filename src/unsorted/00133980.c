/* UNIT 0x00133980 0x00133C30 .sdata=0x29C2C8:0x29C2D0 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef union Body { unsigned long flags; struct { unsigned int *x0; } p; char bytes[0xD0]; } Body;
typedef struct Rider { char pad0[0x792C]; Body *x792C; } Rider;
typedef struct Physics { float x0; char pad4[0x2C]; void *x30,*x34,*x38,*x3C,*x40; char pad44[0x2C]; Vec x70; } Physics;
typedef struct Obj { int x0; Rider *x4; Body *x8; int xC; Physics x10; char pad90[0x80]; float x110; char pad114[0x18]; int x12C; char pad130[0x10]; float x140,x144,x148; int x14C; Vec x150[8]; int x1D0,x1D4,x1D8,x1DC; char x1E0; } Obj;
/* Retail sdata: C8: int 6, CC: 0.7f (no code references it), D0: int 0 (owned by unit
   00133DE0), D4: 5000.0f. D0 splits our two words and a unit claims one contiguous range,
   so this file owns C8:D0; D4 (D_0029C2D4) stays in asm, and the .extern below (with its
   true size) lets the assembler reach it via $gp exactly as retail does. */
int D_0029C2C8 = 6;
static int D_0029C2CC_pad = 0x3F333333; /* 0.7f; completes the C8:D0 range, no code references it */
extern float D_0029C2D4;
__asm__(".extern D_0029C2D4, 4");
extern float D_0029C2D8[],D_0029C2DC[];
extern void func_001308A8(Rider *);
extern void func_00231968(Physics *,Vec *);
extern void func_002319A0(Physics *,float,float,float);
extern void func_00231AF8(Physics *,float,float,float,float);
extern void func_00231BD8(Physics *);
extern void func_00232D68(void *,Physics *);
static inline float Min(float a,float b) { float r; __asm__ volatile("min.s %0,%1,%2" : "=&f"(r) : "f"(a),"f"(b)); return r; }
static inline float Max(float a,float b) { float r; __asm__ volatile("max.s %0,%1,%2" : "=&f"(r) : "f"(a),"f"(b)); return r; }
static inline void Clamp(float *p) { float f=*p; float limit=D_0029C2D4; if(f>limit) *p=limit; else if(f < -limit) *p=-limit; }
void func_00133980(Obj *obj) {
 Vec box[2];
 Physics *physics;
 Physics *p;
 Body *body;
 Vec *sphere;
 int count,i;
 float r;
 obj->x1E0=1; obj->x1D4=0; obj->x1D0=D_0029C2C8; obj->x1DC=30;
 obj->x8=obj->x4->x792C;
 *obj->x8->p.x0=(*obj->x8->p.x0 & 0xFFFC007F) | ((obj->x1D8 & 0x7FF)<<7);
 body=obj->x8;
 p=&obj->x10;
 p->x34=body->bytes+0x10; p->x38=body->bytes+0x40;
 if(body->flags & 0x400000000000000UL) { p->x3C=body->bytes+0x90; p->x40=body->bytes+0xC0; p->x30=body->bytes+0xB0; }
 p->x0=1.0f/obj->x110;
 func_002319A0(p,obj->x140,obj->x144,obj->x148);
 physics=p;
 p=(Physics *)&obj->x10.x70;
 {
  Vec *lo=&box[0],*hi=&box[1];
  lo->x=D_0029C2D8[0]; lo->y=D_0029C2D8[0]; lo->z=D_0029C2D8[0];
  hi->x=D_0029C2DC[0]; hi->y=D_0029C2DC[0]; hi->z=D_0029C2DC[0];
 }
 count=obj->x12C;
 for(i=0;i<count;i++) {
  sphere=&obj->x150[i];
  r=sphere->w;
  box[0].x=Min(box[0].x,sphere->x-r);
  box[0].y=Min(box[0].y,sphere->y-r);
  box[0].z=Min(box[0].z,sphere->z-r);
  box[1].x=Max(box[1].x,sphere->x+r);
  box[1].y=Max(box[1].y,sphere->y+r);
  box[1].z=Max(box[1].z,sphere->z+r);
 }
 func_00231AF8(physics,box[1].x-box[0].x,box[1].y-box[0].y,box[1].z-box[0].z,obj->x110);
 func_00231BD8(physics);
 func_00232D68((char *)obj->x4+0x40C0,physics);
 Clamp(&obj->x10.x70.x); Clamp(&((Vec *)p)->y); Clamp(&((Vec *)p)->z);
 func_00231968(physics,(Vec *)p);
 func_001308A8(obj->x4);
}
