/* UNIT 0x0020D4B0 0x0020D5B8 */
typedef struct Heap { int x0; char *cursor; } Heap;
typedef struct Entry { int type; char *addr; int size; char xC[0x15 - 0xC]; char name[0x38 - 0x15]; int x38; int x3C; int index; char x44; char pad45[0x48 - 0x45]; } Entry;
typedef struct Table { Entry entries[80]; int count; Heap *heap; char *x1688; } Table;
extern int func_00241F90(char *name, char *addr);
extern void func_0020DD38(char *dst, char *name);
extern void func_00254544(char *dst, char *src);
static inline char *HeapAlloc(Heap *heap)
{
    char *p;
    p = (char *)(((int)heap->cursor + 0x7F) & ~0x7F);
    heap->cursor = p + 1;
    return p;
}
char *func_0020D4B0(Table *table, int index, char *name, unsigned char type)
{
    Heap *heap;
    char *addr;
    int size;
    int useHeap;
    Entry *e;
    heap = table->heap;
    useHeap = 0;
    if (heap != 0) {
        useHeap = 1;
        addr = HeapAlloc(heap);
    } else {
        addr = table->x1688;
    }
    size = func_00241F90(name, addr);
    if (useHeap) {
        table->heap->cursor += size - 1;
    }
    e = &table->entries[index];
    e->type = type;
    e->addr = addr;
    e->size = size;
    e->x38 = 0;
    e->index = index;
    e->x44 = 0;
    func_0020DD38(e->xC, name);
    func_00254544(e->name, name);
    if (index >= table->count) {
        table->count++;
    }
    return addr;
}
