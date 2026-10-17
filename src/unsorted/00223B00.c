/* UNIT 0x00223B00 0x00223B50 */
typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
} Heap;

void func_00223B00(Heap *h)
{
    int top;
    int mark;
    int prev;

    h->x8 = 0;
    h->x4 = h->x0;
    top = (h->xC - 4) & ~3;
    h->x10 = top;
    h->x14 = top;
    *(int *)top = 0;

    mark = (h->x4 + 3) & ~3;
    prev = h->x8;
    h->x8 = mark;
    h->x4 = mark + 4;
    *(int *)mark = prev;
}
