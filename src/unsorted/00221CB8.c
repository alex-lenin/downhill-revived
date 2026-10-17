/* UNIT 0x00221CB8 0x00221D58 */
/* Normalize the three upper-left 3x3 columns of a 4x4 matrix. */
void func_00221CB8(float m[4][4])
{
    int i;
    float *p;
    float x, y, z;
    float length, inv, nx, ny, nz;

    for (i = 2; i >= 0; i--) {
        p = &m[2][2 - i];
        x = p[-8];
        y = p[-4];
        z = p[0];
        __asm__ volatile ("sqrt.s %0, %1" : "=&f"(length) : "f"(x * x + y * y + z * z) : "memory");
        if (length < 1e-7f) {
            p[-8] = 0.0f;
            p[-4] = 0.0f;
            p[0] = 1.0f;
        } else {
            inv = 1.0f / length;
            nx = x * inv;
            ny = y * inv;
            nz = z * inv;
            p[0] = nz;
            p[-8] = nx;
            p[-4] = ny;
        }
    }
}
