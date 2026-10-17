/* UNIT 0x00172638 0x001726F8 .rodata=0x28B0C0:0x28B0F0 */
typedef struct Entry {
    int x0;
    int type;
    char pad8[0x128];
} Entry;

typedef struct List {
    Entry entries[7];
    int x850[14];
    int count;
} List;

extern int func_001591D8(unsigned int *);

void func_00172638(List *list, unsigned int *item)
{
    int value;
    int count;

    switch (*item >> 18) {
    case 0x500:
        list->entries[list->count].type = 1;
        break;
    case 0x505:
        list->entries[list->count].type = 2;
        break;
    case 0x508:
        list->entries[list->count].type = 3;
        break;
    case 0x501:
    case 0x506:
    case 0x509:
        return;
    default:
        list->entries[list->count].type = 0;
        break;
    }
    value = func_001591D8(item);
    count = list->count;
    list->x850[count] = value;
    list->count = count + 1;
}
