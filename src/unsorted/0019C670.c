/* UNIT 0x0019C670 0x0019C868 .sdata=0x29D1AC:0x29D1B4 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Packet { unsigned long x0, x8; };
int D_0029D1AC = 1;
int D_0029D1B0 = 1;
void func_0019C670(struct Packet *p, int *count, int flags) {
    *(u128 *)&p[*count] = 0;
    *(unsigned int *)&p[(*count)++] = 0x1000000D;
    p[*count].x0 = 0x1000000000000006UL;
    p[(*count)++].x8 = 14;
    p[*count].x0 = 0;
    p[(*count)++].x8 = 63;
    p[*count].x0 = (flags | 0x24218000) | 0xA40000000UL;
    p[(*count)++].x8 = 6;
    p[*count].x0 = ((long)D_0029D1AC << 5) | ((long)D_0029D1B0 << 6);
    p[(*count)++].x8 = 20;
    p[*count].x0 = 2;
    p[(*count)++].x8 = 71;
    p[*count].x0 = 0x8000000044UL;
    p[(*count)++].x8 = 66;
    p[*count].x0 = 0;
    p[(*count)++].x8 = 8;
    p[*count].x0 = 0x50AB400000008001UL;
    p[(*count)++].x8 = 0x53531;
}
