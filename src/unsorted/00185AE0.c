/* UNIT 0x00185AE0 0x00185BB8 */
typedef struct Entry {
    char x0;
    char pad1[3];
    int x4;
    int x8;
    int xC;
    char x10;
    char pad11[3];
} Entry;

typedef struct Input {
    char pad0[0xC];
    int xC;
    int x10;
    char pad14[0xC4 - 0x14];
    Entry entries[1];
} Input;

extern int func_002467D8(int, int, int *, int *, int *);
extern int func_002466A0(int, int *, int *);
extern int func_00261F60(Input *);

#define ENTRY(addr) ((Entry *)(addr))

int func_00185AE0(int *state, Input *input)
{
    int status;
    int cmd;
    int xC;
    int x8;
    int x4;
    /* The entry address is held as an int, not an Entry *: the stores at
       nonzero offsets then keep their order relative to the stack loads. */
    int entry;

    cmd = 1;
    func_002467D8(input->x10, input->xC, &x4, &x8, &xC);
    func_002466A0(0, &cmd, &status);
    entry = (int)&input->entries[func_00261F60(input)];
    ENTRY(entry)->x10 = (status == -2 || status == -1 || (ENTRY(entry)->x4 != 0 && status < 0));
    ENTRY(entry)->x0 = 1;
    ENTRY(entry)->x4 = x4;
    ENTRY(entry)->xC = xC;
    ENTRY(entry)->x8 = x8;
    if (ENTRY(entry)->xC == 0) {
        ENTRY(entry)->x8 = 8000;
    }
    if (status < -9) {
        ENTRY(entry)->x4 = 0;
    }
    return 1;
}
