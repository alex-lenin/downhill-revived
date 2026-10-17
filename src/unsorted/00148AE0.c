/* UNIT 0x00148AE0 0x00148B68 */
typedef struct Parent {
 char pad0[31020];
 char * x792C;
} Parent;
typedef struct Obj {
 char pad0[160];
 float xA0[4];
 char padb0[516];
 Parent * x2B4;
 char pad2b8[164];
 int x35C;
} Obj;
extern char *func_0023D040(int);
extern void func_00221ED0(void *,void *);
void func_00148AE0(Obj *p) {
 float tmp[4]; char *result=func_0023D040(p->x35C); Parent *parent=p->x2B4;
 char *matrix=parent->x792C+0x40;
 __asm__ volatile("lqc2 $vf11,0(%0)
	lqc2 $vf12,0(%1)
	lqc2 $vf13,16(%1)
	lqc2 $vf14,32(%1)
	vmulax.xyz ACC,$vf12,$vf11x
	vmadday.xyz ACC,$vf13,$vf11y
	vmaddz.xyz $vf11,$vf14,$vf11z
	sqc2 $vf11,0(%2)" : : "r"(p->xA0),"r"(matrix),"r"(tmp) : "memory");
 __asm__ volatile("lqc2 $vf11,0(%0)
	lqc2 $vf12,0(%1)
	vadd.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,16(%2)" : : "r"(parent->x792C+16),"r"(tmp),"r"(result) : "memory");
 func_00221ED0(result+64,p->x2B4->x792C+64);
}
