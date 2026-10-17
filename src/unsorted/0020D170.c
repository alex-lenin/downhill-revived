/* UNIT 0x0020D170 0x0020D190 */
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

int func_0020D170(Node *nodes, int i)
{
    return nodes[nodes[i].x40].x4;
}
