/* UNIT 0x00136B40 0x00136C20 */
struct State { char pad0[0x4098]; unsigned long x4098; char pad40A0[0x792C-0x40A0]; void *x792C; char pad7930[0x797C-0x7930]; int x797C; int x7980; int x7984; };
struct Obj { char pad0[0x150]; float x150; int x154; struct State *x158; char pad15C[0x474-0x15C]; float x474; char pad478[0x4B0-0x478]; long x4B0; };
extern float func_00238F38(void);
extern void func_00231C40(struct Obj *, float *);
extern void func_001350E8(struct Obj *);
extern void func_001E5110(void *, int);
void func_00136B40(struct Obj *obj) {
    float scale, random;
    float vec[4];
    struct State *state;
    if ((obj->x4B0 & 2) && !((unsigned char)(obj->x158->x4098 >> 2) & 1) && obj->x474 > 0.01f) {
        scale = obj->x474 * 50.0f;
        random = func_00238F38();
        vec[0] = 0;
        vec[1] = obj->x150 * (random * 0.5f) * scale;
        vec[2] = 0;
        func_00231C40(obj, vec);
        func_001350E8(obj);
        state = obj->x158;
        if (state->x7984) func_001E5110(state->x792C, state->x797C);
    }
}
