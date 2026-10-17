/* UNIT 0x001781E0 0x001782B8 */
typedef struct Sprite {
    unsigned long state : 6;
    unsigned long flags : 58;
    char pad8[0x4C - 0x8];
    float x4C;
} Sprite;

typedef struct Gauge {
    char pad0[0x8];
    unsigned short count;
    char padA[0x20 - 0xA];
    Sprite *items[1];
} Gauge;

typedef struct Meter {
    char pad0[0x4];
    float value;
    float max;
} Meter;

typedef struct Rider {
    char pad0[0x2E94];
    Meter x2E94;
} Rider;

typedef struct Hud {
    char pad0[0xF5C];
    Gauge *xF5C;
    char padF60[0x1164 - 0xF60];
    Rider *x1164;
} Hud;

void func_001781E0(Hud *hud) {
    Gauge *gauge = hud->xF5C;
    float prev = 0.0f;
    int i = 1;

    if (gauge != 0) {
        Meter *meter = &hud->x1164->x2E94;
        float step = meter->max / gauge->count;
        float value = meter->value;
        for (; i <= hud->xF5C->count; i++) {
            float limit = i * step;
            Sprite *sprite = hud->xF5C->items[i - 1];
            sprite->state = 0;
            if (limit <= value) {
                sprite->x4C = 1.0f;
            } else if (value <= prev) {
                sprite->state = 9;
            } else {
                sprite->x4C = (value - prev) / step;
            }
            prev = limit;
        }
    }
}
