/* UNIT 0x00206440 0x002065B8 */
typedef struct Item {
    char pad0[0x1C];
    union {
        unsigned int raw;
        struct {
            unsigned int x0 : 4;
            unsigned int period : 4;
            unsigned int state : 4;
            unsigned int x12 : 20;
        } f;
    } x1C;
    int x20;
    void *obj;
} Item;

typedef struct List {
    int x0;
    unsigned int count;
    Item *items[1];
} List;

typedef struct Change {
    signed char old;
    signed char next;
    short count;
} Change;

extern int D_0029E160;
extern char D_004D0248[];
extern int func_00206A78(void *obj);
extern void func_00206AD8(void *obj);
extern void func_00202BA0(void *arg, int period, int state, int flag);

void func_00206440(List *list, Change *changes)
{
    unsigned int i;
    Item **cursor;
    int frame;

    cursor = list->items;
    frame = D_0029E160;
    for (i = 0; i < list->count; i++, cursor++) {
        Item *item = *cursor;
        void *obj = item->obj;

        if (obj && func_00206A78(obj)) {
            int state = item->x1C.f.state;
            int current = frame % (int)(item->x1C.f.period + 1);
            int apply = state == current;

            if (changes) {
                Change *change = (Change *)((char *)changes + ((item->x1C.raw >> 2) & 0x3C));

                if (change->count && state == change->old) {
                    unsigned char next = change->next;

                    if (current == state || current == (signed char)next) {
                        apply = 1;
                        item->x1C.f.state = next;
                        change->count--;
                    }
                }
            }
            if (obj && apply) {
                func_00206AD8(obj);
            }
            func_00202BA0(D_004D0248, item->x1C.f.period, item->x1C.f.state, 1);
        }
    }
}
