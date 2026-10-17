/* UNIT 0x0023B420 0x0023B518 */
typedef struct Ring {
    int x0;
    char pad4[0x4];
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    char pad1C[0x24];
    int x40;
} Ring;

extern void func_0025A720(int);
extern void func_0025A700(int);

void func_0023B420(Ring *ring, int *ptr1, int *len1, int *ptr2, int *len2)
{
    int offset;
    int avail;
    int end;

    func_0025A720(ring->x40);
    end = ring->x10 + 2;
    avail = ((ring->x8 - end) << 11) - ring->x14;
    offset = (((ring->xC + ring->x10) << 11) + ring->x14) % ring->x18;
    if (ring->x18 - offset >= avail) {
        *ptr1 = ring->x0 + offset;
        *len1 = avail;
        *ptr2 = 0;
        *len2 = 0;
    } else {
        *ptr1 = ring->x0 + offset;
        *len1 = ring->x18 - offset;
        *ptr2 = ring->x0;
        *len2 = avail - (ring->x18 - offset);
    }
    func_0025A700(ring->x40);
}
