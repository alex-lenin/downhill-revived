/* UNIT 0x00199298 0x001992B8 */
typedef struct State {
    int x0;
    float x4;
    int x8;
} State;

typedef struct Obj {
    char pad0[0x2000];
    State state;
} Obj;

static __inline__ void State_Init(State *state)
{
    state->x0 = 0;
    state->x4 = 10.0f;
    state->x8 = 0;
}

void func_00199298(Obj *obj)
{
    State_Init(&obj->state);
}
