/* UNIT 0x00159318 0x00159418 */
/* func_00159318: reset per-slot state, then activate slots whose lookup succeeds. */
typedef struct Slot {
    char pad0[0x1E3];
    unsigned char active; /* 0x1E3: 1 = active */
    char pad1E4[0x1C];
} Slot; /* size 0x200 */

typedef struct Entry {
    char pad0[0x8];
    float x8;
} Entry;

typedef struct Obj {
    int x0;
    char pad4[0xC];
    Slot x10[20];
    unsigned int **x2810[20];
    int x2860[20];
    char pad28B0[0x1C];
    float x28CC;
    int x28D0[20];
    int x2920;
    char pad2924[0xC];
    int x2930;
} Obj;

extern float D_0029C6D8[];
extern char D_003041B0[];
extern int func_00122258(void *, int);
extern Entry *func_00159468(Obj *, int);
extern void func_00159930(Slot *, Obj *, unsigned int **, int, int, Entry *);

/* p28D0 points at x28D0[i]; the sibling arrays are reached by fixed word
   offsets from it: x2810[i] is 0xC0 bytes back, x2860[i] is 0x70 bytes back. */
#define X2810_FROM_P28D0(p) (*(unsigned int ***)((p) - 0x30))
#define X2860_FROM_P28D0(p) (*((p) - 0x1C))

void func_00159318(Obj *obj)
{
    Entry *entry;
    Slot *slot;
    int *p28D0;
    int i;

    obj->x28CC = D_0029C6D8[0];
    obj->x2930 = -1;
    for (i = 0; i < obj->x2920; i++) {
        p28D0 = &obj->x28D0[i];
        slot = &obj->x10[i];
        if (X2810_FROM_P28D0(p28D0) != 0) {
            entry = func_00159468(obj, func_00122258(D_003041B0, **X2810_FROM_P28D0(p28D0) >> 18));
        } else {
            entry = func_00159468(obj, 0);
        }
        if (entry != 0) {
            obj->x28CC = entry->x8;
            func_00159930(slot, obj, X2810_FROM_P28D0(p28D0), X2860_FROM_P28D0(p28D0), *p28D0, entry);
            slot->active = 1;
        }
    }
    obj->x0 = 0;
}
