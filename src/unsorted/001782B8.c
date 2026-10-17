/* UNIT 0x001782B8 0x001786A0 */
typedef struct {
    char pad0[0xF6C];
    int xF6C;
    char padF70[0x1164 - 0xF70];
    char *x1164;
    char pad1168[0x128C - 0x1168];
    float x128C;
    char pad1290[0x1368 - 0x1290];
    int x1368;
} Obj;

extern unsigned int D_0026EE20[];
extern float func_001E6480(char *);
extern float func_001E6488(char *);
extern void func_001786A0(Obj *, int, unsigned int, unsigned int);

static inline float Min(float a, float b) {
    __asm__ volatile ("min.s %0,%1,%0" : "+f"(b) : "f"(a));
    return b;
}

static inline float Max(float a, float b) {
    __asm__ volatile ("max.s %0,%1,%0" : "+f"(b) : "f"(a));
    return b;
}

void func_001782B8(Obj *obj) {
    float value, old, level, phase, t, rest, rest2, up, down, lim, step = 5.0f;
    int index, frame, period;
    char *state;
    unsigned int base, out, mixed;
    unsigned char *bytes;
    unsigned int midmask;
    int lowmask;

    state = obj->x1164 + 0x6610;
    out = 0x80000000;
    if (obj->xF6C == 0) return;

    value = func_001E6480(state);
    old = obj->x128C;
    if (value - old > 0.0f) {
        up = Min(value, old + step);
        obj->x128C = up;
        value = up;
    } else {
        down = Max(value, old - step);
        obj->x128C = down;
        value = down;
    }
    level = value / func_001E6488(state) * 10.0f;

    if (level <= 10.0f) {
        /* fill segments up to the level, fade the partial one */
        index = 1;
        lowmask = -256;
        midmask = 0xFFFF00FF;
        base = 0x80000000;
        for (; index < 11; index++) {
            if ((float)index <= level) {
                func_001786A0(obj, index, D_0026EE20[index], 0x6E);
            } else if (level + 1.0f < (float)index) {
                func_001786A0(obj, index, D_0026EE20[0], 0x6E);
            } else {
                phase = (1.0f - (float)index) + level;
                mixed = base & lowmask;
                mixed |= (int)((float)((unsigned char *)&D_0026EE20[index])[0] * phase) & 255;
                mixed &= midmask;
                out = mixed | (((int)((float)((unsigned char *)&D_0026EE20[index])[1] * phase) & 255) << 8);
                func_001786A0(obj, index, out, 0x6E);
            }
        }
    } else {
        /* full: cycle the segments through palette colours 4 -> 7 -> 10 */
        frame = obj->x1368 + 1;
        obj->x1368 = frame;
        if (frame >= 31) obj->x1368 = 0;
        bytes = (unsigned char *)D_0026EE20;
        period = 15;
        lim = 2.0f;
        for (index = 1; index < 11; index++) {
            t = ((float)(30 - obj->x1368) + (float)index * 3.0f) / (float)period;
            out = 0x80000000;
            if (t >= lim) t -= lim;
            if (t < 1.0f) {
                rest = 1.0f - t;
                ((unsigned char *)&out)[0] = (int)((float)bytes[0x1C] * t + (float)bytes[0x10] * rest);
                ((unsigned char *)&out)[1] = (int)((float)bytes[0x1D] * t + (float)bytes[0x11] * rest);
                func_001786A0(obj, index, out, 0x6E);
            } else if (t < lim) {
                t -= 1.0f;
                rest2 = 1.0f - t;
                ((unsigned char *)&out)[0] = (int)((float)bytes[0x28] * t + (float)bytes[0x1C] * rest2);
                ((unsigned char *)&out)[1] = (int)((float)bytes[0x29] * t + (float)bytes[0x1D] * rest2);
                func_001786A0(obj, index, out, 0x6E);
            }
        }
    }
}
