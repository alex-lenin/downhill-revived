/* UNIT 0x001E5328 0x001E5460 */
typedef struct Stats {
    char pad0[0x46A];
    unsigned short a[43];
    unsigned short b[43];
    unsigned short c[8];
    char pad526[0x694 - 0x526];
} Stats;

typedef struct Player { char pad0[0x796C]; int x796C; } Player;

typedef struct Obj {
    char pad0[0x40];
    Player *x40;
    char pad44[0x204];
    int a[43];
    int b[43];
    int c[8];
    char pad3C0[0x78];
    char x438[0x2D0];
    int x708;
    char pad70C[4];
    int x710;
} Obj;

extern Stats D_004225A0[];
extern void func_00194DC0(Stats *, void *, int, int);

static inline Stats *PlayerStats(Obj *obj)
{
    int player = (obj->x40->x796C >> 5) - 1;

    return &D_004225A0[player];
}

void func_001E5328(Obj *obj)
{
    int value;
    int count;
    int i;
    int j;

    for (i = 1; i < 43; i++) {
        value = obj->a[i];
        if (value > 0) {
            PlayerStats(obj)->a[i] += value;
        }
        value = obj->b[i];
        if (value > 0) {
            PlayerStats(obj)->b[i] += value;
        }
    }
    for (j = 1; j < 8; j++) {
        count = obj->c[j];
        if (count > 0) {
            PlayerStats(obj)->c[j] += count;
        }
    }
    func_00194DC0(PlayerStats(obj), obj->x438, obj->x710, obj->x708);
}
