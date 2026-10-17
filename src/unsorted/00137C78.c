/* UNIT 0x00137C78 0x00137DE8 */
struct State { char pad0[0x38]; float *x38; char pad3C[0x84]; float xC0[4]; char padD0[0x160]; float x230[4]; float x240,x244,x248; };
extern float func_00238F38(void);
extern int D_0029C838;
extern void func_00223348(float *,float *);
extern void func_00222E80(float *,float *,float);
extern void func_00223210(float *,float *,float *);
extern void func_002235B0(float *,float *,float *);
extern void func_002230B0(float *,float *);
extern void func_002232C8(float *,float *);
extern void func_00231A78(struct State *,float *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_00137C78(struct State *s) {
 float q[4],rot[4],pos[4],mat[16]; float scale;
 scale=60.0f/func_00238F38();
 func_00223348(q,s->x38);
 if(s->x248!=0.0f) {
  func_00222E80(rot,s->x230,-s->x248*scale);
  func_00223210(q,q,rot);
  if(D_0029C838) { func_002235B0(pos,q,s->xC0); COPY_VECTOR(s->xC0,pos); }
 }
 if(s->x244!=0.0f) { func_00222E80(rot,s->x38+4,-s->x244*scale); func_00223210(q,q,rot); }
 if(s->x240!=0.0f) { func_00222E80(rot,s->x38,-s->x240*scale); func_00223210(q,q,rot); }
 func_002230B0(q,q); func_002232C8(mat,q); func_00231A78(s,mat);
}
