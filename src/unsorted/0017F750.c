/* UNIT 0x0017F750 0x0017F8A8 */
typedef struct Entry {
    void *x0;
    char pad4[0x1C];
} Entry;

typedef struct Slot {
    char pad0[0x10];
    Entry x10[16];
    char pad210[0x10];
} Slot;

typedef struct Obj {
    char pad0[0x30];
    Slot x30[40];
    char pad5530[0x573C - 0x5530];
    int x573C;
} Obj;

typedef struct Node {
    long x0;
} Node;

#define NODE_TYPE(n) ((n)->x0 & 0x3F)
/* bits 18..31 of the node header: node id */
#define NODE_ID(n) ((int)((n)->x0 >> 18) & 0x3FFF)
/* x573C is a 1-based count of slots in use: current slot */
#define SLOT(o) (&(o)->x30[(o)->x573C] - 1)

void func_0017F750(Obj *obj, Node *node)
{
    unsigned int idx;

    if (NODE_TYPE(node) != 4) {
        return;
    }
    if ((idx = NODE_ID(node) - 0x4EE) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x50D) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x55C) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x58C) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x592) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x598) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x59E) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x5A4) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x5AA) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x5B0) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x5B6) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x570) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x576) < 2) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x57C) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x582) < 2) {
        SLOT(obj)->x10[idx].x0 = node;
    } else if ((idx = NODE_ID(node) - 0x586) < 4) {
        SLOT(obj)->x10[idx].x0 = node;
    }
}
