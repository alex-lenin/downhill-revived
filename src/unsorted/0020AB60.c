/* UNIT 0x0020AB60 0x0020AC80 */
typedef struct Cell {
    unsigned int count : 10;
    unsigned int start : 22;
} Cell;

typedef struct Grid {
    char pad0[0x4];
    short w;
    short h;
    char pad8[0x1C - 0x8];
    int *x1C;
    unsigned int *x20;
    int *x24;
    Cell cells[1];
} Grid;

extern void func_0020AAC8(int *p);

void func_0020AB60(Grid *grid)
{
    int i;
    int j;
    int idx;
    unsigned int *p;
    unsigned int *end;

    if (grid == 0) {
        return;
    }
    for (i = 0; i < grid->w; i++) {
        for (j = 0; j < grid->h; j++) {
            idx = j * grid->w + i;
            p = grid->x20 + grid->cells[idx].start;
            end = p + grid->cells[idx].count;
            while (p < end) {
                func_0020AAC8(grid->x24 + (*p++ >> 7));
            }
            grid->x1C[idx] = 0;
        }
    }
}
