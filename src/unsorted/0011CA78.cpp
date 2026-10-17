/* UNIT 0x0011CA78 0x0011CD58 */
struct A {
    char pad0[0x268];
    unsigned long x268_0 : 49;
    unsigned long x268_49 : 1;
    unsigned long x268_50 : 14;
    void SetFlag49(int on) { x268_49 = on; }
};

struct B {
    A items[30];
    unsigned int **x4920[30];
    int x4998[30];
    int x4A10;
    int x4A14;
    int x4A18[30];
    int x4A90;
    int x4A94;
    int x4A98;
    int x4A9C[13];
    float x4AD0[13];
    void func_0011CA78();
};

struct C { char pad0[8]; float x8; };
struct Config { char pad0[0x288]; int x288; };

extern C *func_0011CD58(B *, int);
extern int func_00122258(void *, int);
extern void func_0011D488(A *, B *, unsigned int **, int, int, C *);
extern float D_0029C160[];
extern char D_003041B0[];
extern Config D_004B25A0;

void B::func_0011CA78()
{
    int i, stride, idx;
    float f;
    C *s2;
    A *a;

    f = D_0029C160[0];
    for (i = 0; i < 13; i++) {
        x4AD0[i] = f;
        x4A9C[i] = -1;
    }

    stride = D_004B25A0.x288 >= 2 ? 2 : 1;
    for (i = 0; i < x4A90; i += stride) {
        if (x4920[i]) {
            s2 = func_0011CD58(this, func_00122258(D_003041B0, **x4920[i] >> 18));
        } else {
            s2 = func_0011CD58(this, 0);
        }
        if (s2) {
            switch (func_00122258(D_003041B0, **x4920[i] >> 18)) {
                case 4: idx = 1; break;
                case 8: idx = 2; break;
                case 0x10: idx = 3; break;
                case 0x20: idx = 4; break;
                case 0x40: idx = 5; break;
                case 0x80: idx = 6; break;
                case 0x100: idx = 7; break;
                case 0x200: idx = 8; break;
                case 0x400: idx = 9; break;
                case 0x800: idx = 10; break;
                case 0x1000: idx = 11; break;
                case 0x2000: idx = 12; break;
                default: idx = 0; break;
            }
            x4AD0[idx] = s2->x8;
            a = &items[i];
            func_0011D488(a, this, x4920[i], x4998[i], x4A18[i], s2);
            a->SetFlag49(1);
        }
    }
}
