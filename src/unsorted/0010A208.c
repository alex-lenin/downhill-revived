/* UNIT 0x0010A208 0x0010A2B8 .sdata=0x29C100:0x29C104 */
/* Next to crt0: a registration table (count + array). The count is the first word of .sdata. */
typedef struct Entry {
    long key; /* bits 7..17: secondary id, bits 18..31: primary id */
    int x8;
    void *xC;
    char x10[0x40];
    char x50[1];
} Entry;

int D_0029C100 = 0;
extern Entry *D_0029E700[];

extern void func_0010A528(void *, void *, void *);

void func_0010A208(int x) {
    D_0029E700[D_0029C100] = (Entry *)x;
    D_0029C100++;
}

/* Finds the registered entry with ids (id, subId) and runs it. */
void func_0010A230(unsigned short id, unsigned short subId) {
    int i;
    Entry *e;

    for (i = 0; i < D_0029C100; i++) {
        e = D_0029E700[i];
        if (((int)(e->key >> 18) & 0x3FFF) == id && ((int)(e->key >> 7) & 0x7FF) == subId) {
            func_0010A528(e->xC, e->x10, e->x50);
            return;
        }
    }
}
