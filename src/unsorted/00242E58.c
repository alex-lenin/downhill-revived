/* UNIT 0x00242E58 0x00242E98 */
typedef struct Node { struct Node *next; int value; } Node;
extern Node *func_00242E20(void);
void func_00242E58(Node **head, int value) {
    Node *node;
    node = func_00242E20();
    node->value = value;
    node->next = *head;
    *head = node;
}
