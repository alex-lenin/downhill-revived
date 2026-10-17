/* UNIT 0x00126D30 0x00126D90 */
struct Vec { float x0, x4; };
extern float func_00248D90(float, float);
extern void func_002087C0(float *);
float func_00126D30(void *unused, struct Vec *a, struct Vec *b) {
    float result;
    float dx = b->x0 - a->x0;
    float dy = b->x4 - a->x4;
    if (dy == 0.0f) dy = 0.01f;
    result = func_00248D90(dx, dy);
    func_002087C0(&result);
    return result;
}
