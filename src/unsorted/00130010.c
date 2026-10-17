/* UNIT 0x00130010 0x001301B8 */
#include "matrix_packets.h"
typedef struct Obj { char pad0[0x10]; float pos[4]; char pad20[0x20]; float matrix[16]; } Obj;
typedef struct State { Obj *obj; unsigned char x4, x5; char pad6[10]; float pos[4]; } State;
typedef struct Owner { char pad0[0x792c]; State *state; } Owner;
extern int func_0023D608(void);
extern Obj *func_0023D040(int);
extern Obj *func_0023DF20(int);

extern int func_00217DE0(void);
int func_00130010(Owner *owner) {
    float pos[4], delta[4], limit[4], mat1[16] __attribute__((aligned(16))), mat2[16] __attribute__((aligned(16)));
    int i = 0, count;
    Obj *obj, *other;
    float *matrix, *limit_ptr, *res;
    State *state;
    { Obj *initial = owner->state->obj;
    pos[0] = initial->pos[0]; pos[1] = initial->pos[1];
    pos[2] = initial->pos[2]; pos[3] = initial->pos[3]; }
    count = func_0023D608();
    if (count > 0) do {
        if ((owner->state->x5 >> i) & 1) return 0;
        obj = func_0023D040(i);
        other = func_0023DF20(i);
        matrix = obj->matrix;
        state = owner->state;
        delta[0] = state->pos[0] - obj->pos[0];
        delta[1] = state->pos[1] - obj->pos[1];
        delta[2] = state->pos[2] - obj->pos[2];
        ComposeMatrixResultLegacy(mat1, (float *)((char *)other + 0x30), matrix);
        res = ComposeMatrixResultLegacy(mat2, (float *)((char *)other + 0x70), matrix);
        limit[0] = 200.0f; limit[1] = 0; limit[2] = 0; limit[3] = 0;
        limit_ptr = limit;
        __asm__ volatile ("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 16(%1)\n\tlqc2 $vf3, 32(%1)\n\tlqc2 $vf7, 0(%2)\n\tlqc2 $vf8, 16(%2)\n\tlqc2 $vf9, 32(%2)\n\tlqc2 $vf5, %3\n\tlqc2 $vf4, 0(%4)\n\tlqc2 $vf10, 0(%0)\n\tvcallms 0xc0\n\tvnop" : "=r"(limit_ptr) : "r"(mat1), "r"(mat2), "m"(pos[0]), "r"(delta), "0"(limit_ptr), "r"(res) : "memory");
        if (func_00217DE0()) return 1;
    } while (++i < count);
    return 0;
}
