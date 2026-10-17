/* UNIT 0x00177F80 0x00178090 */
struct Quad {
    int x0;
    int x4;
    int x8;
    int xC;
};

struct Rider {
    char pad0[0x661C];
    float x661C;
};

extern int D_004B26AC[];
extern char D_0029C9D8[]; /* "%6d " */
extern "C" float func_001301B8(Rider *rider);
extern "C" int func_00254D68(char *buf, const char *fmt, ...);

class Obj {
public:
    char pad0[0xC84];
    Quad quads[38];
    char padEE4[0xF04 - 0xEE4];
    Quad *digitDst[6];
    char padF1C[0xF2C - 0xF1C];
    long *digitTag[6];
    char padF44[0xF68 - 0xF44];
    int xF68;
    char padF6C[0x1164 - 0xF6C];
    Rider *x1164;

    void func_00177F80();
};

/* Shows a 6-digit number: digits copy their quad, other characters (blanks) set tag bits to 9. */
void Obj::func_00177F80()
{
    float value;
    int n;
    char buf[16];
    int i;
    char *p;
    unsigned int digit;
    Quad *src;

    if (xF68 == 0) {
        return;
    }
    if (D_004B26AC[0] != 15) {
        value = x1164->x661C;
    } else {
        value = func_001301B8(x1164);
    }
    n = (int)value;
    if (n <= 0) {
        return;
    }
    func_00254D68(buf, D_0029C9D8, n);
    for (i = 5, p = buf; i >= 0; i--, p++) {
        digit = *p - '0';
        if (digit >= 10) {
            *digitTag[i] = (*digitTag[i] & ~0x3F) | 9;
        } else {
            src = &quads[digit];
            *digitTag[i] &= ~0x3F;
            digitDst[i]->x0 = src->x0;
            digitDst[i]->x4 = src->x4;
            digitDst[i]->x8 = src->x8;
            digitDst[i]->xC = src->xC;
        }
    }
}
