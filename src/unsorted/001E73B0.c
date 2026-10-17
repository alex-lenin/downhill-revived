/* UNIT 0x001E73B0 0x001E7440 */
struct Row {
    int x0;
    int values[8];
};

struct Obj {
    char pad0[0x9BC];
    int history[20];
    char padA0C[0x8];
    int count;
};

extern struct Row D_00272448[];

int func_001E73B0(struct Obj *obj)
{
    int row, found, idx, i;

    row = 1;
    do {
        found = row;
        idx = obj->count - 1;
        for (i = 7; i >= 0; i--) {
            if (obj->history[idx] != D_00272448[row].values[i]) {
                found = 0;
            }
            if (--idx < 0) {
                idx = 19;
            }
        }
        row++;
    } while (row < 27 && found == 0);
    return found;
}
