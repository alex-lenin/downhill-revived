/* UNIT 0x00162B50 0x00162B98 */
struct Block { char bytes[32]; };
int func_00162B50(struct Block *source, struct Block *dest) {
    *dest = *source;
    return 32;
}
