/* UNIT 0x00210578 0x002105A0 */
struct Quad {
    int x0;
    int x4;
    int x8;
    int xC;
};

void func_00210578(int a, struct Quad *q, int x, int y)
{
    q->x0 = x << 4;
    q->x4 = y << 4;
    q->x8 = 0xFFFF28;
    q->xC = 0;
}
