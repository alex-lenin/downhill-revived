/* UNIT 0x002112D0 0x00211390 */
typedef struct Node Node;

typedef struct Grid {
    char pad0[0x4];
    short width;
    char pad6[0x16];
    Node **cells;
} Grid;

typedef struct Obj {
    char pad0[0x84];
    short minX;
    short minY;
    short maxX;
    short maxY;
} Obj;

extern Grid *D_0029DA50;
extern void func_00242E98(Node **head, Obj *value);

/* Remove obj from every grid cell it covers, then reset its cell bounds. */
void func_002112D0(Obj *obj)
{
    int y;
    int x;

    for (y = obj->minY; y <= obj->maxY; y++) {
        for (x = obj->minX; x <= obj->maxX; x++) {
            func_00242E98(&D_0029DA50->cells[y * D_0029DA50->width + x], obj);
        }
    }
    obj->minX = 0x7FFF;
    obj->minY = 0x7FFF;
    obj->maxX = -0x7FFF;
    obj->maxY = -0x7FFF;
}
