/* UNIT 0x001BB5A0 0x001BB670 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef union Qword {
    u128 q;
    long d[2];
    struct {
        int x, y, z, w;
    } i;
} Qword;

/* Writes a GIF packet for a 4-vertex gouraud strip: tag, then RGBAQ/XYZ2 pairs. */
Qword *func_001BB5A0(Qword *p, int x, int y, int w, int h, int c0, int c1, int c3, int c2)
{
    p->d[0] = 0x2026C00000008004;
    p->d[1] = 0x51;
    p++;

    p->q = 0;
    p->i.w = c0;
    p++;
    p->q = 0;
    p->i.x = x << 4;
    p->i.y = y << 4;
    p->i.z = 0xFFFF28;
    p++;

    p->q = 0;
    p->i.w = c1;
    p++;
    p->q = 0;
    p->i.x = (x + w) << 4;
    p->i.y = y << 4;
    p->i.z = 0xFFFF28;
    p++;

    p->q = 0;
    p->i.w = c2;
    p++;
    p->q = 0;
    p->i.x = (x + w) << 4;
    p->i.y = (y + h) << 4;
    p->i.z = 0xFFFF28;
    p++;

    p->q = 0;
    p->i.w = c3;
    p++;
    p->q = 0;
    p->i.x = x << 4;
    p->i.y = (y + h) << 4;
    p->i.z = 0xFFFF28;
    p++;

    return p;
}
