/* UNIT 0x00112A68 0x00112B20 */
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

/* Bit 50 of x440: x0's x18 is within (-50, 10) of x8's (only while x294 < 65). */
void func_00112A68(Rider *r)
{
    float d;

    r->x440 &= ~(1L << 50);
    if (r->x8 != 0 && r->x294 < 65.0f) {
        d = r->x0->x792C->x18 - r->x8->x792C->x18;
        if (d < 10.0f && -50.0f < d) {
            r->x440 |= 1L << 50;
        }
    }
}
