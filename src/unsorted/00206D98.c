/* UNIT 0x00206D98 0x00206DD0 */
typedef struct Node { char pad[0x50]; struct Node *x50; } Node;
typedef struct List { char pad[0x10]; Node *x10; } List;
int func_00206D98(List *list, Node *target) {
    Node *node = list->x10;
    while (node) {
        if (node == target) return 1;
        node = node->x50;
    }
    return 0;
}
