/* UNIT 0x0020CC40 0x0020CD78 */
typedef struct Sound {
    char pad0[0x20]; int x20;
    char pad24[0x60-0x24]; int x60,x64,x68,x6C;
    char pad70[0x290-0x70]; float x290,x294; int x298; float x29C;
    char pad2A0[0x2B0-0x2A0]; int x2B0,x2B4,x2B8,x2BC;
} Sound;
extern Sound D_004D9720[];
void func_0020CC40(int index, float duration, float fade, unsigned int color, int arg)
{
    int active;
    Sound *sound = &D_004D9720[index];
    duration *= 60.0f;
    fade *= 60.0f;
    sound->x2B0 = (int)duration;
    active = sound->x2B8 >= 2;
    sound->x29C = (float)(color >> 24);
    { Sound *next = &D_004D9720[index];
    next->x2B8 = next->x2B0 >= 1 ? 1 : 2;
    if (fade > 0.5f) {
        if (active) {
            next->x294 = (next->x29C - next->x290) / fade;
        } else {
            next->x290 = 0.0f;
            next->x294 = next->x29C / fade;
        }
    } else next->x290 = next->x29C;
    }
    { Sound *last = &D_004D9720[index];
    last->x2BC = arg;
    last->x20 = 0x44;
    last->x60 = color & 255;
    last->x64 = (color >> 8) & 255;
    last->x68 = (color >> 16) & 255;
    last->x6C = 0;
    }
}
