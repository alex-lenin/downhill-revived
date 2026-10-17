/* UNIT 0x0011B658 0x0011B6E0 */
void func_0011B658(float *src, float *pos, float *out, float matrix[4][4])
{
    int i, j, k;

    pos[0] = src[2];
    pos[1] = src[3];
    pos[2] = src[4];
    k = 18;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            matrix[i][j] = src[k++];
        }
    }
    out[0] = src[82];
    out[1] = src[83];
    out[2] = src[84];
}
