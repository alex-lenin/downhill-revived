/* UNIT 0x001634E0 0x001635E8 */
typedef struct Rider {
    char pad0[0x7A5C];
    int x7A5C;
    char pad7A60[0x7BAA - 0x7A60];
    unsigned char x7BAA;
} Rider;

typedef struct Game {
    char pad0[0x16D0];
    Rider *rider;
    char pad16D4[0x2110 - 0x16D4];
    float dt;
} Game;

typedef struct Gauge {
    Game *game;
    float value;
    float max;
    float drain;
    float fill;
    float x14;
    float x18;
    char pad1C[0x24 - 0x1C];
    float x24;
    float x28;
    unsigned char x2C;
    unsigned char x2D;
} Gauge;

extern int D_0039E834[];

void func_001634E0(Gauge *gauge)
{
    float drain;

    if (0.0f < gauge->fill) {
        gauge->value = gauge->value + gauge->fill * gauge->x24 * gauge->x28 * gauge->game->dt;
        if (gauge->max < gauge->value) {
            gauge->value = gauge->max;
        }
    }
    drain = gauge->drain * gauge->game->dt;
    if (gauge->x2D) {
        drain += gauge->x14;
    }
    if (gauge->x2C) {
        drain += gauge->x18;
    }
    if (D_0039E834[0] && gauge->game->rider->x7A5C == 1) {
        drain *= 0.75f;
    }
    if (0.0f < drain && !gauge->game->rider->x7BAA) {
        gauge->value = gauge->value - drain;
        if (gauge->value < 0.0f) {
            gauge->value = 0.0f;
        }
    }
}
