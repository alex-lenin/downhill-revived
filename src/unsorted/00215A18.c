/* UNIT 0x00215A18 0x00215B58 */
typedef struct Vec {float x,y,z,w;} __attribute__((aligned(16))) Vec;
typedef struct Matrix {Vec v[4];} Matrix;
typedef struct Obj {char pad0[0x10]; Vec x10; char pad20[0x20]; Matrix x40; char pad80[0x34]; unsigned short xb4;} Obj;
typedef struct Entry {Obj *x0; int x4; Matrix *x8; unsigned short xc,xe; int x10;} Entry;
extern char D_0029E19C;
extern int D_0029E194,D_0029E198;
extern Entry D_005156A8,D_00514AF0[];
extern Matrix D_005156C0;
extern void func_00221888(Matrix*,Vec*,Vec*);
static inline void CopyMatrix(Matrix *out, Matrix *in) {
 __asm__ volatile("lq $8, 0x0(%4)
	lq $9, 0x10(%4)
	lq $10, 0x20(%4)
	lq $11, 0x30(%4)
	sq $8, %0
	sq $9, %1
	sq $10, %2
	sq $11, %3" : "=m"(out->v[0]),"=m"(out->v[1]),"=m"(out->v[2]),"=m"(out->v[3]) : "r"(in) : "$8","$9","$10","$11","memory");
}
extern void *memcpy(void*,const void*,unsigned int);
void func_00215A18(Obj *obj, int value) {
 Matrix matrix; Vec vec; Entry *entry;
 D_0029E19C=value;
 D_005156A8.x8=&D_005156C0;
 D_005156A8.xe=1;
 D_0029E194=0;
 D_005156A8.xc=obj->xb4;
 D_005156A8.x10=0;
 D_0029E198=0;
 CopyMatrix(&matrix,&obj->x40);
 __asm__ volatile("lqc2 $vf11, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(vec) : "r"(&vec),"r"(&obj->x10),"f"(-1.0f) : "$2", "memory");
 func_00221888(&matrix,&vec,&matrix.v[3]);
 CopyMatrix(&D_005156C0,&matrix);
 entry=&D_00514AF0[D_0029E194++];
 entry->x0=obj;
 entry->x4=1;
 entry->x8=D_005156A8.x8;
 entry->x10=D_005156A8.x10;
 entry->xe=D_005156A8.xe;
 entry->xc=D_005156A8.xc;
}
