/* UNIT 0x00158378 0x00158520 */
typedef struct Player {
    char pad0[0x7988];
    unsigned int x7988;
} Player;

typedef struct Obj {
    Player *x0;
    int x4;
    int x8;
    unsigned char xC;
    signed char xD;
    signed char xE;
    signed char xF;
    unsigned char x10;
} Obj;

extern char D_003B2270[];
extern void func_0017B338(void *, int, int, int, int, int);

void func_00158378(Obj *obj)
{
    char *base;
    unsigned int id;

    if (obj->xF) {
        obj->xF--;
        obj->x4 = 2;
        obj->x10 -= 4;
        obj->x8 = 0;
        base = D_003B2270;
        id = obj->x0->x7988;
        if (id < 4) {
            func_0017B338(base + (id * 0x1380 + 0x150), 16, 1, 0, 0, 0);
        }
        if (obj->x0->x7988 < 4) {
            *(int *)(base + obj->x0->x7988 * 0x1380 + 0x1478) = 240;
        }
    } else if (obj->xE) {
        obj->xE--;
        obj->x4 = 1;
        obj->x10 -= 2;
        obj->x8 = 0;
        base = D_003B2270;
        id = obj->x0->x7988;
        if (id < 4) {
            func_0017B338(base + (id * 0x1380 + 0x150), 17, 1, 0, 0, 0);
        }
        if (obj->x0->x7988 < 4) {
            *(int *)(base + obj->x0->x7988 * 0x1380 + 0x1478) = 240;
        }
    } else if (obj->xD) {
        obj->xD--;
        obj->x4 = 0;
        obj->x10--;
        obj->x8 = 0;
        base = D_003B2270;
        id = obj->x0->x7988;
        if (id < 4) {
            func_0017B338(base + (id * 0x1380 + 0x150), 18, 1, 0, 0, 0);
        }
        if (obj->x0->x7988 < 4) {
            *(int *)(base + obj->x0->x7988 * 0x1380 + 0x1478) = 240;
        }
    } else {
        char *base2;
        unsigned int id2;
        obj->xC++;
        base2 = D_003B2270;
        id2 = obj->x0->x7988;
        if (id2 < 4) {
            func_0017B338(base2 + (id2 * 0x1380 + 0x150), 19, 1, 0, 0, 0);
        }
    }
}
