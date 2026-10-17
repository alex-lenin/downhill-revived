/* UNIT 0x00139A48 0x00139AF0 */
struct Item {
    int x0;
    int x4;
    int x8;
    int xC;
};

struct Row {
    char pad0[0x24];
    struct Item items[10];
};

struct Table {
    char pad0[0x20];
    int rows[10];
};

struct Obj {
    char pad0[0x31714];
    struct Table *table;
    int x31718;
    int firstRow;
    int firstCol;
};

extern struct Row *func_00215B58(int);

void func_00139A48(struct Obj *obj) {
    int row;
    int first;
    int col;
    struct Item *items;
    struct Item *item;

    first = obj->firstRow;
    if (first < 10) {
        row = first;
        do {
            items = func_00215B58(obj->table->rows[row])->items;
            col = obj->firstCol;
            if (col < 10) {
                item = (struct Item *)(col * sizeof(struct Item) + (int)items);
                do {
                    item->x0 = 0;
                    col++;
                    item->x4 = 0;
                    item->x8 = 0;
                    item->xC = 0;
                    item++;
                } while (col < 10);
            }
            row++;
        } while (row < 10);
    }
}
