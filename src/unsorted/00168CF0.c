/* UNIT 0x00168CF0 0x00168DB0 */
typedef struct Rider {
    char pad0[0x798C];
    int x798C;
} Rider;

typedef struct World {
    char pad0[0x50130];
    Rider *riders[1];
} World;

typedef struct Race {
    char pad0[0x288];
    int count;
} Race;

typedef struct Msg {
    char pad0[0x500];
    int x500;
} Msg;

extern World D_002AF2C0;
extern Race D_004B25A0;
extern Msg D_0038A2C0;
extern char D_003F52D0[];

extern void func_00190660(char *, Msg *);

void func_00168CF0(void *arg0, int id)
{
    int i;

    for (i = 0; i < D_004B25A0.count; i++) {
        if (id == D_002AF2C0.riders[i]->x798C - 1) {
            D_0038A2C0.x500 = i;
            func_00190660(D_003F52D0, &D_0038A2C0);
        }
    }
}
