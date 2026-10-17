/* UNIT 0x00208EE0 0x00208FB8 */
struct Arena { int x0, x4; };
struct Node { void *x0; struct Node *x4; struct Node *x8; };
struct List { int x0; struct Node *x4; struct Node *x8; int xC; };

extern struct Arena D_00663B08[];
extern struct List D_004D0318[], D_004D0328[], D_004D0338[];
extern char D_00773080[];
extern void *func_001FFB70(int size, void *p);
extern void func_00242DE0(void);

void func_00208EE0(void) {
    int i;
    int p;
    struct Node *node;
    struct Node *next;

    D_004D0318[0].x8 = (struct Node *)D_004D0318;
    D_004D0318[0].x4 = (struct Node *)D_004D0318;
    D_004D0328[0].x8 = (struct Node *)D_004D0328;
    D_004D0328[0].x4 = (struct Node *)D_004D0328;
    D_004D0338[0].x8 = (struct Node *)D_004D0338;
    D_004D0338[0].x4 = (struct Node *)D_004D0338;
    for (i = 0; i < 800; i++) {
        p = (D_00663B08[0].x4 + 15) & ~15;
        D_00663B08[0].x4 = p + 12;
        node = func_001FFB70(12, (void *)p);
        next = D_004D0328[0].x4;
        node->x0 = D_00773080 + i * 0x100;
        node->x8 = (struct Node *)D_004D0328;
        node->x4 = next;
        D_004D0328[0].x4 = node;
        if (next != 0) {
            next->x8 = node;
        }
    }
    func_00242DE0();
}
