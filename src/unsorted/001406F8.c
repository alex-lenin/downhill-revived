/* UNIT 0x001406F8 0x00140810 */
typedef struct Entry {
    int x0;
    int x4;
    int x8;
} Entry;

typedef struct Global {
    char pad0[0x290];
    int count;
} Global;

typedef struct Font {
    char pad0[0xA0];
} Font;

extern Global D_004B25A0;
extern Entry *D_00365D74[];
extern Font D_004C8508[];
extern void *func_0013AF60(int);
extern void func_00218498(void *, int);
extern int func_0013A258(void);
extern int func_001F9E50(Font *, Entry *);
extern void func_001FA7D8(Font *, int);

void func_001406F8(int *sel) {
    int n = *sel;
    Entry *entry;

    if (n < 2) {
        func_00218498(func_0013AF60(0x258A), (D_004B25A0.count / 32 - 1) * 2 + n);
        func_00218498(func_0013AF60(0x258B), (D_004B25A0.count / 32 - 1) * 2 + n);
    } else {
        func_00218498(func_0013AF60(0x258A), n + 30);
        func_00218498(func_0013AF60(0x258B), n + 30);
        entry = &D_00365D74[0][func_0013A258()];
        if (func_001F9E50(D_004C8508, entry)) {
            func_001FA7D8(D_004C8508, entry->x0);
        }
    }
}
