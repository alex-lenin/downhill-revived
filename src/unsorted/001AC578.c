/* UNIT 0x001AC578 0x001AC620 */
#include "camera.h"
#include "rider.h"



extern void func_001634E0(void *);

extern void func_001CFF20(void *);

void func_001AC578(RiderComponent *obj) {
    if (obj->node && obj->rider) {
        if (!obj->x2160) {
            /* copy position vector and matrix from the parent's transform */
            __asm__ volatile ("lq $2, 0(%0)\n\tsq $2, 0x10(%1)"
                : : "r"(&obj->rider->bodyNode->position), "r"(obj->node) : "$2", "memory");
            __asm__ volatile ("lq $8, 0x0(%0)\n\tlq $9, 0x10(%0)\n\tlq $10, 0x20(%0)\n\tlq $11, 0x30(%0)\n\t"
                              "sq $8, 0x40(%1)\n\tsq $9, 0x50(%1)\n\tsq $10, 0x60(%1)\n\tsq $11, 0x70(%1)"
                : : "r"(&obj->rider->bodyNode->matrix), "r"(obj->node) : "$8", "$9", "$10", "$11", "memory");
        }
        func_001634E0((char *)obj + 0x1054);
    }
    if (obj->x2160) func_001B0D38((RiderCamera *)((char *)obj + 0x16F0));
    func_001CFF20((char *)obj + 0x2168);
}
