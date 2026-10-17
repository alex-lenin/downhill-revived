/* UNIT 0x0023DF70 0x0023DF88 */
typedef struct Entry120 {
    char pad0[0x120];
} Entry120;

extern Entry120 D_0075B3F0[];

Entry120 *func_0023DF70(int index) {
    return &D_0075B3F0[index];
}
