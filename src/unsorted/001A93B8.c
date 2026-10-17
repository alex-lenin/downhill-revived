/* UNIT 0x001A93B8 0x001A9508 */
typedef unsigned int Vec __attribute__((mode(TI)));
struct Obj { char pad0[0x28]; int x28; char pad2C[0x10]; int x3C; float x40; };
struct Point { float x0,x4,x8,xC; float x10,x14; char x18; signed char x19; };
#include "rider.h"
struct Global { char pad0[0x80]; Vec x80; char pad90[0x2B4-0x90]; struct Rider *x2B4; char pad2B8[0x2E4-0x2B8]; int x2E4; char pad2E8[0x348-0x2E8]; int x348; char pad34C[0x360-0x34C]; int x360; };
extern struct Global D_0037EA60;
extern void func_0023D898(int,float *,float *);
extern void func_0023D8D8(int,float,float);
extern void func_0014B4E8(struct Global *,void *);
extern void func_00148560(struct Global *,int);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_001A93B8(struct Obj *obj, struct Point *point, int index) {
    float y,x;
    struct Rider *rider = D_0037EA60.x2B4;
    obj->x3C = D_0037EA60.x360;
    if (obj->x28 == 0) func_0023D898(0,&obj->x40,&y);
    else func_0023D898(0,&x,&y);
    func_0023D8D8(0,point->x14,y);
    if (index >= 0) {
        void *entry = (char *)obj + ((index << 5) + 0x9D0);
        COPY_VECTOR(D_0037EA60.x80,point);
        func_0014B4E8(&D_0037EA60,entry);
        D_0037EA60.x348=1;
        func_00148560(&D_0037EA60,4);
    } else if (point->x19 == 2) {
        if (point->x8 < rider->bodyNode->position.z - 10.0f) {
            D_0037EA60.x360=5;
            D_0037EA60.x2E4=0;
            func_00148560(&D_0037EA60,1);
        }
    } else {
        COPY_VECTOR(D_0037EA60.x80,point);
        func_00148560(&D_0037EA60,4);
    }
    obj->x28=1;
}
