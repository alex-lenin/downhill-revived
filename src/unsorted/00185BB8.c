/* UNIT 0x00185BB8 0x00185CC8 .sdata=0x29CD68:0x29CD6C .sbss=0x29DE08:0x29DE18 */
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

int D_0029CD68 = 1;
int D_0029DE08 __attribute__((section(".sbss"))) = 0;
int D_0029DE0C __attribute__((section(".sbss"))) = 0;
int D_0029DE10 __attribute__((section(".sbss"))) = 0;
int D_0029DE14 __attribute__((section(".sbss"))) = 0;

extern int func_002467D8(int, int, int *, int *, int *);
extern int func_002466A0(int, int *, int *);
extern int func_00261F60(Input *);

int func_00185BB8(int *state, Input *input)
{
    Entry *e;

    if (*state == 0) {
        func_002467D8(input->x10, input->xC, &D_0029DE08, &D_0029DE14, &D_0029DE0C);
        *state = *state + 1;
    } else if (*state != 1) {
        return 0;
    }
    if (func_002466A0(1, &D_0029CD68, &D_0029DE10)) {
        e = &input->entries[func_00261F60(input)];
        e->x10 = (D_0029DE10 == -2 || D_0029DE10 == -1 || (e->x4 != 0 && D_0029DE10 < 0));
        e->x0 = 1;
        e->x4 = D_0029DE08;
        e->xC = D_0029DE0C;
        e->x8 = D_0029DE14;
        if (e->xC == 0) {
            e->x8 = 8000;
        }
        if (D_0029DE10 < -9) {
            e->x4 = 0;
        }
        return 1;
    }
    return 0;
}
