/* UNIT 0x0010DEF8 0x0010E150 */
#include "vector.h"
typedef Vector4 Vec;
typedef struct Rider { char pad0[0x4180]; Vec x4180; char pad4190[0x379C]; char *x792C; } Rider;
typedef struct Obj { Rider *x0; char pad4[0x7C]; Vec *x80; char pad84[0x300]; int x384; char pad388[0x24]; int x3AC, x3B0; char pad3B4[0x7C]; int x430; char pad434[0xC]; unsigned long x440; } Obj;
extern void func_0010B770(Obj *);
extern int func_00138560(void *);
extern float func_0017C008(void);
void func_0010DEF8(Obj *obj) {
    Vec diff, normal;
    float x, y, z, sum, inv, dot, length, speed;
    int value;
    void *check;
    float *velocity, *otherVel;
    if (obj->x80 && obj->x384 < obj->x430) {
        velocity = (float *)&obj->x0->x4180;
        check = (char *)obj->x0 + 0x40C0;
        x = velocity[0];
        y = velocity[1];
        z = velocity[2];
        __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(sum) : "f"(x), "f"(y), "f"(z));
        __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(sum));
        normal.x = x * inv;
        normal.y = y * inv;
        normal.z = z * inv;
        __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(&diff), "r"(obj->x80), "r"(obj->x0->x792C + 16) : "memory");
        __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "f"(diff.x), "f"(normal.x), "f"(diff.y), "f"(normal.y), "f"(diff.z), "f"(normal.z));
        if (dot < 0.0f) {
            obj->x80 = 0;
            obj->x440 &= ~(1UL << 46);
            func_0010B770(obj);
            return;
        }
        if (!(func_00138560(check) & 0xFF)) {
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(diff.x), "f"(diff.y), "f"(diff.z));
            if (length < 40.0f) {
                otherVel = (float *)&obj->x0->x4180;
                __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(speed) : "f"(otherVel[0]), "f"(otherVel[1]), "f"(otherVel[2]));
                speed *= 0.6818182f;
                value = (int)(speed * func_0017C008());
                if (obj->x3AC && obj->x3AC < value) {
                    obj->x440 &= ~(1UL << 35);
                    obj->x440 &= ~(1UL << 34);
                    if (obj->x3AC + 2 < value) obj->x440 |= 1UL << 38;
                }
                if (obj->x3B0 && value < obj->x3B0) {
                    obj->x440 &= ~(1UL << 38);
                    obj->x440 |= 1UL << 34;
                    if (value < obj->x3B0 - 2) obj->x440 |= 1UL << 35;
                }
            }
        }
    }
}
