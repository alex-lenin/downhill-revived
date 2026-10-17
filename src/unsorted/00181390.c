/* UNIT 0x00181390 0x00181618 */
typedef struct Other { unsigned int *x0; char x4; unsigned char x5; } Other;
typedef struct Obj { char pad0[8]; Other *x8; char padC[0x144]; float x150[4]; char pad160[0x40]; float x1A0; } Obj;
typedef struct Attachment { void *x0; char pad4[0xC]; float x10[4]; } Attachment;
extern void func_002087C0(float *);
extern void func_00222140(void *,float *);
extern int D_004B2840[];
void func_00181390(Obj *obj) {
 float length,angle; float *velocity=obj->x150;
 float *part; int count,mode; unsigned int id;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(velocity[0]),"f"(velocity[1]),"f"(velocity[2]) : "memory");
 obj->x1A0=length*0.6818182f*0.024444444f;
 if(obj->x8->x5) {
  id=*obj->x8->x0>>18;
  switch(id) {
  case 0x4EC:
   if(D_004B2840[0]!=5) {angle=0.10471976f;break;}
  case 0x56E:angle=0.05235988f;break;
  case 0x58A: case 0x5A2: case 0x5AE: case 0x5B4:case 0x5A8:case 0x596:case 0x59C:case 0x590:case 0x584:case 0x55A:
   angle=0.2443461f;break;
  case 0x57A:angle=0.06981317f;break;
  case 0x574:case 0x580:angle=0.17453292f;break;
  default:angle=0.10471976f;break;
  }
  part=(float *)((char *)obj+0x20);count=5;
  do {
   if(*(void **)(part-4)) {
    part[0]-=angle*obj->x1A0*2.7272727f;
    func_002087C0(part);
    func_00222140((char *)(*(void **)(part-4))+0x10,part);
   }
   count--;part+=8;
  }while(count>=0);
 }
}
