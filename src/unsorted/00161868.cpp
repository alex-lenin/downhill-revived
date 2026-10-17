/* UNIT 0x00161868 0x00161960 */
typedef union Color {
    struct {
        unsigned int r : 8;
        unsigned int g : 8;
        unsigned int b : 8;
        unsigned int a : 8;
    } ch;
    unsigned int packed;
} Color;

extern float D_0029C7C4;
extern float D_0029C7C8;
__asm__(".extern D_0029C7C4,4\n.extern D_0029C7C8,4");

/* Scales a color's RGB by (0.375 + 0.75 * light), clamped to 255; alpha is kept. */
unsigned int func_00161868(Color color, float *light)
{
    Color out;
    float max = 255.0f;
    out.ch.r = (int)(max <? ((float)(int)color.ch.r * (D_0029C7C4 + light[0] * D_0029C7C8)));
    out.ch.g = (int)(max <? ((float)(int)color.ch.g * (D_0029C7C4 + light[1] * D_0029C7C8)));
    out.ch.b = (int)(max <? ((float)(int)color.ch.b * (D_0029C7C4 + light[2] * D_0029C7C8)));
    out.ch.a = color.ch.a;
    return out.packed;
}
