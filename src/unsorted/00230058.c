/* UNIT 0x00230058 0x00230190 */
typedef struct { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct { char pad[0xC0]; float xC0,xC4,xC8; int xCC; } Obj;
extern char *func_0023D040(int);
extern Obj *func_0023DF70(int);
extern float D_00736C80[3][4];
extern float D_0029E334;
void func_00230058(int arg) {
    Vec a={0};
    Vec b,c,d,e;
    float length,scale;
    char *matrix;
    Obj *obj;
    a.y=-1.0f; a.w=1.0f;
    matrix=func_0023D040(arg)+0x40;
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(b) : "r"(&a),"r"(matrix) : "memory");
    c.x=D_00736C80[0][0]*0.03125f;
    c.y=D_00736C80[1][0]*0.03125f;
    c.z=D_00736C80[2][0]*0.03125f;
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(d) : "r"(&c),"r"(&b) : "memory");
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(d.x),"f"(d.y),"f"(d.z));
    scale=1.0f/length;
    e.x=d.x*scale; e.y=d.y*scale; e.z=d.z*scale;
    D_00736C80[2][3]=e.z*32.0f;
    D_00736C80[0][3]=e.x*32.0f;
    D_00736C80[1][3]=e.y*32.0f;
    obj=func_0023DF70(arg);
    obj->xC0=D_0029E334; obj->xC4=D_0029E334; obj->xC8=D_0029E334; obj->xCC=0;
}
