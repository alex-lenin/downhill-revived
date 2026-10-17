/* UNIT 0x00137DE8 0x00137E68 .sdata=0x29C3B4:0x29C3B8 */
float D_0029C3B4=250.0f;
struct Entry { char pad[8]; float x8; char padc[20]; };
extern struct Entry D_00768098[];
extern void func_00231C20(void *, float *);
void func_00137DE8(char *obj) {
 float tmp[4] __attribute__((aligned(16)));
 int index=*(unsigned short *)(obj+0x206)&31;
 struct Entry *entry;
 float scale;
 if(index>=32) entry=D_00768098;
 else entry=&D_00768098[index];
 scale=-D_0029C3B4*entry->x8*0.01f;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)" : : "r"(tmp),"r"(obj+0xc0),"f"(scale) : "$2","memory");
 func_00231C20(obj,tmp);
}
