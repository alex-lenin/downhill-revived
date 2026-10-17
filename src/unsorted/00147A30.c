/* UNIT 0x00147A30 0x00147A80 */
typedef struct Item {
    void *vtbl;
    char pad4[0xC];
} Item;

extern char D_002842C8[];
extern Item D_00373630[];

/* static initializer: constructs a global array of 30 objects */
void func_00147A30(int initialize, int priority)
{
    Item *item;
    int i;

    if (priority == 0xFFFF && initialize != 0) {
        item = D_00373630;
        i = 29;
        do {
            item->vtbl = D_002842C8;
            item++;
        } while (--i != -1);
    }
}
