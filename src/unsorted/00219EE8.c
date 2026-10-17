/* UNIT 0x00219EE8 0x0021A000 */
typedef struct Entry {
    int x0;
    int x4;
    int x8;
    int xC;
} Entry;

typedef struct Dst {
    int x0;
    int x4;
    int x8;
    int xC;
} Dst;

extern Entry D_00621310[];
extern int D_0029E218;
extern int D_0029E224;
extern int func_00218C28(int);
extern int func_00218B80(void *, int, int);

int func_00219EE8(int id, Dst *dst, void *packet)
{
    Entry *entry;
    int ret;
    int tmp;

    if (id >= 1000) {
        return 0;
    }
    if (id <= 0) {
        if (id == 0) {
            long v;
            v = D_0029E224 | 0x24120000;
            dst->x0 = v;
            dst->x4 = 2;
        }
        return 0;
    }
    entry = &D_00621310[id];
    D_0029E218++;
    ret = 0;
    if ((entry->x4 & 0x80000000) == 0) {
        tmp = (entry->x4 & ~0x3FFF) | (func_00218C28(id) & 0x3FFF);
        entry->x4 = tmp;
        ret = func_00218B80(packet, id, tmp & 0x3FFF);
        entry->x4 |= 0x80000000;
    }
    *(unsigned short *)&dst->x0 = (*(unsigned short *)&dst->x0 & 0xC000) | (*(unsigned short *)&entry->x4 & 0x3FFF);
    return ret;
}
