/* UNIT 0x00196410 0x00196460 */
typedef struct Entry {
    int x0;
    int x4;
    int *x8;
    int xC;
    int x10;
} Entry;

typedef struct Mgr {
    int x0[6];
    Entry x18[150];
    int xBD0[6];
} Mgr;

extern void func_00196558(Entry *entry);

void func_00196410(Mgr *mgr, int index) {
    int offset;

    offset = index * sizeof(Entry);
    offset += 0x18;
    mgr->xBD0[mgr->x18[index].x4]--;
    func_00196558((Entry *)((char *)mgr + offset));
}
