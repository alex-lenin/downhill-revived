/* UNIT 0x001991B0 0x00199298 */
/* gcc 2.95 vtable entry: this-adjustment, then function */
typedef struct VEntry {
    short delta;
    short pad2;
    void (*func)(void *);
} VEntry;

typedef struct Record { char pad0[0x14]; int x14; char pad18[0x8]; } Record;

typedef struct Loader {
    Record records[0x102];
    char pad2040[0xC];
    int x204C;
    char pad2050[0x8];
    int x2058;
    VEntry *vtbl;
} Loader;

/* file image loaded at a fixed address */
typedef struct FileHeader {
    int magic;
    int count;
    Record records[1];
} FileHeader;

#define FILE_BUFFER ((FileHeader *)0x91CB00)

extern char D_004348F0[];
extern int func_0020D930(void *dst, char *name);
extern void *memcpy(void *, const void *, unsigned int);
extern void func_001962F0(void *table);
extern Record *func_001992B8(Loader *obj, int index);
extern int func_001961D8(void *table, int id, Record *rec);

void func_001991B0(Loader *obj, char *name)
{
    int i;
    int size;
    int count;
    Record *rec;
    VEntry *entry;

    size = func_0020D930(FILE_BUFFER, name);
    obj->x2058 = 0;
    if (size <= 0)
        return;
    if (FILE_BUFFER->magic != 0x10182001)
        return;
    count = FILE_BUFFER->count;
    obj->x204C = count;
    memcpy(obj, FILE_BUFFER->records, count * sizeof(Record));
    entry = &obj->vtbl[1];
    entry->func((char *)obj + entry->delta);
    func_001962F0(D_004348F0);
    for (i = 0; i < obj->x204C; i++) {
        rec = func_001992B8(obj, i);
        func_001961D8(D_004348F0, rec->x14, rec);
    }
}
