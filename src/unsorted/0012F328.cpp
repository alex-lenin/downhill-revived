/* UNIT 0x0012F328 0x0012F560 */
#include "object_registry.h"
struct Vec { float x,y,z,w; } __attribute__((aligned(16)));
class Target {
public:
    virtual void v0();
    virtual void v1();
    virtual void position(Vec *,Vec *);
    virtual void *body();
};
struct Hit { char pad[4]; unsigned short id; char pad6[10]; Vec pos; };
struct Physics { char pad[0x34]; Vec *pos; Vec *direction; };

extern void func_00232A80(Physics *,Vec *,Vec *);
extern void func_001B0AC0(void *,float);
extern void func_00133718(Physics *,Vec *);
extern void func_00155E78(void *,int);
static inline void sub(Vec *out,Vec *a,Vec *b) {
__asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out): "r"(a),"r"(b));
}
static inline float norm(Vec *out,Vec *v) {
float len, inv, x=v->x, y=v->y, z=v->z;
/* SDK length and reciprocal sequence, as in the promoted vector helpers. */
__asm__ volatile ("mula.s %2, %2
	madda.s %3, %3
	madd.s %1, %4, %4
	sqrt.s %1, %1
	"
                  "lui $2, 0x3f80
	mtc1 $2, %0
	nop
	div.s %0, %0, %1"
                  : "=&f"(inv), "=&f"(len) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
out->x=x*inv; out->y=y*inv; out->z=z*inv;
return len;
}
static inline float dot(Vec *a,Vec *b) {
float r;
__asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r): "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z),"f"(a->y));
return r;
}
static inline void scale(Vec *out,Vec *v,float s) {
__asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*out): "r"(v),"f"(s): "$2");
}
void func_0012F328(char *self,Hit *hit,float limit,float factor) {
Vec delta, unit, unit2, pos, transformed;
Physics *physics=(Physics *)(self+0x40c0);
Target *target=(Target *)func_0017D3D0(hit->id);
if(target) {
char *body=(char *)target->body();
if(body) {
Vec *input=&hit->pos;
func_00232A80(physics,&transformed,input);
target->position(input,&pos);
sub(&transformed,&transformed,&pos);
float length=norm(&unit,&transformed);
sub(&delta,physics->pos,(Vec *)(body+16));
norm(&unit2,&delta);
float sign;
if(dot(&unit2,physics->direction)<0.0f) sign=-1.0f;
else sign=1.0f;
func_001B0AC0(self+0x4e54,length*sign/limit);
float force=length*factor;
force*=5.0f;
scale(&unit2,&delta,force);
func_00133718(physics,&unit2);
if(limit<length) func_00155E78(self+0x3fd0,2);
}
}
}
