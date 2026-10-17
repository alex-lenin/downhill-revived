/* UNIT 0x0018A828 0x0018A868 */
typedef struct Entry {
    int x0;
    char pad4[0x10];
} Entry;

typedef struct Table {
    char pad0[0xD0];
    Entry entries[1];
} Table;

extern Table D_003EB760;
extern int func_00261F60(Table *t);

int func_0018A828(void)
{
    return D_003EB760.entries[func_00261F60(&D_003EB760)].x0 == 0;
}
