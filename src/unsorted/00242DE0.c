/* UNIT 0x00242DE0 0x00242E20 */
typedef struct Node {
    struct Node *next;
    int x4;
} Node;

extern Node D_0075D400[800]; /* node pool */
extern Node *D_0029E49C;     /* free list head */

void func_00242DE0(void)
{
    int i;

    for (i = 0; i < 799; i++) {
        D_0075D400[i].next = &D_0075D400[i + 1];
        D_0075D400[i].x4 = 0;
    }
    D_0075D400[799].next = 0;
    D_0029E49C = D_0075D400;
}
