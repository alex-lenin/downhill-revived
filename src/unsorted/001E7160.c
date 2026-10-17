/* UNIT 0x001E7160 0x001E7260 */
__asm__(".extern D_0029D744, 4");

typedef struct Obj {
    char pad0[0x8];
    int count;
    int xC[300];
    int x4BC[300];
    int x96C[20];
    char pad9BC[0x50];
    int xA0C;
    int xA10;
    char padA14[4];
    unsigned char xA18;
    unsigned char xA19;
} Obj;

extern int D_0029D744;

extern int func_001E7260(Obj *, int);

void func_001E7160(Obj *obj, int all)
{
    int mask;
    int index;
    int n;
    int slot;
    int bits;

    if (obj->xA19 == 0 || obj->xA18 == 0) {
        return;
    }
    mask = 0;
    index = obj->xA0C - 1;
    n = D_0029D744;
    if (all == 0) {
        n = obj->count;
    }
    while (n > 0) {
        if (index < 0) {
            index += 300;
        }
        bits = obj->xC[index];
        mask |= bits;
        mask |= func_001E7260(obj, obj->x4BC[index]);
        index--;
        n--;
    }
    if (mask != 0) {
        slot = obj->xA10;
        obj->x96C[slot] = mask;
        slot++;
        obj->xA10 = slot;
        if (slot >= 20) {
            obj->xA10 = 0;
        }
        obj->xA19 = 0;
        obj->count = 0;
    }
}
