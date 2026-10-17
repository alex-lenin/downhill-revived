/* UNIT 0x0011D048 0x0011D148 .rodata=0x288B70:0x288C20 */
typedef struct Node {
    unsigned int flags; /* bits 0-5 = node type, bits 18+ = id */
    char pad4[0xC];
    struct Node *child;
} Node;

typedef struct Shadow {
    char pad0[0x4];
    unsigned char x4;
} Shadow;

typedef struct AnimalHerd {
    char pad0[0x4998];
    Shadow *shadows[0x3F];
    int shadowCount;
} AnimalHerd;

extern void func_00254D08(const char *fmt, ...);
extern Shadow *func_00208FB8(void);
extern void func_002182A8(Shadow *shadow, int a);
extern void func_00218278(Shadow *shadow, Node *node);

void func_0011D048(AnimalHerd *herd, Node *node)
{
    Node *group;

    if ((node->flags & 0x3F) != 6) {
        func_00254D08("AnimalHerd::addShadow  EXPECTING SWITCH NODE FOR SHADOW ENTRY ID\n");
        return;
    }
    group = node->child;
    if ((group->flags & 0x3F) != 1) {
        func_00254D08("AnimalHerd::addShadow  EXPECTING GROUP NODE FOR SHADOW ENTRY \n");
        return;
    }
    herd->shadows[herd->shadowCount] = func_00208FB8();
    if (herd->shadows[herd->shadowCount] != 0) {
        herd->shadows[herd->shadowCount]->x4 = 0;
        func_002182A8(herd->shadows[herd->shadowCount], 0);
        func_00218278(herd->shadows[herd->shadowCount], group);
        herd->shadowCount++;
    } else {
        func_00254D08("Bad shadow CS for animal %d\n", group->flags >> 18);
    }
}
