/* UNIT 0x00233488 0x00233630 */
typedef struct Node {
    char pad0[8];
    unsigned short count;
    char padA[2];
    char *xC;
    char **items;
} Node;

/* Loaded file header; offsets are relative to the header until fixed up. */
typedef struct Header {
    unsigned int nodeCount;
    unsigned int itemCount;
    unsigned char fixed;
    char pad9[3];
    Node **nodes;
    char **items;
} Header;

typedef struct Obj {
    char pad0[0x1D4];
    int x1D4;
    Header *header;
} Obj;

extern int func_00233658(Obj *);
extern int func_002331F8(Obj *, int);

int func_00233488(Obj *obj, Header *header, int arg2)
{
    unsigned int i;
    unsigned int j;

    obj->header = header;
    if (!obj->header->fixed) {
        obj->header->nodes = (Node **)((char *)header + (int)obj->header->nodes);
        obj->header->items = (char **)((char *)header + (int)obj->header->items);
        for (i = 0; i < obj->header->nodeCount; i++) {
            obj->header->nodes[i] = (Node *)((char *)header + (int)obj->header->nodes[i]);
            obj->header->nodes[i]->items = (char **)((char *)header + (int)obj->header->nodes[i]->items);
            for (j = 0; j < obj->header->nodes[i]->count; j++) {
                obj->header->nodes[i]->items[j] = (char *)header + (int)obj->header->nodes[i]->items[j];
            }
            obj->header->nodes[i]->xC = (char *)header + (int)obj->header->nodes[i]->xC;
        }
        for (i = 0; i < obj->header->itemCount; i++) {
            obj->header->items[i] = (char *)header + (int)obj->header->items[i];
        }
        obj->header->fixed = 1;
    }
    obj->x1D4 = arg2;
    func_002331F8(obj, func_00233658(obj));
    return 0;
}
