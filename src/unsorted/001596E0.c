/* UNIT 0x001596E0 0x00159778 */
typedef struct { char pad[0x1E3]; unsigned char active; char tail[0x1C]; } Item;
typedef struct { char pad[0x10]; Item items[20]; char gap[0x110]; int count, x2924, reset; } Group;
extern void func_00159318(Group *);
extern int func_00159690(Item *);
extern void func_0015A4F0(Item *);
void func_001596E0(Group *group)
{
    int i;
    Item *item;
    int active;
    if (group->reset) {
        func_00159318(group);
        group->reset = 0;
    } else {
        i = 0;
        if (group->count > 0) {
        active = 1;
        item = group->items;
        do {
            if (func_00159690(item)) item->active = active;
            else item->active = 0;
            i++;
            func_0015A4F0(item);
            item++;
        } while (i < group->count);
        }
    }
}
