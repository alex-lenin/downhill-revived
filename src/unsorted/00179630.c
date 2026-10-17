/* UNIT 0x00179630 0x001797A0 */
typedef struct {float v[4];} Vec __attribute__((aligned(16)));
typedef struct {char pad0[0x10];Vec x10;char pad20[0x20];Vec x40;} Frame;
typedef struct {char pad0[0x792C];Frame *x792C;char pad7930[0x12C];int x7A5C;char pad7A60[0x170];} Rider;
typedef struct {Rider riders[1];char pad7BD0[0x50000-0x7BD0];struct {char pad0[0x1A8];int count;} tail;} Global;
typedef struct {char pad0[0x1164];Rider *x1164;char pad1168[0x1B8];int x1320;} Obj;
extern Global D_002AF2C0;
extern float D_0029CA40[];
extern void func_00221888(Vec *,Vec *,Vec *);
void func_00179630(Obj *o){Vec diff,result;float best,distance;int i=0;Rider *r;
 o->x1320=0;best=D_0029CA40[0];
 for(;i<D_002AF2C0.tail.count;i++){r=&D_002AF2C0.riders[i];
 if(r->x7A5C && r!=o->x1164){
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(diff) : "r"(&r->x792C->x10),"r"(&o->x1164->x792C->x10));
 func_00221888(&o->x1164->x792C->x40,&diff,&result);
 if(result.v[1]<=0.0f && result.v[1]>-20.0f && result.v[0]<20.0f && result.v[0]>-20.0f){
 distance=result.v[0]*result.v[0]+result.v[1]*result.v[1]+result.v[2]*result.v[2];o->x1320++;if(distance<best)best=distance;
 }
 }
 }
}
