/* UNIT 0x00210F40 0x00211280 .sdata=0x29DA50:0x29DA54 */
typedef struct Node {
    struct Node *next;
    int value;
} Node;

typedef struct Grid {
    int x0;
    short width;
    short height;
    float cellSize;
    float originX;
    float originY;
    char pad14[8];
    Node **cells;
} Grid;

typedef struct Obj {
    char pad0[0x84];
    short minX;
    short minY;
    short maxX;
    short maxY;
} Obj;

Grid *D_0029DA50 = 0;

extern void func_00242E58(Node **head, Obj *obj);
extern void func_00242E98(Node **head, Obj *obj);

static inline float fmin(float a, float b)
{
    float r;
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float fmax(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

/* Move obj to the grid cells covered by pos+/-radius; pos points at obj+0x10.
 * The int arg (cell type from the caller) is ignored. */
void func_00210F40(Obj *obj, float *pos, int type, float radius)
{
    float inv;
    float px;
    float py;
    short nb[4]; /* new cell bounds: [0]=minY [1]=minX [2]=maxY [3]=maxX */
    short x;
    short y;
    short nx; /* second nest uses its own counters (register allocation) */
    short ny;

    px = pos[0];
    py = pos[1];
    inv = 1.0f / D_0029DA50->cellSize;
    nb[1] = (int)fmax(fmin((px - radius - D_0029DA50->originX) * inv, (float)D_0029DA50->width), 0.0f);
    nb[0] = (int)fmax(fmin((py - radius - D_0029DA50->originY) * inv, (float)D_0029DA50->height), 0.0f);
    nb[3] = (int)fmax(fmin((px + radius - D_0029DA50->originX) * inv, (float)(short)(D_0029DA50->width - 1)), -1.0f);
    nb[2] = (int)fmax(fmin((py + radius - D_0029DA50->originY) * inv, (float)(short)(D_0029DA50->height - 1)), -1.0f);

    for (y = obj->minY; y <= obj->maxY; y++) {
        for (x = obj->minX; x <= obj->maxX; x++) {
            if (y < nb[0] || y > nb[2] || x < nb[1] || x > nb[3])
                func_00242E98(&D_0029DA50->cells[y * D_0029DA50->width + x], obj);
        }
    }
    for (ny = nb[0]; ny <= nb[2]; ny++) {
        for (nx = nb[1]; nx <= nb[3]; nx++) {
            if (ny < obj->minY || ny > obj->maxY || nx < obj->minX || nx > obj->maxX)
                func_00242E58(&D_0029DA50->cells[ny * D_0029DA50->width + nx], obj);
        }
    }
    obj->minY = nb[0];
    obj->maxY = nb[2];
    obj->minX = nb[1];
    obj->maxX = nb[3];
}
