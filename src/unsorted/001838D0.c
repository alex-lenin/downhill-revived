/* UNIT 0x001838D0 0x001839A8 */
typedef struct Global {
    char pad0[0x288];
    int count;
} Global;

typedef struct Rider {
    long x0;
    char pad8[0x43D6 - 0x8];
    unsigned short type : 5;
    unsigned short x43D6_5 : 11;
} Rider;

typedef struct Game {
    char pad0[0x50130];
    Rider *riders[1];
} Game;

extern Global D_004B25A0;
extern Game D_002AF2C0;
extern void func_00183678(void *obj, int id, int index);

int func_001838D0(void *obj)
{
    int i;

    for (i = 0; i < D_004B25A0.count; i++) {
        int type = D_002AF2C0.riders[i]->type;
        if (type == 4 || type == 7 || type == 21) {
            func_00183678(obj, 0x33F4, i);
        }
    }
    return 1;
}
