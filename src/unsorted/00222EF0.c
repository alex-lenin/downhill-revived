/* UNIT 0x00222EF0 0x00222F60 */
/* Quaternion from axis (xyz) and angle in degrees (w); VU0 microroutine at 0x978
 * returns sin in vf14.x and cos in vf15.x (uses inline asm for the VU0 transfer). */
void func_00222EF0(float *quat, float *axis)
{
    float sc[2];
    float half;

    half = axis[3];
    half *= 0.008726646f;

    __asm__ volatile (
        "mfc1 $2, %2\n\t"
        "qmtc2.ni $2, $vf14\n\t"
        "vcallms 0x978\n\t"
        "nop\n\t"
        "qmfc2.i $2, $vf14\n\t"
        "mtc1 $2, %0\n\t"
        "qmfc2.ni $2, $vf15\n\t"
        "mtc1 $2, %1"
        : "=&f"(sc[0]), "=&f"(sc[1]) : "f"(half) : "$2");
    quat[0] = sc[0] * axis[0];
    quat[1] = sc[0] * axis[1];
    quat[2] = sc[0] * axis[2];
    quat[3] = sc[1];
}
