/* UNIT 0x00120838 0x001208D8 */
#include "rider_geometry.h"
typedef struct Target { char pad[0x190]; int x190; } Target;
typedef struct Obj { char pad0[0x10]; char *x10; char pad1[0x20]; Target *x34; char pad2[0x224]; short x25c; } Obj;
extern void func_0011E568(Obj *,int);
extern void func_00166948(Target *);
extern void func_00223348(void *,void *);

extern void func_00165560(Target *);
void func_00120838(Obj *obj) {
    float zero[4] __attribute__((aligned(16)));
    Target *target=obj->x34;
    if(target) {
        if((target->x190 ^ 1)==0 || obj->x25c>=121) {
            __asm__ volatile("sqc2 $vf0,0(%0)" : : "r"(zero) : "memory");
            func_0011E568(obj,8);
            func_00166948(obj->x34);
            obj->x34=0;
            func_00223348((char*)obj+0x20,obj->x10+0x40);
            func_00121348((GroundRider *)(obj), (void *)(obj->x10+0x10), 1);
        } else func_00165560(target);
    }
}
