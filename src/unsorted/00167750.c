/* UNIT 0x00167750 0x00167800 */
typedef struct Light {
    float x0;
    float x4;
    float x8;
    float xC;
    float r;
    float g;
    float b;
    float x1C;
    char pad20[0xD0];
    int xF0;
    int xF4[1];
} Light;

extern void func_0020ED30(int, int, int, int, float, float, float, float, float);

void func_00167750(Light *light, int arg1, int count)
{
    int i;

    light->xF0 = arg1;
    for (i = 0; i < count; i++) {
        light->xF4[i] = 0;
        func_0020ED30(i, light->r, light->g, light->b, 1.0f, light->x0, light->x4, light->x1C, light->x8);
    }
}
