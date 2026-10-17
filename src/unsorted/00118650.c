/* UNIT 0x00118650 0x00118898 */
typedef struct Node {
    char x0[0x14];
    unsigned short flags; /* 0x14 */
    unsigned char count;  /* 0x16 */
    char x17;
    short links[4];       /* 0x18 */
} Node;

extern Node *func_00118640(Node *base, Node *node);
extern int func_00222BD8(int min, int max);
extern void func_00254D08(char *fmt, Node *node, int count);
extern char D_002889E0[];

#define FLAG_BIT(n, b) (((n)->flags >> (b)) & 1)
#define FLAG_BIT8(n) (((unsigned char *)&(n)->flags)[1] & 1)

Node *func_00118650(Node *base, Node *node, int preferred, int allow)
{
    Node *sel;
    Node *next;
    int tries;
    int idx;
    int remaining;
    int i;
    unsigned short flags;

    if (FLAG_BIT(node, 5)) {
        return node;
    }
    if ((unsigned)(node->count - 1) >= 4) {
        func_00254D08(D_002889E0, func_00118640(base, node), node->count);
    }

    tries = 0;
    idx = func_00222BD8(0, node->count - 1);
    sel = &base[node->links[idx]];
    while (tries < 7 && !allow && FLAG_BIT8(sel)) {
        tries++;
        idx = func_00222BD8(0, node->count - 1);
        sel = &base[node->links[idx]];
    }

    if (preferred) {
        if (!FLAG_BIT(sel, 12)) {
            for (i = 0; i < node->count; i++) {
                next = &base[node->links[i]];
                flags = next->flags;
                if ((flags >> 12) & 1) {
                    sel = next;
                    if (allow || !((flags >> 8) & 1)) {
                        if (func_00222BD8(0, 1)) {
                            break;
                        }
                    }
                }
            }
        }
    } else {
        remaining = node->count - 1;
        while (remaining > 0 && FLAG_BIT(sel, 12)) {
            idx++;
            remaining--;
            if (idx >= node->count) {
                idx = 0;
            }
            next = &base[node->links[idx]];
            if (allow || !FLAG_BIT8(next)) {
                sel = next;
            }
        }
    }
    return sel;
}
