/* UNIT 0x00119648 0x00119700 */
typedef struct Item {
    char pad0[0x14];
    unsigned short pad14 : 9;
    unsigned short stop : 1;
} Item;

extern Item *func_001177F8(void *list, int id);
extern Item *func_00119810(void *list, Item *item);
extern int func_00118640(void *list, Item *item);

int func_00119648(void *list, int id)
{
    Item *item;
    int next;
    int stop;
    int i;

    stop = func_001177F8(list, id)->stop;
    if (!stop) {
        next = id;
        for (i = 0; i < 5; i++) {
            item = func_00119810(list, func_001177F8(list, next));
            next = func_00118640(list, item);
            stop = item->stop;
            if (stop) {
                id = next;
                break;
            }
        }
    }
    return id;
}
