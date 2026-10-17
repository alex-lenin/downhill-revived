/* UNIT 0x00112CD0 0x00112D88 */
typedef struct Body {
    char pad0[0x18];
    float x18;
} Body;

typedef struct Player {
    char pad0[0x792C];
    Body *x792C;
} Player;

typedef struct Rider {
    Player *x0;
    int x4;
    Player *x8;
    char padC[0x288];
    float x294;
    char pad298[0x1A8];
    unsigned long x440;
} Rider;

/* Bit 47 of x440: x8's x18 is within (-5, 65) of x0's (only while x294 < 100). */
void func_00112CD0(Rider *r)
{
    float d;

    r->x440 &= ~(1L << 47);
    if (r->x8 != 0 && r->x294 < 100.0f) {
        d = r->x8->x792C->x18 - r->x0->x792C->x18;
        if (-5.0f < d && d < 65.0f) {
            r->x440 |= 1L << 47;
        }
    }
}
