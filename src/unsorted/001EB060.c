/* UNIT 0x001EB060 0x001EB0B0 */
typedef struct { void *x0; char pad[0x4c]; } Entry;
extern Entry D_004C5C50[];
extern char D_00288428[];
void func_001EB060(int enabled, int id) {
    Entry *p;
    int i;
    if (id == 0xffff && enabled) {
        p = D_004C5C50;
        i = 2;
        do {
            p->x0 = D_00288428;
            i--;
            p++;
        } while (i != -1);
    }
}
