/* UNIT 0x00127BB8 0x00127CC8 */
typedef struct Item {
    char pad0[0x18];
    float start;
    char pad1C[0x28 - 0x1C];
    float end;
    char pad2C[0xF0 - 0x2C];
} Item;

typedef struct List {
    int count;
    char pad4[0xC];
    Item items[1];
} List;

typedef struct Range {
    char pad0[0x198];
    float lo;
    float hi;
} Range;

extern Range D_002FF2C0;
extern int func_001286D8(List *, int);
extern void func_00127CC8(List *, int);
extern void func_001284F0(List *, int);

void func_00127BB8(List *list)
{
    Item *item;
    int i;
    int outside;
    int inside;
    float hi, lo, start, end;

    for (i = 0; i < list->count; i++) {
        item = &list->items[i];
        hi = D_002FF2C0.hi;
        lo = D_002FF2C0.lo;
        start = item->start;
        end = item->end;
        inside = 0;
        outside = 0;
        if (hi < start || (end < hi && lo < start)) {
            outside = 1;
        } else if (lo < end) {
            inside = 1;
        }
        if (inside) {
            if (!func_001286D8(list, i))
                func_00127CC8(list, i);
        } else if (outside) {
            if (func_001286D8(list, i))
                func_001284F0(list, i);
        }
    }
}
