/* UNIT 0x00242E98 0x00242F10 */
typedef struct Node { struct Node *next; int value; } Node;
extern void func_00242E40(Node *);
void func_00242E98(Node **head, int value) {
    Node **link;
    Node *next;
    if (*head) {
        link = head;
        while (value != (*link)->value) {
            link = &(*link)->next;
            if (!*link) return;
        }
        next = (*link)->next;
        func_00242E40(*link);
        *link = next;
    }
}
