/* UNIT 0x002327B0 0x00232950 */
typedef struct {float f[4];} Vec __attribute__((aligned(16)));
typedef struct {Vec m[4];} Mat;
typedef struct Obj {char pad0[0x30]; float *x30; Vec *x34; Mat *x38; Vec *x3C; Mat *x40; char pad44[12]; Vec x50,x60,x70; char pad80[0x40]; Vec xC0,xD0,xE0,xF0;} Obj;
extern void func_00232B08(Obj *,Mat *,Vec *),func_00221AB8(Mat *,Mat *,Mat *),func_00221C20(Mat *,Mat *,float),func_00221B80(Mat *,Mat *,Mat *),func_00222800(Mat *),func_00232950(Obj *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(dst):"r"(src):"$2")
#define COPY_MATRIX(dst,src) __asm__ volatile("lq $8,0(%4)\n\tlq $9,16(%4)\n\tlq $10,32(%4)\n\tlq $11,48(%4)\n\tsq $8,%0\n\tsq $9,%1\n\tsq $10,%2\n\tsq $11,%3":"=m"((dst).m[0]),"=m"((dst).m[1]),"=m"((dst).m[2]),"=m"((dst).m[3]):"r"(src):"$8","$9","$10","$11")
#define ADD_SCALED(dst,src,t) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0":"=m"(dst):"r"(&(dst)),"r"(src),"f"(t):"$2")
void func_002327B0(Obj *obj,float dt) {
 Mat mat; Vec v; Mat *rotation; Vec *position;
 if(!(dt<=0.0f)) {
  if(obj->x3C && obj->x40) {COPY_VECTOR(*obj->x3C,obj->x34);COPY_MATRIX(*obj->x40,obj->x38);*obj->x30=dt;}
  rotation=obj->x38;
  func_00232B08(obj,&mat,&obj->xD0);
  position=&obj->x50;
  func_00221AB8(&mat,&mat,rotation);
  ADD_SCALED(obj->x50,&obj->xC0,dt);
  func_00221C20(&mat,&mat,dt);
  func_00221B80(rotation,rotation,&mat);
  func_00222800(rotation);
  ADD_SCALED(obj->x60,&obj->xE0,dt);
  ADD_SCALED(obj->x70,&obj->xF0,dt);
  func_00232950(obj);
  __asm__ volatile("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0":"=m"(v):"r"(obj->x38),"r"((char *)obj+16));
  __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*obj->x34):"r"(position),"r"(&v));
 }
}
