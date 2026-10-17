/* UNIT 0x00222040 0x00222140 */
/* rotation matrix from the euler angles in rot (x, y, z) */
void func_00222040(float m[4][4], float *rot)
{
    float s[4] __attribute__((aligned(16)));
    float c[4] __attribute__((aligned(16)));
    float cx, sx, sy, sz, cy, cz;
    int i;
    int *p, *q;

    /* VU0 micro routine at 0x978: vf14 = sin, vf15 = cos */
    __asm__ volatile (
        "lqc2 $vf14, 0x0(%0)\n\t"
        "vcallms 0x978\n\t"
        "nop\n\t"
        "qmfc2.i $v0, $vf14\n\t"
        "sqc2 $vf15, 0x0(%1)\n\t"
        "sq $v0, 0x0($sp)"
        : : "r"(rot), "r"(c) : "$2", "memory");

    sx = s[0];
    sy = s[1];
    sz = s[2];
    cx = c[0];
    cy = c[1];
    cz = c[2];
    m[0][0] = cy * cz;
    m[0][1] = cy * -sz;
    m[0][2] = sy;
    m[1][0] = sx * sy * cz + cx * sz;
    m[1][1] = sx * -sy * sz + cx * cz;
    m[1][2] = sx * -cy;
    m[2][0] = cx * -sy * cz + sx * sz;
    m[2][1] = cx * sy * sz + sx * cz;
    m[2][2] = cx * cy;
    q = (int *)m[3];
    p = (int *)(m[0] + 3);
    i = 3;
    do {
        *p = 0;
        i--;
        *q = 0;
        p += 4;
        q++;
    } while (i >= 0);
    m[3][3] = 1.0f;
}
