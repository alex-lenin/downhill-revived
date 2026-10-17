/* UNIT 0x001A8200 0x001A8658 */
/* joint angle limits in degrees, {max, min} pairs */
extern float D_00270240[];

#define DEG2RAD(x) ((x) * 0.017453292f)
#define CLAMP(v, i) \
    if (DEG2RAD(D_00270240[i]) < (v)) (v) = DEG2RAD(D_00270240[i]); \
    else if ((v) < DEG2RAD(D_00270240[(i) + 1])) (v) = DEG2RAD(D_00270240[(i) + 1])

void func_001A8200(void *obj, float *angle, int joint)
{
    if (joint == 1) {
        CLAMP(angle[2], 8);
    } else if (joint == 4) {
        CLAMP(angle[2], 0);
    } else if (joint == 0) {
        CLAMP(angle[0], 10);
        CLAMP(angle[1], 12);
        CLAMP(angle[2], 14);
    } else if (joint == 3) {
        CLAMP(angle[0], 2);
        CLAMP(angle[1], 4);
        CLAMP(angle[2], 6);
    } else if (joint == 7) {
        CLAMP(angle[2], 24);
    } else if (joint == 10) {
        CLAMP(angle[2], 16);
    } else if (joint == 6) {
        CLAMP(angle[0], 26);
        CLAMP(angle[1], 28);
        CLAMP(angle[2], 30);
    } else if (joint == 9) {
        CLAMP(angle[0], 18);
        CLAMP(angle[1], 20);
        CLAMP(angle[2], 22);
    }
}
