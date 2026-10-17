/* UNIT 0x00168F70 0x001692C8 .rodata=0x28AEA0:0x28AF10 */
struct Obj {
    char pad0[0x530];
    int x530;
    int x534;
    int x538;
    int x53C;
    short x540;
    unsigned short x542;
    unsigned short x544;
    short x546;
    short x548;
    short x54A;
    unsigned char x54C;
    unsigned char x54D;
    unsigned char x54E;
    unsigned char x54F;
    unsigned char x550;
    unsigned char x551;
    unsigned char x552;
    unsigned char x553;
    unsigned char x554;
    unsigned char x555;
    unsigned char x556;
};

struct State {
    char pad0[0x6610];
    char x6610[0xC];
    float x661C;
    char pad6620[0x14];
    float x6634;
    char pad6638[0x1360];
    int x7998;
    char pad799C[0xCC];
    unsigned char x7A68;
};

extern char D_002AF2C0[];
extern int D_0026E9F0[];
extern int D_0026EA08[];
extern int D_0026EA20[];
extern int D_0026EA38[];
extern int D_0026EA50[];
extern int D_0026EA68[];
extern int D_0026EA80[];
extern int D_0026EA98[];
extern int D_0026EAB0[];
extern char D_004225A0[];
extern char D_004B1F40[];

extern int *func_001696A8(struct Obj *, int);
extern void func_00169570(struct Obj *);
extern int func_00169728(struct Obj *, int *);
extern void func_00169770(struct Obj *, int);
extern void func_00194648(void *, int);
extern void func_00169808(struct Obj *);
extern void func_001E5328(void *);
extern void func_001B45F8(void *, int, int);

int func_00168F70(struct Obj *p) {
    struct State *state;
    int *list;
    int index;
    char *base = D_002AF2C0;

    base += 0x50000;
    state = *(struct State **)(base + 0x130);
    list = func_001696A8(p, p->x530);
    func_00169570(p);
    if (func_00169728(p, list)) {
        if (!p->x553) {
            p->x553 = 1;
            if (!p->x556) {
                func_00194648(D_004225A0, 0);
                func_00169808(p);
                func_001E5328(state->x6610);
                func_001B45F8(D_004B1F40, 1, 240);
                p->x556 = 1;
            }
        }
    } else {
        index = 0;
        while (list[index] != 15) {
            switch (list[index]) {
            case 0:
                if ((int)state->x661C > D_0026E9F0[p->x530])
                    func_00169770(p, list[index]);
                break;
            case 1:
                if (p->x542 >= D_0026EA08[p->x530])
                    func_00169770(p, list[index]);
                break;
            case 2:
                if (p->x534 > D_0026EA38[p->x530])
                    func_00169770(p, list[index]);
                break;
            case 3:
                if (p->x544)
                    func_00169770(p, list[index]);
                break;
            case 4:
                if (p->x551)
                    func_00169770(p, list[index]);
                break;
            case 5:
                if (p->x552)
                    func_00169770(p, list[index]);
                break;
            case 6:
                if (p->x54E)
                    func_00169770(p, list[index]);
                break;
            case 7:
                if (p->x54D)
                    func_00169770(p, list[index]);
                break;
            case 8:
                if (p->x54F)
                    func_00169770(p, list[index]);
                break;
            case 9:
                if (p->x550)
                    func_00169770(p, list[index]);
                break;
            case 10:
                if ((int)state->x6634 > D_0026EA20[p->x530])
                    func_00169770(p, list[index]);
                break;
            case 11:
                if (state->x7A68 && state->x7998 <= D_0026EA50[p->x530])
                    func_00169770(p, list[index]);
                break;
            case 12:
                if (p->x54C)
                    func_00169770(p, list[index]);
                break;
            case 13:
                if (p->x548 >= D_0026EA68[p->x530])
                    func_00169770(p, list[index]);
                break;
            case 14:
                if (p->x546 >= D_0026EA80[p->x530])
                    func_00169770(p, list[index]);
                break;
            case 16:
                if (p->x538 >= D_0026EA98[p->x530])
                    func_00169770(p, list[index]);
                break;
            case 17:
                if (p->x54A >= D_0026EAB0[p->x530])
                    func_00169770(p, list[index]);
                break;
            default:
                break;
            }
            index++;
        }
    }
    return 1;
}

/* Initializers of func_00169920 (same file), placed after this function's jump table. */
const int D_0028AEE8[3] __attribute__((aligned(8))) = { 27, 14, 1 };
const int D_0028AEF8[3] __attribute__((aligned(8))) = { 32, 6, 6 };
