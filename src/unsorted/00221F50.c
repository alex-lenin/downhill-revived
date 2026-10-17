/* UNIT 0x00221F50 0x00222040 */
void func_00221F50(float *out, const void *src) {
    float first[4] __attribute__((aligned(16)));
    float second[4] __attribute__((aligned(16)));
    float x, y, z, a, b, c;
    float xa, xb, nx;
    int i;
    int *p, *q;

    __asm__ volatile (
        "lqc2 $vf14, 0x0(%0)\n\t"
        "vcallms 0x978\n\t"
        "nop\n\t"
        "qmfc2.i $v0, $vf14\n\t"
        "sqc2 $vf15, 0x0(%1)\n\t"
        "sq $v0, 0x0($sp)"
        : : "r"(src), "r"(second) : "$2", "memory");
    z = first[2];
    x = first[0];
    y = first[1];
    a = second[0];
    b = second[1];
    c = second[2];

    xb = b * x;
    xa = y * x;
    nx = -z;
    out[0] = b * c + xa * z;
    out[1] = b * nx + xa * c;
    out[2] = y * -a;
    out[4] = a * z;
    out[5] = a * c;
    out[6] = x;
    out[8] = y * c - xb * z;
    out[9] = y * nx - xb * c;
    out[10] = b * a;

    q = (int *)(out + 12);
    p = (int *)(out + 3);
    i = 3;
    do {
        *p = 0;
        i--;
        *q = 0;
        p += 4;
        q++;
    } while (i >= 0);
    out[15] = 1.0f;
}
