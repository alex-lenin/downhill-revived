/* UNIT 0x0016FA88 0x0016FBB0 */
typedef struct { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct { char pad0[0x14]; unsigned short x14; } Node;
typedef struct { char pad0[0x80]; void *x80; } Obj;
extern char D_00412510[];
extern Vec *func_00164170(void *,Node *,Vec *);
extern Node *func_00163DA0(void *,Node *,int);
void func_0016FA88(Obj *obj,Node *node,Vec *out,int count) {
 Vec diff;
 int done=0;
 float half=0.5f;
 do {
 if(count<0) {
 done=1;
 if(node) {
 Vec *first=func_00164170(D_00412510,node,0);
 if(first) {
 Vec *second=func_00164170(D_00412510,node,first);
 if(second) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(diff) : "r"(second),"r"(first));
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,%2\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(first),"m"(diff),"f"(half) : "$2");
 }
 }
 }
 } else {
 node=func_00163DA0(obj->x80,node,0);
 if(node) count -= (node->x14 >> 1) & 1;
 else done=1;
 }
 } while(!done);
}
