/* UNIT 0x001178A0 0x00117960 */
typedef struct List {
    char pad0[0x10064];
    int x10064;
} List;

extern List D_0029EA20;

extern int func_00118640(List *list, int pos);
extern void *func_001177F8(void *obj, int index);
extern int func_00118CA8(void *obj, void *item, int b, int c);

void *func_001178A0(void *obj, int a, int b)
{
    void *item;
    void *found = 0;
    int index;
    int last;
    int first;

    first = func_00118640(&D_0029EA20, a) + 1;
    last = func_00118640(&D_0029EA20, b);
    D_0029EA20.x10064 = 0;
    for (index = first; index <= last; index++) {
        item = func_001177F8(obj, index);
        if (func_00118CA8(obj, item, b, 0) != 0) {
            found = item;
            break;
        }
    }
    return found;
}
