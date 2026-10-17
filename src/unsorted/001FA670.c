/* UNIT 0x001FA670 0x001FA6F0 */
extern char D_002778D0[], D_00277A98[], D_00277770[];
extern float func_001FA618(void *, int, int);
float func_001FA670(int *obj) {
    float sum;
    sum = func_001FA618(D_002778D0, 0x26, obj[0]);
    sum += func_001FA618(D_00277A98, 0x26, obj[0]);
    sum += func_001FA618(D_00277770, 0xb, obj[2]);
    return sum * 0.33333334f;
}
