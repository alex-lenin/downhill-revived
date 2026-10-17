/* UNIT 0x002105A0 0x002105B8 */
struct Quad {
    int x0;
    int x4;
    int x8;
    int xC;
};

void func_002105A0(int unused, struct Quad *q, int a, int b, int c, int d)
{
    q->x0 = a;
    q->x4 = b;
    q->x8 = c;
    q->xC = d;
}
