/* UNIT 0x00117CA0 0x00117E30 */
typedef struct Node {
    float pos[4];
    char pad10[4];
    unsigned short flags;
    unsigned char count;
    char pad17;
    short links[4];
} Node;

typedef struct Graph {
    char pad0[12];
    Node *nodes;
    int count;
} Graph;

extern int func_00117E30(float *pos, Node *a, Node *b);
extern float func_00118048(Node *a, Node *b, float *pos);
extern float func_00118138(Node *a, Node *b, float *pos);

short func_00117CA0(Graph *graph, float *pos, int index, float *distance) {
    float delta[4] __attribute__((aligned(16)));
    int i;
    Node *node;
    short best;

    node = &graph->nodes[index];
    best = -1;
    for (i = 0; i < node->count; i++) {
        short next = node->links[i];
        Node *other = &graph->nodes[next];
        if (func_00117E30(pos, node, other)) {
            float d = func_00118048(node, other, pos);
            if ((node->flags >> 1) & 1) {
                d += func_00118138(node, other, pos);
            }
            if (d < *distance) {
                *distance = d;
                best = next;
            }
        }
    }
    if (best < 0) {
        float x, y, z, d;
        if (node->count) {
            best = node->links[0];
        } else {
            int next = index + 1;
            int last = graph->count - 1;
            if (next >= last) next = last;
            best = next;
        }
        __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%2)" : : "r"(pos), "r"(node), "r"(delta) : "memory");
        x = delta[0];
        y = delta[1];
        z = delta[2];
        __asm__("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0" : "=&f"(d) : "f"(x), "f"(y), "f"(z));
        *distance = d;
    }
    return best;
}
