/* UNIT 0x00163F98 0x001640D0 */
struct Node { char pad0[8]; float x8; char padc[0xa]; short count; short links[4]; };
struct Obj { struct Node *nodes; char pad4[0x6a]; unsigned char x6e; };
extern int func_00222BD8(int, int);
extern int func_00222B08(int, int);

struct Node *func_00163F98(struct Obj *obj, struct Node *node, float threshold) {
    int done = 0;
    int i;
    struct Node *previous;
    struct Node *neighbor;
    float best;
    int choice;

    do {
        previous = node;
        best = node->x8;
        for (i = 0; i < previous->count; i++) {
            neighbor = &obj->nodes[previous->links[i]];
            if (neighbor->x8 < best) {
                node = neighbor;
                best = neighbor->x8;
            } else if (neighbor->x8 < previous->x8) {
                if (obj->x6e) choice = func_00222BD8(0, 1);
                else choice = func_00222B08(0, 1);
                if (choice) {
                    node = neighbor;
                    best = neighbor->x8;
                }
            }
        }
        if (node->x8 < threshold || node == previous) done = 1;
    } while (!done);
    return node;
}
