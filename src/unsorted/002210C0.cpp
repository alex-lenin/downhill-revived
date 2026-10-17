/* UNIT 0x002210C0 0x002211F0 */
struct Item {
    unsigned long flags;
    char pad8[8];
    float x10[4];
    float x20;
    float x24;
    float x28;
    float x2c;
};

struct Node {
    Item *item;
    Node *next;
};

struct Iterator {
    Node *node;
    Iterator(Node *n) : node(n) {}
    Item *operator*() { return node->item; }
    void operator++() { node = node->next; }
    int operator!=(const Iterator &o) const { return node != o.node; }
};

struct List {
    Node sentinel;
    Iterator begin() { return Iterator(sentinel.next); }
    Iterator end() { return Iterator(&sentinel); }
};

extern int func_00220DA0(float *, float *);

void func_002210C0(List *list)
{
    float v[4];
    Iterator it = list->begin();

    for (; it != list->end(); ++it) {
        if (((*it)->flags & (1UL << 62)) && func_00220DA0((*it)->x10, v)) {
            (*it)->x20 = v[0] * 46.08f;
            (*it)->x24 = v[1] * 46.08f;
            (*it)->x28 = v[2] * 46.08f;
            (*it)->x2c = (v[0] * 0.3f + v[1] * 0.6f + v[2] * 0.1f) * 1.2f;
        }
    }
}
