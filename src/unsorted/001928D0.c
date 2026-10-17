/* UNIT 0x001928D0 0x00192920 */
struct Entry { void *x0; char pad4[0x1c]; int x20; char pad24[12]; int x30; char pad34[12]; int x40; char pad44[0x5c]; };
extern char D_00287338[];
extern struct Entry D_003F5470[];
void func_001928D0(int enabled, int mode) {
    struct Entry *entry; int i; void *table;
    if (mode==0xffff && enabled) {
        table=D_00287338; entry=D_003F5470; i=2;
        do {
            entry->x0=table;
            entry->x20=0;
            entry->x30=0;
            entry->x40=0;
            --i; ++entry;
        } while(i != -1);
    }
}
