/* UNIT 0x00208FB8 0x00209050 */
#include "scene.h"
struct Node {
    void *x0;
    Node *next;
    Node *prev;
};

struct ListIter {
    Node *node;
    ListIter(Node *n) : node(n) {}
    void *operator*() const { return node->x0; }
};

struct List {
    int x0;
    Node *next;
    Node *prev;
    int xC;
    ListIter begin() { return ListIter(next); }
};

extern "C" {
extern List D_004D0318;
extern List D_004D0328;

void *func_00208FB8(void);
}

void *func_00208FB8(void) {
    ListIter it = D_004D0328.begin();
    if (*it) {
        Node *node = it.node;
        if (node->prev) node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;
        node->next = 0;
        node->prev = (Node *)&D_004D0318;
        node->next = D_004D0318.next;
        D_004D0318.next = node;
        if (node->next) node->next->prev = node;
        func_002182D0((SceneNode *)*it);
        return *it;
    }
    return 0;
}
