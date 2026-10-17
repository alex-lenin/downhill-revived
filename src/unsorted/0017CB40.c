/* UNIT 0x0017CB40 0x0017CC58 */
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Node { long flags; float mass; char padC[4]; Vec x10,x20,x30,x40; struct Node *x50; int pad54; long x58; Vec x60,x70,x80,x90,xA0; } Node;
typedef struct Owner { char pad[0x20]; Node *x20; } Owner;
typedef struct Obj { int pad; float x4,x8,xC; char pad10[8]; Owner *volatile x18; volatile int x1C; Node *x20; } Obj;
#define SCALE(dst,src,s) __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(src), "f"(s) : "$2")
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_0017CB40(Obj *obj, Node *target, int value, Vec *pos) {
 Node *node;
 obj->x1C=value; obj->x18=(Owner*)target;
 obj->x4=pos->x; obj->x8=pos->y; obj->xC=pos->z;
 node=((Owner*)target)->x20; target=0;
 if ((node->flags & 63)==4) {
  target=node->x50;
  if ((target->flags & 63)==22) {
   if (!(target->x58 & 0x1000000000L)) {
    target->x58 |= 0x1000000000L;
    __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(target->xA0) : "r"(&node->x40), "r"(pos));
    target->xA0.w=0; target->xA0.z-=10.0f;
    SCALE(target->x60,&node->x10,target->x20.x / target->mass);
    COPY_VECTOR(target->x70,&node->x30);
    SCALE(target->x80,&node->x20,-1.0f);
    __asm__ volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&target->x90) : "memory");
   }
   node->flags=(node->flags & ~63L)|9;
  } else target=0;
 }
 obj->x20=target;
}
