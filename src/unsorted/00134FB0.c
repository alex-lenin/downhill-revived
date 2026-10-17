/* UNIT 0x00134FB0 0x001350E8 */
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct PackedLong { long value; } __attribute__((packed)) PackedLong;
extern char D_0029C390[];
extern void *memcpy(void *,const void *,unsigned int);
extern float D_0026DE60[];
extern void func_00231BD8(void *);
extern void func_001350E8(void *);
extern void func_001350F0(void *);
extern void func_00155148(void *);
extern void func_00135100(void *);
extern float func_00135440(void *);
#define F(o,n) (*(float *)((o)+(n)))
#define I(o,n) (*(int *)((o)+(n)))
#define S(o,n) (*(short *)((o)+(n)))
static inline void set(Vec *v,float x,float y,float z) { v->x=x; v->y=y; v->z=z; }
void func_00134FB0(char *o) {
 Vec zero; char *p; float result;
 func_00231BD8(o);
 F(o,0)=1.0f/F(o,0x150);
 set((Vec *)(o+0x220),0,0,1); set((Vec *)(o+0x230),0,0,1);
 set((Vec *)(o+0x100),0,0,0); set((Vec *)(o+0x110),0,0,0);
 func_001350E8(o); func_001350F0(o);
 F(o,0x154)=1.0f;
 p=*(char **)(o+0x158);
 F(o,0x418)=D_0026DE60[(I(p,0x796C)>>5)-1];
 S(o,0x4A4)=0; S(o,0x4A6)=0; S(o,0x4A8)=0; F(o,0x488)=0;
 *(long *)(o+0x4B0)&=~1L;
 func_00155148(p+0x3FD0);
 I(o,0x494)=0; F(o,0x48C)=1; F(o,0x490)=0;
 func_00135100(o);
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(zero));
 __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(*(Vec *)(o+0x140)) : "r"(&zero) : "$2");
 memcpy(o+0x4B4,D_0029C390,8);
 *(void **)(o+0x164)=o;
 result=func_00135440(o);
 I(o,0x410)=0; F(o,0x414)=1; F(o,0x428)=result;
}
