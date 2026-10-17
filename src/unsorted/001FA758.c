/* UNIT 0x001FA758 0x001FA7C0 */
extern char D_00277800[], D_00277A00[];
extern float func_001FA618(void *, int, int);
float func_001FA758(int *arg) {
    float value = func_001FA618(D_00277800, 25, arg[1]);
    return (value + func_001FA618(D_00277A00, 38, arg[0])) * 0.5f;
}
