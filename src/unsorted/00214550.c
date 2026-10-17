/* UNIT 0x00214550 0x002145F0 */
/* Axis-aligned box overlap test: box A = [minA, maxA], box B = [minB, maxB]. */
int func_00214550(float *minA, float *maxA, float *minB, float *maxB)
{
    if (minA[0] > maxB[0] || minB[0] > maxA[0]) {
        return 0;
    }
    if (minA[1] > maxB[1] || minB[1] > maxA[1]) {
        return 0;
    }
    if (minA[2] > maxB[2] || minB[2] > maxA[2]) {
        return 0;
    }
    return 1;
}
