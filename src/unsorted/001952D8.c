/* UNIT 0x001952D8 0x00195368 */
struct Stats { char pad[0x34]; unsigned char x34,x35,x36,x37; unsigned short x38,x3A; int x3C,x40; };
void func_001952D8(struct Stats *stats, int amount, int count, int category, int extra) {
    ++stats->x38;
    stats->x3A+=count;
    stats->x3C+=amount;
    stats->x40+=extra;
    if(category==1) { ++stats->x34; return; }
    if(category==2) { ++stats->x35; return; }
    if(category==3) { ++stats->x36; return; }
    ++stats->x37;
}
