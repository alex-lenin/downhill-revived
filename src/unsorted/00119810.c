/* UNIT 0x00119810 0x00119958 */
typedef struct Node {
    char pad0[0x16];
    unsigned char count, pad17;
    short links[4];
} Node;
typedef struct Graph { Node nodes[2048]; char pad10000[0x4C]; int count; } Graph;
extern int func_00118640(Graph *graph, Node *node);
extern Node *func_001177F8(Graph *graph, int index);

/* Finds a node that links to `from`, searching backwards first, then forwards. */
Node *func_00119810(Graph *graph, Node *from) {
    Node *node = 0;
    int target = func_00118640(graph, from);
    int found = 0;
    int index;
    int i;
    int limit;

    for (index = target; index > 0 && !found; index--) {
        node = func_001177F8(graph, index);
        for (i = 0; i < node->count; i++) {
            if (node->links[i] == target) found = 1;
        }
    }
    if (!found) {
        limit = graph->count;
        for (index = target + 1; index < limit && !found; index++) {
            node = func_001177F8(graph, index);
            for (i = 0; i < node->count; i++) {
                if (node->links[i] == target) found = 1;
            }
        }
    }
    return node;
}
