/* UNIT 0x0015F838 0x0015FA10 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Item {
    char pad0[0x1C];
    short type;
    bool Is(int value) { return type == value; }
} Item;

/* menu items are laid out in 16-byte rows */
typedef struct Row {
    char pad0[0x10];
} Row;

struct Slot {
    char pad0[0xC];
    short id;
    char padE[2];
    bool Is(int value) { return id == value; }
};

typedef struct Menu {
    char pad0[0x10];
    Slot slots[72];
    u128 x490;
    char pad4A0[0x504 - 0x4A0];
    int x504;
    int SoundId() { return *(int *)((char *)this + 0x45C); }
} Menu;

typedef struct Sound {
    char pad0[0x64];
    char x64[0x12D - 0x64];
    unsigned char x12D;
} Sound;

typedef struct Game {
    char pad0[0x10C];
    int x10C;
    int x110;
    char pad114[0x354 - 0x114];
    int x354;
    char pad358[0x35C - 0x358];
    unsigned char x35C;
    char pad35D[0x4F0 - 0x35D];
    char x4F0[0x10];
} Game;

extern Sound D_003F52D0;
extern Game D_004B25A0;
extern char D_004B2A90[];
extern char D_0043F9F0[];
extern char D_00388490[];
extern char D_0037F870[];
extern char D_00386B90[];

extern int func_00190D60(void *, int, int);
extern void func_001C57A8(char *, u128);
extern void func_00190660(Sound *, char *);
extern void func_00190890(Sound *, Item *);
extern void func_00187C50(void);
extern void func_0015C920(void);
extern void func_001A8A48(char *, Item *);

static __inline__ char *SoundData(Sound *snd)
{
    return snd->x64;
}

static __inline__ void SoundSetPlayed(Sound *snd)
{
    snd->x12D = 1;
}

static __inline__ bool IsType(Item *item, int type)
{
    return item->type == type;
}

void func_0015F838(Menu *menu, int index)
{
    Item *item;

    if (menu->x504 == 0 && (index == 0 || func_00190D60(SoundData(&D_003F52D0), 8, menu->SoundId()) != 0)) {
        D_004B25A0.x35C = 1;
        /* packed name "S_SELECT" */
        func_001C57A8(D_004B25A0.x4F0, (u128)0x5443454C45535F53L);
    } else {
        if (menu->slots[index].Is(26)) {
            func_00190660(&D_003F52D0, D_00388490);
            return;
        } else if (menu->slots[index].Is(34)) {
            menu->x490 = 0;
            func_001C57A8(D_004B2A90, (u128)0x5443454C45535F53L);
            func_0015C920();
        } else if (menu->slots[index].Is(35)) {
            item = (Item *)&((Row *)menu)[index];
            func_00190890(&D_003F52D0, item);
            func_00187C50();
            return;
        } else if (menu->slots[index].Is(27)) {
            item = (Item *)&((Row *)menu)[index];
            func_001A8A48(D_0043F9F0, item);
            if (D_004B25A0.x10C == 19) {
                SoundSetPlayed(&D_003F52D0);
                D_004B25A0.x354 = 1;
                D_004B25A0.x35C = 0;
            } else {
                if (D_004B25A0.x110 == 1) {
                    func_00190660(&D_003F52D0, D_0037F870);
                } else {
                    func_00190660(&D_003F52D0, D_00386B90);
                }
                return;
            }
        }
    }
    SoundSetPlayed(&D_003F52D0);
}
