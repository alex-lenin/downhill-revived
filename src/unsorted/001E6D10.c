/* UNIT 0x001E6D10 0x001E6E28 */
typedef struct Obj {
    char pad0[0x1088];
    int keys[21];
    int count;
} Obj;

extern int D_002719FC[][5];
extern int D_00271D5C[][5];

int func_001E6D10(Obj *obj, int alt)
{
    int i;
    int found;
    int j;

    i = 1;
    if (obj->count == 0) {
        return 0;
    }
    do {
        found = i;
        for (j = 0; j < obj->count; j++) {
            int *plain = D_002719FC[i];
            if (alt) {
                if (obj->keys[j] != D_00271D5C[i][j]) {
                    found = 0;
                }
            } else {
                if (obj->keys[j] != plain[j]) {
                    found = 0;
                }
            }
        }
        if (j < 4) {
            if (alt) {
                if (D_00271D5C[i][j] != 0) {
                    found = 0;
                }
            } else {
                if (D_002719FC[i][j] != 0) {
                    found = 0;
                }
            }
        }
        i++;
    } while (i < 43 && found == 0);
    return found;
}
