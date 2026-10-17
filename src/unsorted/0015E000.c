/* UNIT 0x0015E000 0x0015E0C0 */
typedef struct {
    char pad0[0x10C];
    int x10C;
    int x110;
    char pad114[0x240];
    int x354;
    int x358;
    char x35C;
} State;

extern State D_004B25A0;
extern char D_0037F870[], D_00386B90[], D_003F52D0[], D_0043F9F0[];
extern char D_003F53FD[];
extern void func_001A8A48(void *);
extern void func_00190660(void *, void *);
extern void func_00190688(void *);

/* entries are 16 bytes; the short at +0x1C is the entry type */
static inline int is_type(char *entry, int type) {
    return *(short *)(entry + 0x1C) == type;
}

/* obj is passed as an integer address. The then-branch is wrapped in a
   do/while (0) with returns: the loop-end note stops gcc's CSE from
   following the jump into the else branch, so the type is loaded again
   (retail bnel + lh in the delay slot). */
void func_0015E000(int obj, int index) {
    index *= 16;
    if (is_type((char *)(index + obj), 18)) {
        do {
            func_001A8A48(D_0043F9F0);
            if (D_004B25A0.x10C == 19) {
                D_003F53FD[0] = 1;
                D_004B25A0.x354 = 1;
                D_004B25A0.x35C = 0;
                return;
            } else if (D_004B25A0.x110 == 1) {
                func_00190660(D_003F52D0, D_0037F870);
                return;
            } else {
                func_00190660(D_003F52D0, D_00386B90);
                return;
            }
        } while (0);
    } else if (is_type((char *)(index + obj), 19)) {
        func_00190688(D_003F52D0);
    }
}
