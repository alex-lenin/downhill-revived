/* UNIT 0x00220CC0 0x00220D68 */
typedef struct Light {
    char pad0[0x40];
    float color[4];
    float matrix[4][4];
    char pad90[0x30];
} Light;

extern Light D_00660E80[];

void func_00220CC0(void) {
    Light *light;
    int i;

    for (i = 0; i < 3; i++) {
        light = &D_00660E80[i];
        light->matrix[0][0] = 0.0f;
        light->matrix[0][1] = 1.0f;
        light->matrix[0][2] = 0.0f;
        light->matrix[0][3] = 0.0f;
        light->matrix[1][0] = -1.0f;
        light->matrix[1][1] = 0.0f;
        light->matrix[1][2] = 0.0f;
        light->matrix[1][3] = 0.0f;
        light->matrix[2][0] = 0.0f;
        light->matrix[2][1] = 0.0f;
        light->matrix[2][2] = 0.0f;
        light->matrix[2][3] = 0.0f;
        light->matrix[3][0] = 0.0f;
        light->matrix[3][1] = 0.0f;
        light->matrix[3][2] = 0.0f;
        light->matrix[3][3] = 1.0f;
        light->color[0] = 0.2f;
        light->color[1] = 0.2f;
        light->color[2] = 0.2f;
        light->color[3] = 0.2f;
    }
    D_00660E80[2].color[0] = 0.0f;
    D_00660E80[2].color[1] = 0.0f;
    D_00660E80[2].color[2] = 255.0f;
}
