/* UNIT 0x00223348 0x00223578 */
/* Rotation matrix (m, 4x4 row-major) to quaternion (out = x, y, z, w).
 * gcc 2.95 cannot emit the EE sqrt.s instruction, so the original source used
 * inline asm for it (allowed PS2 exception). Each switch case computes its
 * root into a fresh variable; reusing zz instead changes register allocation.
 */
#define EE_SQRT(dst, src) __asm__ volatile ("sqrt.s %0,%1" : "=&f"(dst) : "f"(src) : "memory")

void func_00223348(float *out, float *m)
{
    float xx, yy, zz, sum, trace, scale;
    int axis;

    xx = m[0];
    yy = m[5];
    zz = m[10];
    sum = xx + yy;
    trace = sum + zz;
    if (trace > 0.0f) {
        float root, s;
        EE_SQRT(root, trace + 1.0f);
        s = 0.5f;
        out[3] = root * s;
        s = s / root;
        out[0] = (m[6] - m[9]) * s;
        out[1] = (m[8] - m[2]) * s;
        out[2] = (m[1] - m[4]) * s;
        return;
    }
    axis = 0;
    if (xx < yy) axis = 1;
    if (m[axis * 5] < zz) axis = 2;
    switch (axis) {
    case 0: {
        float root;
        EE_SQRT(root, xx - (yy + zz) + 1.0f);
        scale = 0.5f;
        out[0] = root * scale;
        scale = scale / root;
        out[1] = (m[1] + m[4]) * scale;
        out[2] = (m[2] + m[8]) * scale;
        out[3] = (m[6] - m[9]) * scale;
        break;
    }
    case 1: {
        float root;
        EE_SQRT(root, yy - (zz + xx) + 1.0f);
        scale = 0.5f;
        out[1] = root * scale;
        scale = scale / root;
        out[2] = (m[6] + m[9]) * scale;
        out[0] = (m[4] + m[1]) * scale;
        out[3] = (m[8] - m[2]) * scale;
        break;
    }
    case 2: {
        float root;
        EE_SQRT(root, zz - sum + 1.0f);
        scale = 0.5f;
        out[2] = root * scale;
        scale = scale / root;
        out[0] = (m[8] + m[2]) * scale;
        out[1] = (m[9] + m[6]) * scale;
        out[3] = (m[1] - m[4]) * scale;
        break;
    }
    }
}
