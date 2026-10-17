/* UNIT 0x0017EF70 0x0017F050 */
typedef struct Obj {
    int x0;
    int table[25][4];
} Obj;

extern void func_0017EEF8(Obj *obj, int row, int value, int arg);

void func_0017EF70(Obj *obj, int row, int arg)
{
    int i;
    int j;
    int value;
    int target;

    target = obj->table[row][0];
    for (i = 0; i < 25; i++) {
        if (i == row) continue;
        for (j = 0; j < 4; j++) {
            value = obj->table[i][j];
            if (value == target) {
                func_0017EEF8(obj, row, value, arg);
            } else if (!value) {
                break;
            }
        }
    }
}
