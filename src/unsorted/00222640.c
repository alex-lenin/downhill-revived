/* UNIT 0x00222640 0x00222660 */
typedef struct { float x0, x4; } Vec;
extern float func_00248D90(float, float);
float func_00222640(Vec *obj) {
    return func_00248D90(obj->x0, obj->x4);
}
