/* UNIT 0x0020D0F8 0x0020D170 */
typedef struct Entry { int x0, x4, x8; char xC[9]; char x15[0x33]; } Entry;
typedef struct List { Entry entries[80]; int count; } List;
extern int func_0020DD38(void *, void *);
extern int func_00254544(void *, void *);
void func_0020D0F8(List *list, int a, int b, void *source, int flags)
{
    Entry *entry = &list->entries[list->count];
    entry->x4 = a;
    entry->x0 = flags;
    entry->x8 = b;
    func_0020DD38(entry->xC, source);
    func_00254544(entry->x15, source);
    list->count++;
}
