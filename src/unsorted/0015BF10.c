/* UNIT 0x0015BF10 0x0015BF80 */
typedef struct { void *x0; char pad[0x2C]; int x30[4], x40[4], x50[4], x60[4], x70[4], x80[4], x90[4], xA0[4], xB0[4], xC0[4], xD0[4]; char tail[0x120]; } Obj;
extern char D_002849F8[];
extern Obj D_00383D50[];
void func_0015BF10(int enabled, int phase) {
    Obj *p;
    void *table;
    int count;
    if (phase == 65535 && enabled) {
        table = D_002849F8;
        p = D_00383D50;
        for (count = 19; count != -1; count--, p++) {
            p->x0 = table;
            p->x30[0] = 0;
            p->x40[0] = 0;
            p->x50[0] = 0;
            p->x60[0] = 0;
            p->x70[0] = 0;
            p->x80[0] = 0;
            p->x90[0] = 0;
            p->xA0[0] = 0;
            p->xB0[0] = 0;
            p->xC0[0] = 0;
            p->xD0[0] = 0;
        }
    }
}
