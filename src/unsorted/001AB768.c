/* UNIT 0x001AB768 0x001AB7C8 */
typedef struct Vec3 { float x, y, z; } Vec3;
typedef struct State { char pad[0x74]; void *x74; void *x78; } State;
typedef struct Obj { char pad[0x20]; State *x20; } Obj;
void func_001AB768(Obj *arg0, float *out, float *in) {
    State *state;
    void *matrix, *pos;
    float x = in[0], y = in[1], z = in[2];
    state = (State *)((char *)arg0->x20 + 0x40);
    out[0] = x;
    out[1] = y;
    out[2] = z;
    matrix = *(void **)((char *)state + 0x38);
    __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tlqc2 $vf13, 0x10(%1)\n\tlqc2 $vf14, 0x20(%1)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, 0(%0)" : : "r"(out), "r"(matrix) : "memory");
    pos = *(void **)((char *)state + 0x34);
    __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%0)" : : "r"(out), "r"(pos) : "memory");
}
