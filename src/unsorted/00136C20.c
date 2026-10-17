/* UNIT 0x00136C20 0x00136D30 */
typedef struct Entry { char pad0[0xC]; float xC; char pad10[0x10]; } Entry;
struct State { char pad0[0x4098]; unsigned long x4098; char pad40A0[0x792C-0x40A0]; void *x792C; char pad7930[0x797C-0x7930]; int x797C; int x7980; int x7984; };
struct Obj {
    char pad0[0x150];
    float x150;
    int x154;
    struct State *x158;
    char pad15C[0x396 - 0x15C];
    unsigned short type : 5;
    unsigned short x396_5 : 11;
    char pad398[0x4B0 - 0x398];
    long x4B0;
};

extern Entry D_00768098[];
extern float func_001385A8(struct Obj *);
extern float func_00238F38(void);
extern void func_00231C40(struct Obj *, float *);
extern void func_001E5110(void *, int);

static inline Entry *GetEntry(int type) {
    if (type >= 32) return D_00768098;
    return &D_00768098[type];
}

void func_00136C20(struct Obj *obj)
{
    float vec[4];
    float factor;
    float scale;
    float force;
    int type;
    struct State *state;

    if ((unsigned char)(obj->x158->x4098 >> 2) & 1) return;
    type = obj->type;
    if (type != 0x16 && type != 0xB) return;
    factor = GetEntry(type)->xC;
    scale = func_001385A8(obj) * 1.5f;
    force = obj->x150 * (func_00238F38() * 0.5f) * scale;
    if (factor > 0.01f) {
        force *= factor;
        state = obj->x158;
        if (state->x7984) func_001E5110(state->x792C, state->x797C);
    }
    vec[0] = 0;
    vec[1] = force;
    vec[2] = 0;
    func_00231C40(obj, vec);
}
