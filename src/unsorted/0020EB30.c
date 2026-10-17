/* UNIT 0x0020EB30 0x0020EC30 */
typedef struct Light {
    float x0;
    float x4;
    float x8;
    float xC;
    float r;
    float g;
    float b;
    float x1C;
} Light;

extern Light D_004F3220[];
extern char D_004F32E0[][0x20];
extern long D_004F33E8[];
extern char D_004F3420[][0x10];

extern void func_0020E990(int, void *, Light *);
extern void func_0020E920(float *, float *, float, float, float);
extern char *func_0023DF70(int);
extern void func_0020E950(void *, float, float, float);
extern void func_0020E970(void *, float, float, float);
extern void func_0023D598(int, long);

float func_0020EB30(int index)
{
    Light *light = &D_004F3220[index];
    float x;
    float y;
    long color;

    func_0020E990(index, D_004F32E0[index], light);
    func_0020E920(&x, &y, light->x4, light->x0, light->xC);
    func_0020E950(func_0023DF70(index) + 0xD0, light->xC, x, y);
    func_0020E970(D_004F3420[index], light->xC, x, y);
    color = (long)(int)light->r | ((long)(int)light->g << 8) | ((long)(int)light->b << 16);
    D_004F33E8[index] = color;
    func_0023D598(index, color);
    return light->x0;
}
