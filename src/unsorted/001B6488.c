/* UNIT 0x001B6488 0x001B65E8 */
#include "particle.h"
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef unsigned int Quad __attribute__((mode(TI)));
typedef struct Type { int x0; short x4; unsigned short x6; } Type;
extern Type D_0038DB90[];

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_001B6488(Vec *a,Vec *b,Vec *c,int arg3,int type) {
 Vec pos,vel,dir;
 float matrix[16] __attribute__((aligned(16)));
 float len;
 int effect;
 Vec *vp=&vel, *pp=&pos, *dp=&dir;
 *(Quad *)pp=*(Quad *)a;
 type &= 0xFFFF;
 COPY_VECTOR(*vp,b);
 *(Quad *)dp=*(Quad *)c;
 __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len) : "f"(vp->x), "f"(vp->y), "f"(vp->z));
 if(len<2.4f) {
 effect=-1;
 if(D_0038DB90[type].x6&1) effect=14;
 else switch(D_0038DB90[type].x4) { case 8:effect=26;break;case 2:effect=29;break;case 30:effect=28;break;case 32:effect=33;break; }
 if(effect!=-1) { func_002226C8(matrix,dp); func_0022CDC0((Vector4 *)(pp), effect, (Matrix4 *)(matrix), (int)(arg3), 1.0f, (Vector4 *)(vp)); }
 }
}
