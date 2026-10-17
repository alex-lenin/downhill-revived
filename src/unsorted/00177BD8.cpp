/* UNIT 0x00177BD8 0x00177D28 */
struct Race {
    char pad0[0x797C];
    int x797C;
    char pad7980[0x7A68 - 0x7980];
    unsigned char x7A68;
};

struct GlobalState {
    char pad0[0x1C];
    int x1C[1];
};

class Obj {
public:
    char pad0[0x1164];
    Race *x1164;
    char pad1168[0x134A - 0x1168];
    char x134A[5];
    char x134F[5];

    void func_00177BD8();
};

extern "C" GlobalState D_0043EEC0;
extern "C" int D_0043EEC8[];
extern "C" char D_0029C9C8[];
extern "C" int func_00254D68(char *buf, const char *fmt, ...);

void Obj::func_00177BD8()
{
    char buf[16];
    int time;
    int seconds;
    int minutes;
    int digit;

    if (x1164->x7A68) {
        time = D_0043EEC0.x1C[x1164->x797C] / 60;
    } else {
        time = D_0043EEC8[0] / 60;
    }
    seconds = time % 60;
    minutes = time / 60 % 60;

    func_00254D68(buf, D_0029C9C8, seconds);
    x134A[0] = x134F[0];
    x134A[1] = x134F[1];
    digit = buf[1] - '0';
    if ((unsigned int)digit >= 10) {
        digit = 0;
    }
    x134F[0] = digit;
    digit = buf[0] - '0';
    if ((unsigned int)digit >= 10) {
        digit = 0;
    }
    x134F[1] = digit;

    func_00254D68(buf, D_0029C9C8, minutes);
    x134A[2] = x134F[2];
    x134A[3] = x134F[3];
    digit = buf[1] - '0';
    if ((unsigned int)digit >= 10) {
        digit = 0;
    }
    x134F[2] = digit;
    digit = buf[0] - '0';
    if ((unsigned int)digit >= 10) {
        digit = 0;
    }
    x134F[3] = digit;
}
