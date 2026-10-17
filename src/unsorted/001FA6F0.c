/* UNIT 0x001FA6F0 0x001FA758 */
extern char D_00277968[], D_002777A0[];
extern float func_001FA618(void *, int, int);
float func_001FA6F0(int *arg) {
    float value = func_001FA618(D_00277968, 38, arg[0]);
    return (value + func_001FA618(D_002777A0, 11, arg[2])) * 0.5f;
}
