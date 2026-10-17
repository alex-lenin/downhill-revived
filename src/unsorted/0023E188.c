/* UNIT 0x0023E188 0x0023E288 */
typedef struct Color {
    unsigned int r : 8;
    unsigned int g : 8;
    unsigned int b : 8;
    unsigned int a : 8;
} Color;

typedef struct IVec {
    int x, y, z;
    float w;
} __attribute__((aligned(16))) IVec;

/* colour of a gradient table at index * scale + base (linear between entries) */
Color func_0023E188(int index, Color *table, float base, float scale)
{
    IVec from;
    IVec to;
    Color c;
    Color out;
    float pos;
    int n;

    pos = index * scale + base;
    if (pos <= 0.0f)
        return table[0];
    n = (int)pos;
    pos -= n;
    from.x = table[n].r;
    from.y = table[n].g;
    from.z = table[n].b;
    from.w = pos;
    n++;
    to.x = table[n].r;
    to.y = table[n].g;
    to.z = table[n].b;
    __asm__ volatile ("lqc2 $vf11, 0(%0)" : : "r"(&from));
    __asm__ volatile ("lqc2 $vf12, 0(%0)" : : "r"(&to));
    __asm__ volatile ("vitof0.xyz $vf11, $vf11\n\t"
                      "vitof0.xyz $vf12, $vf12\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddaw.xyz ACC, $vf12, $vf11w\n\t"
                      "vmsubw.xyz $vf11, $vf11, $vf11w\n\t"
                      "vftoi0.xyz $vf11, $vf11");
    __asm__ volatile ("sqc2 $vf11, 0(%0)" : : "r"(&from) : "memory");
    out = c;
    out.r = from.x;
    out.g = from.y;
    out.b = from.z;
    return out;
}
