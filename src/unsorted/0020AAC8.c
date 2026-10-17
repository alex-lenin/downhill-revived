/* UNIT 0x0020AAC8 0x0020AB60 */
typedef struct Node {
    unsigned int flags;
    char pad4[4];
    unsigned char x8;
    char pad9[0x1C - 9];
    int x1C;
    int x20;
    int x24;
    char pad28[0x50 - 0x28];
    int x50;
    int x54;
} Node;

typedef struct Obj {
    char pad0[0x1E];
    unsigned char x1E;
    char pad1F;
    Node *x20;
    int x24;
} Obj;

void func_0020AAC8(Obj *obj)
{
    Node *node;
    unsigned int flags;

    node = obj->x20;
    flags = node->flags;
    if (flags & 0x40) {
        switch (flags & 0x3F) {
        case 4:
            obj->x1E = node->x8;
            obj->x20 = (Node *)node->x50;
            obj->x24 = node->x54;
            break;
        case 3:
            obj->x1E = node->x8;
            obj->x20 = (Node *)node->x1C;
            obj->x24 = node->x20;
            break;
        case 1:
            obj->x1E = node->x8;
            obj->x20 = (Node *)node->x20;
            obj->x24 = node->x24;
            break;
        }
    }
}
