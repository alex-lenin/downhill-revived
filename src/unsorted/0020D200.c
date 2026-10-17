/* UNIT 0x0020D200 0x0020D220 */
typedef struct Node {
    int x0;
    int x4;
    char pad8[0x30];
    int x38;
    unsigned short x3C;
    short x3E;
    int x40;
    int x44;
} Node;

unsigned short func_0020D200(Node *nodes, int i)
{
    return nodes[nodes[i].x40].x3C;
}
