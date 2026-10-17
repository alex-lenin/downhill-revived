/* UNIT 0x00212500 0x002125A8 */
#include "spatial.h"
void func_00212500(SpatialReference *p,const Vector4 *input,Vector4 *output,float t) {
Vector4 pos; Matrix4 matrix; void *translation;
if(p->node) {
func_00221888(p->node->matrix,input,output);
translation=&p->node->matrix->row[3];
__asm__ volatile ("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(output),"r"(translation) : "memory");
return;
}
if(p->actor) {
func_00209460(p->actor,&pos,&matrix,t);
__asm__ volatile ("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%2)" : : "r"(input),"r"(&pos),"r"(output) : "memory");
func_00221888(&matrix,output,output);
}
}
