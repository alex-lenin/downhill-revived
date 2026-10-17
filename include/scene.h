#ifndef DHD_SCENE_H
#define DHD_SCENE_H

#include "vector.h"
typedef unsigned long SceneNodeFlags __attribute__((mode(DI)));

/* Instance prefix initialized by 002182D0. The first word also carries a
   resource ID/address; rider code uses a linked transform view of that word. */
typedef struct SceneNode {
    union {
        SceneNodeFlags flags;
        struct {
            union { struct SceneNode *x0; void *resource; unsigned int resourceId; unsigned int *resourceWords; };
            unsigned char x4, x5;
            signed char x6;
            unsigned char x7;
        };
        struct { SceneNodeFlags unused:56, b56:1, b57:1, b58:1, b59:1, b60:1, b61:1, b62:1, b63:1; };
    };
    float x8, xC;
    Vector4 position;
    float x20, x24, x28, x2C;
    Vector4 x30;
    Matrix4 matrix;
    int x80;
    short x84, x86, x88, x8A;
    char pad8C[4];
    Vector4 x90, xA0;
    float xB0;
    short xB4, xB6;
    float xB8;
    unsigned int xBC;
    Matrix4 xC0;
} SceneNode;

typedef char SceneNode_size_check[sizeof(SceneNode) == 0x100 ? 1 : -1];
#define DHD_SCENE_OFFSET(field) ((unsigned int)&((SceneNode *)0)->field)
typedef char SceneNode_position_check[DHD_SCENE_OFFSET(position) == 0x10 ? 1 : -1];
typedef char SceneNode_matrix_check[DHD_SCENE_OFFSET(matrix) == 0x40 ? 1 : -1];
typedef char SceneNode_indices_check[DHD_SCENE_OFFSET(xB4) == 0xB4 ? 1 : -1];
typedef char SceneNode_reference_check[DHD_SCENE_OFFSET(xC0) == 0xC0 ? 1 : -1];
#undef DHD_SCENE_OFFSET

#ifdef __cplusplus
extern "C" {
#endif
void func_002182D0(SceneNode *);
#ifdef __cplusplus
}
#endif
#endif
