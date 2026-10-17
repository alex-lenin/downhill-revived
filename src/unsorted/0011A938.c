/* UNIT 0x0011A938 0x0011A9C8 */
struct Flags {
    char pad[0x440];
    unsigned long x440;
};

struct Rider {
    char pad[0x78D4];
    struct Flags *x78d4;
    char pad78d8[0x190];
    unsigned char x7a68;
};

struct Obj {
    char pad[0x50140];
    struct Rider *riders[26];
    int x501a8;
    char pad501ac[0x18];
    int x501c4;
};

extern int D_004B2828[];

void func_0011A938(struct Obj *obj)
{
    int i;
    int count;
    struct Rider *rider;

    count = obj->x501a8 - D_004B2828[0];
    obj->x501c4 = 0;
    for (i = 0; i < count; i++) {
        rider = obj->riders[i];
        if (rider != 0 && rider->x7a68 == 0 && rider->x78d4 != 0
            && ((unsigned char)(rider->x78d4->x440 >> 47) & 1)) {
            obj->x501c4++;
        }
    }
}
