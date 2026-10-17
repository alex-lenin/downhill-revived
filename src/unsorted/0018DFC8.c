/* UNIT 0x0018DFC8 0x0018E098 */
typedef struct Slot {
    char pad0[6];
    unsigned char x6;
    char pad7[0x10 - 7];
} Slot;

typedef struct Obj {
    int x0;
    char pad4[0x18 - 0x4];
    Slot slots[(0x418 - 0x18) / 0x10];
    int count;
} Obj;

static inline Slot *GetSlot(Obj *obj, int i)
{
    return &obj->slots[i];
}

static inline int IsOn(Slot *slot)
{
    return slot->x6 != 0;
}

void func_0018DFC8(Obj *obj, int step)
{
    int cur;
    int i;
    int tries;
    int any;
    Slot *slot;

    any = 0;
    if (obj->count > 0) {
        slot = obj->slots;
        for (i = 0; i < obj->count; i++) {
            any |= slot->x6 != 0;
            slot++;
        }
    }
    if (!any) {
        obj->x0 = -1;
        return;
    }
    tries = 0;
    cur = obj->x0;
    cur = (cur + obj->count) % obj->count;
    while (!GetSlot(obj, cur)->x6 && tries < obj->count) {
        cur = (cur + step + obj->count) % obj->count;
        tries++;
    }
    obj->x0 = cur;
}
