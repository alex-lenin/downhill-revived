/* UNIT 0x00164A88 0x00164C38 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Shape { float mass; char pad4[0x4C]; Vec size; Vec balls[2]; Vec matrix[4]; };
struct Obj { float invMass; char pad4[0x2C]; void *x30,*x34,*x38,*x3C,*x40; char pad44[0xBC]; char *x100; char pad104[0x1C]; int x120; char pad124[0x2C]; Vec x150[4]; };
extern struct Shape D_003960A0[];
extern float D_0029C7F4[],D_0029C7F8[];
extern void func_002319A0(struct Obj *,float,float,float);
extern void func_00231AF8(struct Obj *,float,float,float,float);
extern void func_00231BD8(struct Obj *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static inline float Min(float a,float b) { float r; __asm__ volatile("min.s %0,%1,%2":"=&f"(r):"f"(a),"f"(b)); return r; }
static inline float Max(float a,float b) { float r; __asm__ volatile("max.s %0,%1,%2":"=&f"(r):"f"(a),"f"(b)); return r; }
static inline void Set(volatile float *p,float f) { p[0]=f; p[2]=f; p[1]=f; }
void func_00164A88(struct Obj *obj) {
 float low[4],high[4]; struct Shape *shape; char *p; Vec *ball,*src,*dst; int i,j;
 p=obj->x100; obj->x34=p+0x10; obj->x38=p+0x40;
 shape=&D_003960A0[obj->x120];
 if(*(unsigned long *)p & (0x8000UL<<43)) { obj->x3C=p+0x90; obj->x40=p+0xC0; obj->x30=p+0xB0; }
 obj->invMass=1.0f/shape->mass;
 func_002319A0(obj,shape->size.x,shape->size.y,shape->size.z);
 low[0]=D_0029C7F4[0]; low[1]=D_0029C7F4[0]; low[2]=D_0029C7F4[0];
 Set(high,D_0029C7F8[0]);
 ball=shape->balls; i=1;
 do {
  float radius=ball->w;
  low[0]=Min(low[0],ball->x-radius); low[1]=Min(low[1],ball->y-radius); low[2]=Min(low[2],ball->z-radius);
  high[0]=Max(high[0],ball->x+radius); high[1]=Max(high[1],ball->y+radius); high[2]=Max(high[2],ball->z+radius);
  i--; ball++;
 } while(i>=0);
 func_00231AF8(obj,high[0]-low[0],high[1]-low[1],high[2]-low[2],shape->mass);
 func_00231BD8(obj);
 for(j=0;j<4;j++)
  __asm__ volatile("lq $2,%1
	sq $2,0(%0)" : : "r"(&obj->x150[j]),"m"(shape->matrix[j]),"r"(j) : "$2", "memory");
}
