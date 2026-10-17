/* UNIT 0x00211610 0x002116E8 */
typedef struct Item {
    char pad0[0x8];
    float x8;           /* 0x8: secondary key */
    char padC[0x10];
    float x1C;          /* 0x1C: primary key */
    char pad20[0x10];
} Item;                 /* size 0x30 */

/* Insertion-sorts &items[0..count) into the pointer list sorted[0..*n). */
void func_00211610(Item *items, int count, Item **sorted, int *n)
{
    int i;
    int j;
    Item *item;

    if (count == 0) {
        return;
    }
    i = 0;
    if (*n == 0) {
        sorted[0] = items;
        i = 1;
        (*n)++;
    }
    for (; i < count; i++) {
        item = &items[i];
        j = (*n)++;
        while (item->x1C < sorted[j - 1]->x1C
               || (item->x1C < 0.0f && sorted[j - 1]->x8 < item->x8)) {
            sorted[j] = sorted[j - 1];
            if (--j <= 0) {
                break;
            }
        }
        sorted[j] = item;
    }
}
