/* UNIT 0x00209108 0x002091A0 */
#include "scene.h"
struct Item;


struct Node {
    Item *item;
    Node *next;
    Node *prev;
};

struct ListIter {
    Node *node;
    ListIter(Node *n) : node(n) {}
    Item *operator*() const { return node->item; }
};

struct List {
    int x0;
    Node *next;
    Node *prev;
    int xC;
    ListIter begin() { return ListIter(next); }
};

extern "C" {
extern List D_004D0328;
extern List D_004D0338;
Item *func_00209108(void);
}

Item *func_00209108(void) {
    ListIter it = D_004D0328.begin();
    if (*it) {
        Node *p = it.node;
        if (p->prev) p->prev->next = p->next;
        if (p->next) p->next->prev = p->prev;
        p->next = 0;
        p->prev = (Node *)&D_004D0338;
        p->next = D_004D0338.next;
        D_004D0338.next = p;
        if (p->next) p->next->prev = p;
        func_002182D0((SceneNode *)*it);
        return *it;
    }
    return 0;
}
