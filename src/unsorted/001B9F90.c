/* UNIT 0x001B9F90 0x001BA060 */
typedef struct Entry {
    int x0;
    char pad4[0x10 - 0x4];
    float x10[3];
    char pad1C[0x20 - 0x1C];
    int x20;
    char pad24[0x30 - 0x24];
} Entry;

typedef struct Obj {
    char pad0[0xA0C];
    int xA0C[0x50][10];
    char pad168C[0x1690 - 0x168C];
    Entry x1690[0x50][10];
    int xAC90[0x50];
    int xADD0[0x50];
    char padAF10[0xAF38 - 0xAF10];
    int xAF38;
} Obj;

/* entry j of row i, as an Obj overlay so that the fields keep their Obj offsets */
#define ENTRY(obj, i, j) ((Obj *)((char *)(obj) + (i) * 0x1E0 + (j) * 0x30))

void func_001B9F90(Obj *obj)
{
    int i;
    int j;

    obj->xAF38 = 0;
    for (i = 0; i < 0x50; i++) {
        obj->xADD0[i] = 1;
        obj->xAC90[i] = 0;
        for (j = 0; j < 10; j++) {
            obj->xA0C[i][j] = 0;
            ENTRY(obj, i, j)->x1690[0][0].x0 = 0;
            ENTRY(obj, i, j)->x1690[0][0].x20 = 0;
            if (i == 0) {
                obj->x1690[0][j].x10[0] = 0.0f;
                obj->x1690[0][j].x10[1] = 0.0f;
                obj->x1690[0][j].x10[2] = 0.0f;
            } else if (i == 1) {
                ENTRY(obj, 1, j)->x1690[0][0].x10[0] = 0.0f;
                ENTRY(obj, 1, j)->x1690[0][0].x10[1] = 0.0f;
                ENTRY(obj, 1, j)->x1690[0][0].x10[2] = 0.0f;
            } else {
                ENTRY(obj, i, j)->x1690[0][0].x10[0] = 0.0f;
                ENTRY(obj, i, j)->x1690[0][0].x10[1] = 0.0f;
                ENTRY(obj, i, j)->x1690[0][0].x10[2] = 0.0f;
            }
        }
    }
}
