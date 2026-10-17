/* UNIT 0x00221DB0 0x00221EB0 */
/* Rotation matrix of angle (s = sin, c = cos) about the unit axis. */
void func_00221DB0(float m[4][4], float *axis, float s, float c)
{
    float t = 1.0f - c;
    float tx = t * axis[0];
    float ty = t * axis[1];
    float tz = t * axis[2];

    m[0][0] = tx * axis[0] + c;
    m[0][1] = tx * axis[1] + axis[2] * s;
    m[0][2] = tx * axis[2] - axis[1] * s;
    m[1][0] = ty * axis[0] - axis[2] * s;
    m[1][1] = ty * axis[1] + c;
    m[1][2] = ty * axis[2] + axis[0] * s;
    m[2][0] = tz * axis[0] + axis[1] * s;
    m[2][1] = tz * axis[1] - axis[0] * s;
    m[2][2] = tz * axis[2] + c;
    m[3][0] = 0.0f;
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][2] = 0.0f;
    m[3][1] = 0.0f;
    m[3][3] = 1.0f;
}
