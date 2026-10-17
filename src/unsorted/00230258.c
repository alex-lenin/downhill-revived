/* UNIT 0x00230258 0x002302B8 */
typedef struct { float x,y,z,w; } Vec __attribute__((aligned(16)));
extern float D_00736C80[3][4];
Vec func_00230258(int index) {
    Vec v;
    v.x=D_00736C80[0][index]*0.03125f;
    v.y=D_00736C80[1][index]*0.03125f;
    v.z=D_00736C80[2][index]*0.03125f;
    return v;
}
