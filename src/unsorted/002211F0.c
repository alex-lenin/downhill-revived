/* UNIT 0x002211F0 0x002212A0 */
typedef struct Color {
    float r, g, b, a;
} Color;

extern int func_00220DA0(void *);

void func_002211F0(char *obj, Color *c)
{
    float lum;

    if (func_00220DA0(obj + 0x10) != 0) {
        lum = c->r * 0.3f + c->g * 0.6f + c->b * 0.1f;
        c->r *= 46.08f;
        c->g *= 46.08f;
        c->b *= 46.08f;
        c->a = lum * 1.2f;
    }
}
