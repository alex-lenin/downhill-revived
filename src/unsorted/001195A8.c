/* UNIT 0x001195A8 0x00119648 */
typedef struct Node {
    char pad0[0x14];
    unsigned short flags; /* bit 9 = stop marker, bit 10 = target found */
    char pad16[2];
    short next;
} Node;

extern Node *func_001177F8(void *obj, short index);

int func_001195A8(void *obj, Node *node) {
    int count = 0;
    int found = 0;

    while (node != 0 && !((((volatile Node *)node)->flags >> 9) & 1)) {
        if ((node->flags >> 10) & 1) {
            found = 1;
        } else {
            node = func_001177F8(obj, node->next);
        }
        count++;
        if (count >= 5 || found) {
            break;
        }
    }
    return found;
}
