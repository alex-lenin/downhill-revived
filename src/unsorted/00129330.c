/* UNIT 0x00129330 0x001293C8 */
typedef struct { char pad[0x17C]; int active; char tail[0x20]; } Item;
typedef struct { Item items[30]; char pad[0x184]; int count; int x3248; int reset; } Group;
extern void func_00128F50(Group *);
extern int func_001292E0(Item *);
extern void func_00129C68(Item *);
void func_00129330(Group *group)
{
    int i, active;
    Item *item;
    if (group->reset) {
        func_00128F50(group);
        group->reset = 0;
    } else {
        i = 0;
        if (group->count > 0) {
            active = 1;
            item = (Item *)group;
            do {
                if (func_001292E0(item)) item->active = active;
                else item->active = 0;
                i++;
                func_00129C68(item);
                item++;
            } while (i < group->count);
        }
    }
}
