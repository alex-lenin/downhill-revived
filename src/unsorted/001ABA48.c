/* UNIT 0x001ABA48 0x001ABB48 */
/* gcc 2.95 vtable entry: this-adjustment, then function */
typedef struct VEntry {
    short delta;
    short pad2;
    void (*func)(void *);
} VEntry;

typedef struct Record {
    char pad0[0x10];
    float x10;
    float x14;
    char pad18[0x4];
    float x1C;
    signed char x20;
    signed char x21;
    char pad22[0xE];
} Record;

typedef struct Loader {
    Record records[0x79];
    char pad16B0[0x1C];
    int x16CC;
    char pad16D0[0x8];
    int x16D8;
    VEntry *vtbl;
} Loader;

typedef struct Target {
    char pad0[0x50];
    int x50;
    char pad54[0x4];
    int x58;
} Target;

/* file image loaded at a fixed address */
typedef struct FileHeader {
    int magic;
    int count;
    Record records[1];
} FileHeader;

#define FILE_BUFFER ((FileHeader *)0x91CB00)

extern Target D_0043F9F0;
extern int func_0020D930(void *dst, char *name);
extern void *memcpy(void *, const void *, unsigned int);
extern Record *func_001ABB70(Loader *obj, int index);
extern void func_001A9698(Target *target, Record *rec, float a, float b, float c, int d, int e);

void func_001ABA48(Loader *obj, char *name)
{
    int i;
    int size;
    int count;
    Record *rec;
    VEntry *entry;

    size = func_0020D930(FILE_BUFFER, name);
    obj->x16D8 = 0;
    if (size <= 0)
        return;
    if (FILE_BUFFER->magic != 0x10182001)
        return;
    count = FILE_BUFFER->count;
    obj->x16CC = count;
    memcpy(obj, FILE_BUFFER->records, count * sizeof(Record));
    entry = &obj->vtbl[1];
    entry->func((char *)obj + entry->delta);
    D_0043F9F0.x50 = 0;
    D_0043F9F0.x58 = 0;
    for (i = 0; i < obj->x16CC; i++) {
        rec = func_001ABB70(obj, i);
        func_001A9698(&D_0043F9F0, rec, rec->x10, rec->x14, rec->x1C, rec->x20, rec->x21);
    }
}
