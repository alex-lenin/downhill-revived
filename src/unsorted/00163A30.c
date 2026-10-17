/* UNIT 0x00163A30 0x00163B08 */
typedef struct VTable {
    char pad0[0x10];
    short delta;
    short pad12;
    void (*func)(void *);
} VTable;

typedef struct Entry {
    char pad0[0x12];
    short x12;
} Entry;

typedef struct Save {
    void *x0;
    char pad4[0x5C - 0x4];
    int count;
    char pad60[0x68 - 0x60];
    int x68;
    char pad6C[0x70 - 0x6C];
    VTable *vtbl;
} Save;

extern int func_0020D930(void *addr, char *path);
extern void func_00163B08(Save *save);
extern Entry *func_00163D40(Save *save, int index);
extern void *memcpy(void *dst, const void *src, unsigned int n);

void func_00163A30(Save *save, char *path)
{
    int i;
    int size;

    size = func_0020D930((void *)0x91CB00, path);
    save->x68 = 0;
    if (size <= 0 || *(int *)0x91CB00 != 0x10182001) {
        func_00163B08(save);
        save->vtbl->func((char *)save + save->vtbl->delta);
        return;
    }
    save->count = *(int *)0x91CB04;
    memcpy(save->x0, (void *)0x91CB08, save->count << 5);
    for (i = 0; i < save->count; i++) {
        func_00163D40(save, i)->x12 = 0;
    }
    save->vtbl->func((char *)save + save->vtbl->delta);
}
