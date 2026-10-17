/* UNIT 0x0010FB00 0x0010FDA0 */
typedef int Quad __attribute__((mode(TI)));
#include "vector.h"
typedef Vector4 Vec;
typedef struct Frame { char pad[0x10]; Vec x10; } Frame;
typedef struct World { char pad[0x4564]; short x4564; char pad4566[0x33c6]; Frame *x792C; char pad7930[0x4c]; int x797C; } World;
typedef struct Track { char pad[0x10]; unsigned char x10; char pad11[3]; unsigned short x14; } Track;
typedef struct Obj { World *x0; char pad4[0x2c]; Quad x30,x40; char pad50[0x20]; int x70; Track *x74; char pad78[0x200]; volatile float x278; char pad27C[0x198]; int x414; char pad418[0x28]; unsigned long x440; } Obj;
typedef struct Global { char pad[0x501b4]; int x501B4; } Global;
typedef struct Pair { Quad x0,x10; } Pair;
extern Global D_002AF2C0;
extern char D_0029EA20[];
extern int func_00138560(void *),func_00118BB8(void *,Track *,Track *);
extern Track *func_00119960(void *,void *,void *);
extern Pair *func_00119810(void *,Track *);
extern void func_0010B2B8(Obj *,Track *),func_0010FDA0(Obj *),func_0010E860(Obj *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static inline float Maximum(float b,float a) { float r; __asm__ volatile("max.s %0,%1,%2" : "=&f"(r) : "f"(a),"f"(b)); return r; }
void func_0010FB00(Obj *obj) {
 Vec normal,pos,extra; World *world; Track *track; Pair *pair; Vec *velocity,*axis,*a,*b; float limit,len,inv,dot,dot2,x,y,z,absSpeed,trackSpeed;
 if(!((obj->x74->x14>>5)&1) && obj->x414>=181) {
 if((func_00138560((char *)obj->x0+0x40c0)&255) && obj->x0->x4564>=61) obj->x440 |= 0x2000000000000000UL;
 world=obj->x0;
 if(world->x797C==D_002AF2C0.x501B4) {
 len=__builtin_fabsf(obj->x278);
 __asm__ volatile ("Lfb00_k1:" : : "f"(len));
 if(Maximum((float)(*(Track * volatile *)&obj->x74)->x10,5.0f)<len) {
 velocity=(Vec *)((char *)world+0x4180);
 x=velocity->x; y=velocity->y; z=velocity->z;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(len));
 normal.x=x*inv; normal.y=y*inv; normal.z=z*inv;
 axis=(Vec *)((char *)world->x792C+0x50);
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "f"(normal.x),"f"(axis->x),"f"(normal.y),"f"(axis->y),"f"(normal.z),"f"(axis->z));
 if(dot>0.95f) {
 COPY_VECTOR(pos,&world->x792C->x10);
 track=func_00119960(D_0029EA20,&pos,&extra);
 if(track && track!=obj->x74 && (!func_00118BB8(D_0029EA20,track,obj->x74) || (obj->x440 & 0x2000000000000000UL))) {
 obj->x74=track;
 track=(Track *)func_00119810(D_0029EA20,track);
 obj->x30=((Pair *)track)->x0; obj->x40=((Pair *)track)->x10; obj->x70=0;
 func_0010B2B8(obj,obj->x74); func_0010FDA0(obj);
 }
 goto clear_flag;
 }
 } else {
 clear_flag: obj->x440 &= ~0x2000000000000000UL;
 }
 }
 if(obj->x70 && obj->x74) {
 a=(Vec *)((char *)obj+0xf0); b=(Vec *)((char *)obj+0xd0);
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot2) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 if(dot2<0.5f) func_0010E860(obj);
 }
 }
}
