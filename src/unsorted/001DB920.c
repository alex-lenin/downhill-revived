/* UNIT 0x001DB920 0x001DB9B0 */
typedef struct { char pad[0x10]; float pos[3]; } Position;
typedef struct { char pad[0x3514]; Position *x3514; char pad3518[0xB80]; unsigned long x4098; char pad40A0[0x388C]; Position *x792C; } World;
extern World *D_0037ED14[];
extern float func_00222330(float *, void *);
void func_001DB920(void *arg, int *output, float *previous) {
    World *world = D_0037ED14[0];
    float value = 0.0f;
    if (world != 0) {
        if (!((unsigned char)(world->x4098 >> 2) & 1)) {
            value = func_00222330(world->x792C->pos, arg);
        } else {
            value = func_00222330(world->x3514->pos, arg);
        }
    }
    *output = (int)((*previous - value) * 40.909092f);
    *previous = value;
}
