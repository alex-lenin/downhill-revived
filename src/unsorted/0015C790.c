/* UNIT 0x0015C790 0x0015C7C0 */
typedef struct Node { char pad0[8]; void *x8; } Node;
int func_0015C790(char **obj) {
    char *base = *obj;
    Node *node = (Node *)(base + 0x5E90);
    if (!base || !node) return 0;
    return node->x8 == base + 0x5EEC;
}
