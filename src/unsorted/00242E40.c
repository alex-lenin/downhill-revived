/* UNIT 0x00242E40 0x00242E58 */
typedef struct Node {
    struct Node *next;
} Node;

extern Node *D_0029E49C;

void func_00242E40(Node *node) {
    node->next = D_0029E49C;
    D_0029E49C = node;
}
