/* UNIT 0x00123AD0 0x00123DB0 */
#include "model.h"
#include "rider_geometry.h"
struct Vec { float x,y,z,w; float *func_00261718(); float *func_00261710(); };
struct Pair { Vec a,b; float *func_00261718(); float *func_00261710(); };
struct PackedVec { float x,y,z,w; void func_00203E10(float); } __attribute__((packed));
class Object { public: virtual void v0(); virtual void v1(); virtual void v2(); virtual char *get(); };
struct State { char pad0[0x792C]; char *x792C; };
struct Extra { char pad0[0xE4]; int xE4; };
struct Obj { char pad0[8]; int x8; char padC[0x28]; float x34,x38; char pad3C[4]; int x40[60]; Object *x130; char pad134[0x10]; State *x144; char pad148[0x14]; Extra *x15C; char pad160[0x80]; Vec x1E0,x1F0; };
extern void func_00123DB0(Obj *,char *);
extern void func_00124300(Obj *);
extern void func_001258A0(Obj *,float);

extern void func_00203E10(void *,void *,float);
extern void func_00211280(char *);

extern void func_00236558(Vec *,PackedVec *,float);
extern float func_00238F40();
extern float *func_00261028(Vec *);
extern float *func_00261710(void *);
extern float *func_00261718(void *);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void copyXYZ(Vec *dst,const void *src) {float w=dst->w; COPY_VECTOR(*dst,src); dst->w=w;}
static inline void setRadius(Pair *p,float f) {*p->func_00261718()=f;}
static inline void setSpeed(Pair *p,float f) {*p->func_00261710()=f;}
void func_00123AD0(Obj *self, Vec *pos, int mode) {
    Vec diff; PackedVec target; Pair vectors; float speed; 
    if (!self->x144) return;
    {
        
        __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %2" : : "r"(pos),"r"(self->x130->get()+0x10),"m"(diff) : "memory");
        if (mode) {
            if (func_00192BA8((int)(func_002623C0(self->x130))) == 3) {
                self->x34=5.866667f; self->x38=5.866667f; func_001258A0(self,5.866667f);
            } else {
                self->x34=5.866667f; self->x38=5.866667f; func_001258A0(self,5.866667f);
            }
            char *p=(char *)self->x130 + (self->x40[self->x8]*16+16);
            target=*(PackedVec *)p;
            target.func_00203E10(1.0f);
        } else {
            self->x34=29.333334f; self->x38=29.333334f; func_001258A0(self,29.333334f);
            char *p=(char *)self->x130 + (self->x40[self->x8]*16+16);
            target=*(PackedVec *)p;
            target.func_00203E10(1.0f);
        }
        copyXYZ(&vectors.a,self->x144->x792C+16);
        copyXYZ(&vectors.b,(char *)self->x144+0x4180);
        setRadius(&vectors,0.25f);
        pos=&self->x1E0;
        speed=*func_00261028(pos);
        if (mode != 0) speed = 0.5f * speed;
        setSpeed(&vectors,speed);
        func_002362E8((float *)(&vectors), (Vector4 *)(pos), (Vector4 *)(&self->x1F0), (Vector4 *)(&target));
        func_00236558(pos,&target,func_00238F40());
        func_00123DB0(self,self->x130->get()+0x40);
        char *p=self->x130->get(); COPY_VECTOR(*(Vec *)(p+16),pos);
        func_00124300(self);
        func_00211280(self->x130->get());
        self->x15C->xE4=0;
    }
}
