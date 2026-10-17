/* UNIT 0x00214EB0 0x00215140 */
typedef unsigned int Quad __attribute__((mode(TI)));
typedef struct {float row[4][4];} __attribute__((aligned(16))) Matrix;
typedef struct {char pad0[0xc];float xC;short index;short negX,negY,negZ,posX,posY,posZ;short pad1E;float rotation[4],translation[4];Matrix before,after;union {unsigned short small;unsigned long flags;} xC0;} Obj;
typedef struct {float *values;unsigned char *bits;} Channels;
typedef struct {int x0;Channels x4;Matrix *matrices;} Context;
extern void func_002044A0(Context *,Obj *),func_002045C8(Context *,Obj *),func_00222040(Matrix *,float *),func_00221930(Matrix *,Matrix *,Matrix *);
static inline void copy_matrix(Matrix *dst,Matrix *src) {
 __asm__ volatile("lq $8, 0x0(%1)
	lq $9, 0x10(%1)
	lq $10, 0x20(%1)
	lq $11, 0x30(%1)
	sq $8, 0x0(%0)
	sq $9, 0x10(%0)
	sq $10, 0x20(%0)
	sq $11, 0x30(%0)"::"r"(dst),"r"(src):"$8","$9","$10","$11","memory");
}
void func_00214EB0(Obj *obj,Context *ctx,Matrix **output) {
 Matrix temp;Matrix *matrix;Channels *channels;int index;unsigned long flags;
 index=obj->index;channels=&ctx->x4;
 if(index>=0 && ((channels->bits[(unsigned int)index>>3]>>(index&7))&1))obj->xC=ctx->x4.values[index];
 if(obj->xC!=0.0f && channels) {
 func_002044A0(ctx,obj);
 matrix=(Matrix *)((char *)ctx->matrices+(*(int *)((char *)obj+0xc4)<<6));
 if(output)*output=matrix;
 if(obj->xC0.small){
 if(obj->xC0.flags&2){
 if(obj->negX>=0)obj->rotation[0]=-channels->values[obj->negX];
 if(obj->negY>=0)obj->rotation[1]=-channels->values[obj->negY];
 if(obj->negZ>=0)obj->rotation[2]=-channels->values[obj->negZ];
 }
 if((obj->xC0.flags&1)==1){
 if(obj->posX>=0)obj->translation[0]=channels->values[obj->posX];
 if(obj->posY>=0)obj->translation[1]=channels->values[obj->posY];
 if(obj->posZ>=0)obj->translation[2]=channels->values[obj->posZ];
 }
 func_00222040(matrix,obj->rotation);flags=obj->xC0.flags;
 if(flags&0x10000){
 if(flags&0x20000){func_00221930(&temp,&obj->before,matrix);func_00221930(matrix,&temp,&obj->after);}
 else{func_00221930(&temp,&obj->before,matrix);copy_matrix(matrix,&temp);}
 }else if(flags&0x20000){copy_matrix(&temp,matrix);func_00221930(matrix,&temp,&obj->after);}
 __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0x30(%2)"::"r"((char *)matrix+0x30),"r"(obj->translation),"r"(matrix):"memory");
 func_002045C8(ctx,obj);
 }
 }
}
