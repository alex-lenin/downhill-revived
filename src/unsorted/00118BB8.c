/* UNIT 0x00118BB8 0x00118CA8 */
typedef struct Node {
    char pad0[0x16];
    unsigned char count;
    char pad17[0x1];
    short links[4];
} Node;

typedef struct Graph {
    char pad0[0x10064];
    int x10064;
} Graph;

extern Graph D_0029EA20[];
extern int func_00118CA8(Node *, Node *, Node *, int);
extern int func_00222BD8(int, int);

Node *func_00118BB8(Node *nodes, Node *from, Node *to)
{
    Node *found = 0;
    Node *node;
    short *link;
    int i;

    if (from != 0) {
        if (from == to) {
            found = to;
        } else {
            D_0029EA20[0].x10064 = 0;
            for (i = 0, link = from->links; i < from->count; link++, i++) {
                node = &nodes[*link];
                if (node != 0 && func_00118CA8(nodes, node, to, 0)) {
                    found = node;
                    if (func_00222BD8(0, 1)) {
                        break;
                    }
                }
            }
        }
    }
    return found;
}
