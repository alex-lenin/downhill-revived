/* UNIT 0x00158840 0x00158960 */
typedef struct Entry {
    int x0;
    char pad4[0x44];
} Entry;

typedef struct Obj {
    int x0;
    int x4;
} Obj;

typedef struct Node {
    int x0;
} Node;

typedef struct List {
    int count;
    Node *nodes[1];
} List;

extern Entry D_004D0380[];
extern List *func_0020D170(Entry *table, int index);
extern void func_002068E8(Node *node, int index);
extern void func_00207B40(void);
extern void func_001589A8(void);
extern void func_00209618(Node **node, void (*cb)(void), int *args);

void func_00158840(Obj *obj, int index)
{
    List *list;
    Node **p;
    int i;
    int args[3];
    Entry *table;

    table = D_004D0380;
    list = func_0020D170(table, index);
    obj->x0 = table[index].x0;
    obj->x4 = index;
    for (i = 0; i < list->count; i++) {
        if ((list->nodes[i]->x0 & 0x3F) == 7) {
            func_002068E8(list->nodes[i], index);
        }
    }
    func_00207B40();
    i = 0;
    p = list->nodes;
    if (list->count > 0) {
        do {
            if (((*p)->x0 & 0x3F) != 7) {
                args[0] = 0;
                args[1] = 0;
                args[2] = 0;
                func_00209618(&list->nodes[i], func_001589A8, args);
            }
            i++;
            p++;
        } while (i < list->count);
    }
}
