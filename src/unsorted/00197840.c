/* UNIT 0x00197840 0x001979B0 */
typedef struct Vec {
    float x, y;
    float z;
    float w;
} Vec __attribute__((aligned(16)));

typedef struct Body {
    int x0;
    char pad4[0xC];
    float x10, x14, x18;
} Body;

typedef struct Entry {
    int x0;
    int x4;
    Body *x8;
    int xC;
    int x10;
} Entry;

typedef struct SlotView {
    char pad0[0x1c];
    int id;
    Body *body;
} SlotView;

typedef struct Table {
    char pad0[0x18];
    Entry entries[150];
    char padBDC[0xC00 - 0x18 - 150 * 0x14];
    int count;
} Table;

typedef struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
    int x508;
} Obj;

typedef struct Camera {
    char pad0[0x80];
    Vec x80;
} Camera;

extern Table D_004348F0;
extern Obj D_00435500;
typedef struct World {
    char pad0[0x40C0];
    char x40C0[1];
} World;

extern World D_002AF2C0;
extern Camera D_0037EA60;
extern int func_00196198(Table *);
extern void func_00231A08(void *, Vec *);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline float GetX(Body *b) { return b->x10; }
static inline float GetY(Body *b) { return b->x14; }
static inline float GetZ(Body *b) { return b->x18; }

static inline void GetPos(Entry *e, Vec *v)
{
    v->x = GetX(e->x8);
    v->y = GetY(e->x8);
    v->z = GetZ(e->x8);
}

static inline Table *GetTable(void) { return &D_004348F0; }
static inline SlotView *GetSlot(int index) {
    int off = index * 20;
    off += (int)GetTable();
    return (SlotView *)off;
}
static inline Entry *GetEntry(int index) {
    int off = index * 20;
    off += (int)GetTable();
    return (Entry *)(off + 0x18);
}
void func_00197840(Obj *obj)
{
    int count, total, found, i, index, id;
    Vec pos;
    World *world;
    Entry *entry;
    world = &D_002AF2C0;
    count = GetTable()->count;
    total = func_00196198(GetTable());
    if (count > 0 && total > 0) {
        found = 0;
        for (i = 0; !found && i < count; i++) {
            obj->x500++;
            if (obj->x500 >= count) obj->x500 = 0;
            index = obj->x500;
            if (GetSlot(index)->body->x0 != 0) {
                entry = GetEntry(index);
                GetPos(entry, &pos);
                id = GetSlot(index)->id;
                obj->x504 = id;
                D_00435500.x504 = id;
                pos.z += 1.0f;
                func_00231A08(world->x40C0, &pos);
                COPY_VECTOR(D_0037EA60.x80, &pos);
                obj->x508 = 2;
                found = 1;
            }
        }
    }
}
