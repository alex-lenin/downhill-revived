/* UNIT 0x001614D8 0x001615F0 */
typedef struct Player {
    char pad0[0x7A68];
    unsigned char x7A68;
} Player;

typedef struct Global {
    char pad0[0x50130];
    Player *x50130[2];
} Global;

typedef struct Obj {
    unsigned long masks[2];
    int count;
    char pad14[0x1C - 0x14];
    int setCount[2];
    int clearCount[2];
} Obj;

extern Global D_002AF2C0;

static inline unsigned char TestBit(unsigned long mask, int bit)
{
    return (mask >> bit) & 1;
}

void func_001614D8(Obj *obj, int kind)
{
    unsigned long i;
    unsigned long last;
    Player *player;

    if (kind >= 2) {
        return;
    }
    obj->setCount[kind] = 0;
    obj->clearCount[kind] = 0;
    last = 0;
    for (i = 0; (int)i < obj->count; i++) {
        if (TestBit(obj->masks[kind], i)) {
            obj->setCount[kind]++;
            last = i;
        }
    }
    player = D_002AF2C0.x50130[kind];
    if (player != 0 && player->x7A68 != 0) {
        last = obj->count;
    }
    for (i = 0; i < last; i++) {
        if (((unsigned char)(obj->masks[kind] >> (int)i) ^ 1) & 1) {
            obj->clearCount[kind]++;
        }
    }
}
