/* UNIT 0x001A3F90 0x001A40B8 */
typedef struct Entry {
    char pad0[0xC];
    int xC;
    char pad10[0x4];
} Entry;

typedef struct Obj {
    Entry entries[17];
    int x154;
    int x158;
} Obj;

extern int D_003F52F0[], D_004B2828[];
extern char D_0028E750[];
extern void func_0023D650(int, int *, int *);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_00254D68(char *, char *, ...);
extern void func_0020F9F0(int, int, int, int, char *);

void func_001A3F90(Obj *obj, int alpha) {
    char text[32];
    int width, height;
    int font = D_003F52F0[0];
    int sum;
    int i;

    func_0023D650(font, &width, &height);
    width /= 2;
    func_0020FB80(0, 280, 256, -98, -32);
    func_0020F518(0, 221, 219, 10, alpha);
    sum = 0;
    for (i = 0; i < D_004B2828[0]; i++) {
        sum += obj->entries[i].xC / 20;
    }
    if (alpha < 128) {
        func_00254D68(text, D_0028E750, obj->x158);
    } else {
        func_00254D68(text, D_0028E750, obj->x158 + sum);
    }
    func_0020F9F0(0, font, width, 140, text);
}
