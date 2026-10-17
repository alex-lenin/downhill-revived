/* UNIT 0x001B6668 0x001B6708 */
#include "rider.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef Vector4 Vec4;

typedef Matrix4 Matrix;

typedef SceneNode Node;

typedef Rider Obj;

extern void func_00218278(Node *node, int arg);

/* 64-byte matrix copy (as sceVu0CopyMatrix, inline asm in the original) */
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
                      : "=m"((dst)->row[0]), "=m"((dst)->row[1]), "=m"((dst)->row[2]), "=m"((dst)->row[3]) \
                      : "r"(src) : "$8", "$9", "$10", "$11")

/* 128-bit vector copy (inline asm in the original) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"((dst)->x) : "r"(src) : "$2")

void func_001B6668(Obj *obj) {
    if (obj->x7930 != 0 && obj->x7930->x4 != 0) {
        if (obj->x7A5C == 3) {
            func_00218278(obj->x7930, obj->x7934);
        }
        COPY_MATRIX(&obj->x7930->matrix, &obj->bodyNode->matrix);
        COPY_VEC(&obj->x7930->position, &obj->bodyNode->position);
        obj->x7930->position.z -= obj->x7AC0;
    }
}
