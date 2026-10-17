/* UNIT 0x00206D18 0x00206D98 */
typedef struct Node {
    char pad0[0x50];
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct List {
    char pad0[0x10];
    Node *head;
    int count;
} List;

int func_00206D18(List *list, Node *node)
{
    Node *it;

    for (it = list->head; it != 0; it = it->next) {
        if (it == node) {
            if (list->head == it) {
                list->head = it->next;
            }
            if (it->prev != 0) {
                it->prev->next = it->next;
            }
            if (it->next != 0) {
                it->next->prev = it->prev;
            }
            it->prev = 0;
            it->next = 0;
            list->count--;
            return 1;
        }
    }
    return 0;
}
