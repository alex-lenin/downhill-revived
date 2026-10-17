/* UNIT 0x00118CA8 0x00118DD8 */
typedef struct Node { char pad0[8]; float x8; char padC[10]; unsigned char x16; char x17; short x18[4]; } Node;
typedef struct Graph { Node nodes[2048]; char pad10000[0x64]; int x10064; } Graph;
int func_00118CA8(Graph *graph, Node *from, Node *to, Node *previous) {
    short *links;
    int count;
    int found = 0;
    int index;
    int yes;
    Node *node;
    count = graph->x10064++;
    if (count < 101) {
        if (from == to) found = 1;
        else {
            index = 0;
            if (from->x16 != 0) {
                yes = 1;
                links = (short *)((char *)from + 0x18);
                do {
                    node = &graph->nodes[*links];
                    if (node == to) found = 1;
                    else {
                        if (to->x8 - 100.0f < node->x8) {
                            if (func_00118CA8(graph, node, to, previous)) found = yes;
                        }
                    }
                    index++;
                    if (found) previous = node;
                    links++;
                } while (index < from->x16 && found == 0);
            }
        }
    }
    return found;
}
