/* UNIT 0x0016E678 0x0016E8D8 */
struct Vec {float x,y,z,w;};
struct Vtable { char pad[32]; short delta,pad22; char *(*method)(void *); };
struct Node { struct Vtable *vt; };
struct Part {char pad0[8];int x8;char padc[32];int x2c;char pad30[256];struct Node *x130;char pad134[156];struct Vec x1d0;};
struct Obj {char pad[0x1f0];struct Vec x1f0;};
extern void func_00126B20(void *,int),func_0016E8D8(void *,void *,void *);
static inline char *getPos(struct Part *part) {struct Node *node=part->x130; if(node) return node->vt->method((char *)node+node->vt->delta)+16; return 0;}
#define SUB(d,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(d):"r"(a),"r"(b))
#define SQUARE(len,x,y,z) __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3":"=f"(len):"f"(x),"f"(y),"f"(z))
#define LENGTH(len,x,y,z) __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0":"=&f"(len):"f"(x),"f"(y),"f"(z))
static inline void normalize(struct Vec *v) { float x=v->x,y=v->y,z=v->z,len,inv; SQUARE(len,x,y,z); __asm__ volatile("rsqrt.s %0,%1,%2":"=&f"(inv):"f"(1.0f),"f"(len)); v->x=x*inv;v->y=y*inv;v->z=z*inv; }
static inline float normalizeLength(struct Vec *v) {float x=v->x,y=v->y,z=v->z,len,inv;int bits; __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1":"=&f"(inv),"=&f"(len),"=r"(bits):"f"(x),"f"(y),"f"(z));v->x=x*inv;v->y=y*inv;v->z=z*inv;return len;}
int func_0016E678(struct Obj *obj,struct Part *part) {
 struct Vec direction,delta;
 struct Vec *pos=&obj->x1f0;
 char *p;
 float x,y,z,len,inv,dot;
 int result=0;
 SUB(direction,pos,getPos(part));
 normalize(&direction);
 if(part->x2c) {
 pos=&part->x1d0;
 SUB(delta,pos,getPos(part));
 len=normalizeLength(&delta);
 if(len<6.0f) goto activate;
 {struct Node *node=part->x130;struct Vec *v=(struct Vec *)(node->vt->method((char *)node+node->vt->delta)+0x50);
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(dot):"f"(delta.x),"f"(v->x),"f"(delta.y),"f"(v->y),"f"(delta.z),"f"(v->z));}
 if(dot>0.7f) {result=1;if(part->x8!=1) func_00126B20(part,1);part->x2c=0;}
 } else {
 p=getPos(part);
 SUB(delta,p,&part->x1d0);
 LENGTH(len,delta.x,delta.y,delta.z);
 if(len<6.0f) {activate: func_0016E8D8(obj,part,&direction);part->x2c=1;}
 }
 return result;
}
