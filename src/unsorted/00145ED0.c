/* UNIT 0x00145ED0 0x00145F20 */
typedef struct Item {
    void *vtbl;
    char pad4[0x14];
    int x18;
    char pad1C[0xC];
    int x28;
    char pad2C[0xC];
    int x38;
    char pad3C[0x184];
} Item;

extern char D_00284218[];
extern Item D_0036F060[];

/* static initializer: constructs a global array of 12 objects */
void func_00145ED0(int initialize, int priority)
{
    void *vtbl;
    Item *item;
    int i;

    if (priority == 0xFFFF && initialize != 0) {
        vtbl = D_00284218;
        item = D_0036F060;
        i = 11;
        do {
            item->vtbl = vtbl;
            item->x18 = 0;
            item->x28 = 0;
            item->x38 = 0;
            item++;
        } while (--i != -1);
    }
}
