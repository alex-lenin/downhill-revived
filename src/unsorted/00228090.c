/* UNIT 0x00228090 0x00228328 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Effect { Vec pos; float x10,x14,x18,x1C; float x20,x24,x28; short x2C,x2E; } Effect;
extern int D_00720270[];
extern Effect D_00720290[][74];
extern int func_0023D608(void);
extern Vec *func_0023CF78(int);
extern char *func_0023CF58(int);
static inline float Normalize(Vec *out,Vec *in) {
 float length,inv;
 __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1":"=&f"(inv),"=&f"(length):"f"(in->x),"f"(in->y),"f"(in->z),"f"(in->z):"$2");
 out->x=in->x*inv; out->y=in->y*inv; out->z=in->z*inv;
 return length;
}

#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)
	sq $2,%0":"=m"(dst):"r"(src):"$2")
void func_00228090(Vec *input,unsigned int color,int index,float scale,float size) {
 Vec pos,delta,dir; int *count=&D_00720270[index]; Vec *camera,*axis; float length,dot,factor; Effect *out;
 pos=*input;
 if(*count<74 && func_0023D608()<4) {
 camera=func_0023CF78(index);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(delta):"r"(&pos),"r"(camera));
 length=Normalize(&dir,&delta);
 axis=(Vec *)(func_0023CF58(index)+16);
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(dot):"f"(axis->x),"f"(dir.x),"f"(axis->y),"f"(dir.y),"f"(axis->z),"f"(dir.z));
 if(dot>0.0f) {
 out=&D_00720290[index][*count]; COPY_VECTOR(out->pos,&pos);
 factor=scale/length;
 out->x10=size; out->x14=factor; out->x18=factor*dot;
 if(factor>0.00390625f) {
 out->x20=(float)(color&0xff)*0.0039215684f;
 out->x24=(float)((color>>8)&0xff)*0.0039215684f;
 out->x28=(float)((color>>16)&0xff)*0.0039215684f;
 out->x2C=0; out->x2E=0;
 D_00720270[index]++;
 }
 }
 }
}
