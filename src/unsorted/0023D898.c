/* UNIT 0x0023D898 0x0023D8D8 */
typedef struct { void *x0; float *x4; char pad8[12]; } Entry;
typedef struct { char pad0[0x14]; float x14; char pad18[0x28]; } State;
extern Entry D_0075A9D0[];
extern State D_0075AA70[];
void func_0023D898(int index, float *a, float *b) {
    State *state = &D_0075AA70[index];
    float value = *D_0075A9D0[index].x4;
    *a = value;
    *b = state->x14 / value;
}
