/* UNIT 0x001B0EA8 0x001B0F28 .sdata=0x29D3F8:0x29D3FC */
#include "camera.h"
#include "rider.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef RiderCamera Obj;

float D_0029D3F8 = 0.88f;

void func_001B0EA8(Obj *obj)
{
    Vector4 *a = &obj->xA50;

    /* copy the rider's vector, then blend: b = b * k + a * (1 - k); a = b */
    __asm__ volatile ("lq $2, 0(%1)
	sq $2, %0"
                      : "=m"(obj->xA50) : "r"(&*(Vector4 *)((char *)obj->xA10->rider + 0x41F0)) : "$2", "memory");
    __asm__ volatile ("lqc2 $vf11, 0(%1)
	mfc1 $2, %2
	qmtc2 $2, $vf12
	vmulx.xyz $vf11, $vf11, $vf12x
	sqc2 $vf11, %0"
                      : "=m"(obj->xA50) : "r"(a), "f"(1.0f - D_0029D3F8) : "$2", "memory");
    __asm__ volatile ("lqc2 $vf11, 0(%1)
	mfc1 $2, %2
	qmtc2 $2, $vf12
	vmulx.xyz $vf11, $vf11, $vf12x
	sqc2 $vf11, %0"
                      : "=m"(obj->xA60) : "r"(&obj->xA60), "f"(D_0029D3F8) : "$2", "memory");
    __asm__ volatile ("lqc2 $vf11, 0(%1)
	lqc2 $vf12, 0(%2)
	vadd.xyz $vf11, $vf11, $vf12
	sqc2 $vf11, %0"
                      : "=m"(obj->xA60) : "r"(&obj->xA60), "r"(a) : "memory");
    __asm__ volatile ("lq $2, 0(%1)
	sq $2, %0"
                      : "=m"(obj->xA50) : "r"(&obj->xA60) : "$2", "memory");
}
