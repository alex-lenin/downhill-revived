/* UNIT 0x00196388 0x00196410 */
typedef struct Entry {
    int x0;
    int x4; /* list the entry is on */
    int *x8;
    int xC;
    int x10;
} Entry;

typedef struct Mgr {
    int x0[6];
    Entry x18[150];
    int xBD0[6]; /* entry count per list */
} Mgr;

extern void func_00196960(Entry *entry, int list, int head);

/* Moves entry `index` to `list`. The entry offset is built up in a variable, which keeps
   gcc from reusing the first index * 20 (retail multiplies twice). */
void func_00196388(Mgr *mgr, int index, int list) {
    int offset;

    offset = index * sizeof(Entry);
    offset += 0x18;
    mgr->xBD0[mgr->x18[index].x4]--;
    func_00196960((Entry *)((char *)mgr + offset), list, mgr->x0[list]);
    mgr->xBD0[list]++;
}
