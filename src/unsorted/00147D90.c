/* UNIT 0x00147D90 0x00147F00 */
#include "object_registry.h"
#include "scene.h"
typedef struct { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct { float m[4][4]; } Mat __attribute__((aligned(16)));
typedef SceneNode Node;
typedef struct Obj { int x0,x4; Node *x8; int xC; Vec x10; Mat x20; Vec x60; Mat x70; } Obj;
extern void func_00221D58(Mat *,void *);
extern Node *func_00208FB8(void);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MATRIX(dst, src) __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\tsq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" : "=m"(((Mat *)&(dst))->m[0]),"=m"(((Mat *)&(dst))->m[1]),"=m"(((Mat *)&(dst))->m[2]),"=m"(((Mat *)&(dst))->m[3]) : "r"(src) : "$8","$9","$10","$11")
Obj *func_00147D90(Obj *obj,int id,Vec *pos,void *rot,int arg4,int arg5) {
 Mat mat; Vec *pos_out=&obj->x10; Mat *rot_out;
 obj->x4=arg4; obj->xC=arg5;
 COPY_VECTOR(*pos_out,pos);
 COPY_VECTOR(obj->x60,pos);
 rot_out=&obj->x20;
 func_00221D58(&mat,rot);
 COPY_MATRIX(*rot_out,&mat);
 COPY_MATRIX(obj->x70,&mat);
 obj->x8=func_00208FB8();
 COPY_VECTOR(obj->x8->position,pos_out);
 COPY_MATRIX(obj->x8->matrix,rot_out);
 COPY_VECTOR(obj->x8->x90,pos_out);
 COPY_MATRIX(obj->x8->xC0,rot_out);
 obj->x8->resourceId=id;
 obj->x8->x4=1;
 obj->x8->flags |= (SceneNodeFlags)1 << 57;
 obj->x8->xB4=RegisterObjectRetail((void *)(obj), (unsigned int)(id));
 obj->x8->flags |= (SceneNodeFlags)1 << 58;
 return obj;
}
