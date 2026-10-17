/* UNIT 0x0010BED8 0x0010BF50 */
typedef struct Timer {
    char pad0[0x3B4];
    int x3B4;
} Timer;

typedef struct Player {
    char pad0[0x1A4];
    int x1A4;
    char pad1A8[0x18];
    int x1C0;
    int x1C4;
    char pad1C8[0xA4];
    int x26C;
    char pad270[0x18];
    int x288;
    int x28C;
    char pad290[0x164];
    unsigned char x3F4;
    char pad3F5[0x6];
    unsigned char x3FB;
    unsigned char x3FC;
} Player;

void func_0010BED8(Timer *t, Player *p)
{
    if ((float)t->x3B4 > 30.0f) {
        if ((p->x3FB ? p->x288 : p->x1C0) == 0 && (p->x3FC ? p->x28C : p->x1C4) == 0) {
            int on = 1;

            if (p->x3F4) {
                p->x26C = on;
            } else {
                p->x1A4 = on;
                p->x26C = 0;
            }
        }
    }
}
