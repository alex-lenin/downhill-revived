/* UNIT 0x0012A9B8 0x0012AA58 */
#include "rider_geometry.h"
typedef struct Target { char pad[0x190]; int x190; } Target;
typedef struct Obj { char pad0[4]; char *x4; char pad1[0x168]; Target *x170; int x174; } Obj;
extern void func_00129860(Obj *,int);
extern void func_00166948(Target *);
extern void func_00223348(void *,void *);

extern void func_00165560(Target *);
void func_0012A9B8(Obj *obj) {
    float zero[4] __attribute__((aligned(16)));
    Target *target=obj->x170;
    if(target) {
        if((target->x190 ^ 1)==0 || obj->x174>=121) {
            __asm__ volatile("sqc2 $vf0,0(%0)" : : "r"(zero) : "memory");
            func_00129860(obj,8);
            func_00166948(obj->x170);
            obj->x170=0;
            func_00223348((char*)obj+0x10,obj->x4+0x40);
            func_0012AD80((GroundActor *)(obj), (void *)(obj->x4+0x10), 1);
        } else func_00165560(target);
    }
}
