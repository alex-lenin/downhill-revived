/* UNIT 0x00172A30 0x00172BD0 */
typedef unsigned int Vec __attribute__((mode(TI)));
typedef struct Node { Vec pos,extra; } Node;
typedef struct Child { char pad[0x10]; Vec pos; char pad20[0x20]; float matrix[24]; float dir[4]; } Child;
typedef struct Obj { int x0,x4,x8; Child *xc; char pad10[0x10]; Vec pos,extra; Node *x40,*x44,*x48; int x4c; float x50[4],x60,x64; char pad68[8]; float x70[4]; char pad80[0x3c]; float xbc; } Obj;
extern char D_003B01E0[];
extern Node *func_00163E98(void *,Node *);
extern Node *func_00163F18(void *,Node *);
extern void func_00222578(float *,void *,void *);
extern void func_00221F50(float *,float *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(dst):"r"(src):"$2")
void func_00172A30(Obj *obj,Node *node) {
 Node *next; Child *child; float *dir; float x,y,z,len,inv,zero;
 if(obj->xc) {
 obj->x44=node; obj->pos=node->pos; obj->extra=node->extra;
 if(obj->x4==1) obj->x48=func_00163E98(D_003B01E0,node); else obj->x48=func_00163F18(D_003B01E0,node);
 COPY_VECTOR(obj->xc->pos,&obj->pos);
 next=obj->x48; obj->x40=obj->x44; obj->x44=next; obj->pos=next->pos; obj->extra=next->extra;
 if(obj->x4==1) obj->x48=func_00163E98(D_003B01E0,next); else obj->x48=func_00163F18(D_003B01E0,next);
 zero=0.0f; child=obj->xc; func_00222578(child->dir,&child->pos,&obj->pos);
 { Child *c=obj->xc; c->dir[1]=zero; c->dir[0]=zero; }
 child=obj->xc; func_00221F50(child->matrix,child->dir);
 dir=obj->x50;
 __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0x50(%2)"::"r"(&obj->pos),"r"(&obj->xc->pos),"r"(obj):"memory");
 x=dir[0]; y=dir[1]; z=dir[2];
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3":"=f"(len):"f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2":"=&f"(inv):"f"(1.0f),"f"(len));
 obj->x50[0]=x*inv; dir[1]=y*inv; dir[2]=z*inv;
 { float *v=obj->x70; obj->xbc=zero; obj->x70[0]=zero; v[1]=zero; v[2]=zero; obj->x64=zero; obj->x60=zero; }
 }
}
