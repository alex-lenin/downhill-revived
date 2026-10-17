/* UNIT 0x00193E48 0x00193F38 */
typedef struct Racer {
    char pad0[0x796C];
    int x796C;
    char pad7970[0x798C - 0x7970];
    int x798C;
    char pad7990[0x7A5C - 0x7990];
    int active;
    char pad7A60[0x7BD0 - 0x7A60];
} Racer;

typedef struct Global {
    Racer racers[10];
    char pad4D620[0x501A8 - 0x4D620];
    int racerCount;
} Global;

typedef struct Settings {
    char pad0[0x10C];
    int x10C;
    char pad110[0x2A0 - 0x110];
    int x2A0;
} Settings;

typedef struct Cell {
    char pad0[0x44];
    int score;
    char pad48[0x50 - 0x48];
} Cell;

typedef struct Row {
    Cell cells[21];
    int pad690;
} Row;

typedef struct Table {
    Row rows[1];
} Table;

extern Global D_002AF2C0;
extern Settings D_004B25A0;
extern int func_00194430(Table *table, int a, int b);

void func_00193E48(Table *table)
{
    int i;
    int points;
    Racer *racer;
    int index;
    Cell *cells;

    for (i = 0; i < D_002AF2C0.racerCount; i++) {
        racer = &D_002AF2C0.racers[i];
        if (racer->active == 1) {
            points = func_00194430(table, racer->x798C, D_004B25A0.x2A0);
            index = (racer->x796C >> 5) - 1;
            cells = table->rows[index].cells;
            cells[D_004B25A0.x10C].score += points;
        }
    }
}
