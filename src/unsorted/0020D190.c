/* UNIT 0x0020D190 0x0020D1C8 */
typedef struct Entry { int x0, x4; char pad[0x40]; } Entry;
typedef struct List { Entry entries[80]; int count; } List;
int func_0020D190(List *list, int value)
{
    int count = list->count;
    int i = 0;
    if (count > 0) {
        int *entry = &list->entries[0].x4;
        do {
            if (*entry == value) return i;
            i++;
            entry += 18;
        } while (i < count);
    }
    return 0;
}
