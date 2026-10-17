/* UNIT 0x0012D510 0x0012D628 */
struct Target { char pad[5]; unsigned char x5; char pad6[10]; float pos[4]; };
struct Flags { char pad[28]; int x1C; };
struct Obj { char pad[0x1E84]; struct Flags *x1E84; char pad2[0x3514-0x1E88]; struct Target *x3514; char pad3[0x3530-0x3518]; char x3530[0xF1]; unsigned char x3621; };
extern int func_0023D608(void);
extern float *func_0023CF78(int);
extern float D_0029C284[];
void func_0012D510(struct Obj *obj) {
 float delta[4] __attribute__((aligned(16)));
 struct Target *target = obj->x3514;
 int hidden = 0, i, count;
 char *base;
 float nearest, distance, x,y,z;
 if (target->x5) {
  i=0; count=func_0023D608(); base=(char*)obj+0x3530; nearest=D_0029C284[0];
  if (count>0) {
   float *pos=target->pos;
   do {
    float *other=func_0023CF78(i);
    __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(delta): "r"(pos),"r"(other));
    x=delta[0]; y=delta[1]; z=delta[2];
    __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(distance): "f"(x),"f"(y),"f"(z));
    __asm__ volatile("min.s %0, %1, %2" : "=&f"(distance): "f"(nearest),"f"(distance));
    nearest=distance;
    i++;
   } while(i<count);
  }
  if(nearest>4225.0f) hidden=1;
 } else { hidden=1; base=(char*)obj+0x3530; }
 obj->x1E84->x1C=(obj->x1E84->x1C & ~8) | (hidden<<3);
 base[0xF1]=hidden;
}
