/* UNIT 0x001181F8 0x001182F0 */
typedef struct Node {
    char pad0[0x8];
    float x8;
    char padC[0xA];
    unsigned char count;
    char pad17;
    short links[4];
} Node;

typedef struct Graph {
    char pad0[0xC];
    Node *nodes;
    char pad10[0x4];
    short x14[1];
} Graph;

extern float D_0029C114[];
extern int func_00117A10(Graph *, float);

void func_001181F8(Graph *g, int index)
{
    int hi;
    int lo;
    int i;
    float min;
    Node *node;

    node = &g->nodes[index];
    hi = func_00117A10(g, node->x8);
    min = D_0029C114[0];
    for (i = 0; i < node->count; i++) {
        Node *other = &g->nodes[node->links[i]];
        if (other->x8 < min) {
            min = other->x8;
        }
    }
    lo = func_00117A10(g, min);
    for (i = hi; i >= lo; i--) {
        if (g->x14[i] < 0) {
            g->x14[i] = index;
        } else if (index < g->x14[i]) {
            g->x14[i] = index;
        }
    }
}
