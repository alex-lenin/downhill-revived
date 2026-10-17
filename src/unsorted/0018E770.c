/* UNIT 0x0018E770 0x0018E830 */
typedef struct Slot {
    char pad0[6];
    unsigned char x6;
    char pad7[9];
} Slot;

typedef struct Node {
    char pad0[0x18];
    Slot slots[64];
    int count;
} Node;

extern Node *func_0018EAC8(Node *node, int index);

static inline int func_IsSet(Slot *slot)
{
    return slot->x6 != 0;
}

static inline void func_SetFlag(Slot *slot, int value)
{
    slot->x6 = value;
}

void func_0018E770(Node *node)
{
    int count;
    int i;
    int j;
    int used;
    Node *child;
    int n;

    count = node->count;
    for (i = 0; i < count; i++) {
        child = func_0018EAC8(node, i);
        if (child) {
            n = child->count;
            if (n == 0) {
                func_SetFlag(&node->slots[i], 1);
            } else {
                used = 0;
                for (j = 0; j < n; j++) {
                    used += func_IsSet(&child->slots[j]);
                }
                func_SetFlag(&node->slots[i], used > 0);
            }
        }
    }
}
