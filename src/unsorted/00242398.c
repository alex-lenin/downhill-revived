/* UNIT 0x00242398 0x00242448 */
typedef struct Ring {
    int x0;
    char x4[0x28];
    int x2C;
    char *x30; /* buffer base */
    int x34;   /* read offset */
    int x38;
    int x3C;   /* size */
} Ring;

void func_00242398(Ring *ring, char **ptr1, int *len1, char **ptr2, int *len2)
{
    int avail;
    int tail;

    if (ring->x0 == 0) {
        *ptr1 = &ring->x4[ring->x2C];
        *len1 = 0x28 - ring->x2C;
        *ptr2 = ring->x30;
        *len2 = ring->x3C;
        return;
    }
    avail = ring->x3C - ring->x38;
    tail = ring->x3C - ring->x34;
    if (tail >= avail) {
        *ptr1 = ring->x30 + ring->x34;
        *len1 = avail;
        *ptr2 = 0;
        *len2 = 0;
    } else {
        *ptr1 = ring->x30 + ring->x34;
        *len1 = ring->x3C - ring->x34;
        *ptr2 = ring->x30;
        *len2 = avail - (ring->x3C - ring->x34);
    }
}
