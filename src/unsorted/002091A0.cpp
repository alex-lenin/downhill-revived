/* UNIT 0x002091A0 0x00209248 */
struct Node {
    void *x0;
    Node *next;
    Node *prev;
};

struct ListIter {
    Node *node;
    ListIter(Node *n) : node(n) {}
    void *operator*() const { return node->x0; }
    ListIter &operator++() { node = node->next; return *this; }
    int operator!=(const ListIter &other) const { return node != other.node; }
};

struct List {
    int x0;
    Node *next;
    Node *prev;
    int xC;
    ListIter begin() { return ListIter(next); }
    ListIter end() { return ListIter((Node *)this); }
};

extern List D_004D0338, D_004D0328;
extern void func_002112D0(void *);

extern "C" void func_002091A0(void *key) {
    for (ListIter it = D_004D0338.begin(); it != D_004D0338.end(); ++it) {
        if (*it == key) {
            Node *p = it.node;
            if (p->prev) {
                p->prev->next = p->next;
            }
            if (p->next) {
                p->next->prev = p->prev;
            }
            p->next = 0;
            p->prev = (Node *)&D_004D0328;
            p->next = D_004D0328.next;
            D_004D0328.next = p;
            if (p->next) {
                p->next->prev = p;
            }
            func_002112D0(p->x0);
            return;
        }
    }
}
