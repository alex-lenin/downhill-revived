/* UNIT 0x00171F10 0x00172118 */
typedef struct { float x,y,z,w; } Vec;
#define FIELD(p,o,t) (*(t *)((char *)(p)+(o)))
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
extern float func_00222A68(float,float);
extern float func_00235EA8(float,float,float,float);
extern void func_00221F50(void *,void *);
void func_00171F10(void *obj) {
 Vec delta; Vec *pos; void *sub; float distance,t,angle,step;
 pos=(Vec *)((char *)obj+0x60);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(delta):"r"(pos),"r"((char *)obj+0x70));
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3":"=f"(distance):"f"(delta.x),"f"(delta.y),"f"(delta.z));
 if(distance<FIELD(obj,0x1EC,float)) {
 FIELD(obj,0x70,float)=func_00222A68(-20.0f,20.0f);
 t=func_00222A68(-7.0f,7.0f);
 FIELD(obj,0x1E4,float)=0; FIELD(obj,0x1E8,float)=0;
 FIELD(obj,0x78,float)=t;
 t=func_00222A68(1.0f,2.0f); FIELD(obj,0x1EC,float)=t*t;
 }
 step=func_00235EA8(FIELD(obj,0x1E8,float),0.03f,0.001f,0.0001f);
 FIELD(obj,0x1E8,float)=step;
 FIELD(obj,0x60,float)=func_00235EA8(FIELD(obj,0x60,float),FIELD(obj,0x70,float),step,0.001f);
 FIELD(obj,0x68,float)=func_00235EA8(FIELD(obj,0x68,float),FIELD(obj,0x78,float),FIELD(obj,0x1E8,float),0.001f);
 angle=(FIELD(obj,0x70,float)-FIELD(obj,0x60,float))*0.02617994f;
 t=func_00235EA8(FIELD(obj,0x1E8,float),0.03f,0.01f,0.0001f);
 FIELD(obj,0x1E4,float)=t;
 FIELD(FIELD(obj,4,void *),0xA4,float)=func_00235EA8(FIELD(FIELD(obj,4,void *),0xA4,float),angle,t,0.001f);
 sub=FIELD(obj,4,void *); func_00221F50((char *)sub+0x40,(char *)sub+0xA0);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0":"=m"(delta):"r"(pos),"r"((char *)obj+0x10));
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(FIELD(FIELD(obj,4,void *),0x10,Vec)):"r"((char *)obj+0x50),"r"(&delta));
 COPY_VECTOR(FIELD(obj,0xA0,Vec),(char *)FIELD(obj,4,void *)+0x10);
}
