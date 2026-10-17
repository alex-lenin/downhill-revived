/* UNIT 0x00206C38 0x00206D18 */
typedef struct Node {
    char pad0[0x6];
    short x6;
    char pad8[0x50 - 0x8];
    struct Node *x50;
    struct Node *x54;
} Node;

typedef struct Header {
    char pad0[0x2];
    unsigned short x2;
} Header;

typedef struct Source {
    char pad0[0x4];
    float *x4;
    char pad8[0x20 - 0x8];
    Header *x20;
} Source;

typedef struct List {
    char pad0[0x4];
    Source *x4;
    char pad8[0xC - 0x8];
    float *xC;
    Node *x10;
    int x14;
} List;

int func_00206C38(List *list, Node *node) {
    Node *cur;
    int inserted;
    float *dst;
    float *src;
    int count;

    cur = list->x10;
    inserted = 0;
    if (cur != 0) {
        while (cur != 0 && !inserted) {
            if (node->x6 < cur->x6) {
                if (cur->x54 == 0) {
                    list->x10 = node;
                    inserted = 1;
                    node->x50 = cur;
                    cur->x54 = node;
                } else {
                    cur->x54->x50 = node;
                    inserted = 1;
                    node->x54 = cur->x54;
                    node->x50 = cur;
                    cur->x54 = node;
                }
            } else if (cur->x50 == 0) {
                cur->x50 = node;
                inserted = 1;
                node->x54 = cur;
            } else {
                cur = cur->x50;
            }
        }
    } else {
        inserted = 1;
        list->x10 = node;
        if (list->xC != 0) {
            dst = list->xC;
            src = list->x4->x4;
            for (count = list->x4->x20->x2; count != 0; count--) {
                *dst++ = *src++;
            }
        }
    }
    list->x14++;
    return inserted;
}
