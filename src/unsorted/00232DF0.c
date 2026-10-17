/* UNIT 0x00232DF0 0x00232EE0 */
struct CharCode {
    unsigned short code;
    unsigned short first;
};

/* Symbol glyphs, indexed by (c - 0x20) minus the letters/digits skipped so far. */
extern struct CharCode D_0027EBD0[];
/* Ranges: 0 = digits, 1 = upper case, 2 = lower case. */
extern struct CharCode D_0027EC58[];

unsigned short func_00232DF0(unsigned char c)
{
    int skip;
    int range;
    int base;
    unsigned short result;

    skip = 0;
    range = 0;
    if (c >= 0x20 && c < 0x30) {
        skip = 1;
    } else if (c >= '0' && c <= '9') {
        range = 0;
    } else if (c >= 0x3A && c < 0x41) {
        skip = 11;
    } else if (c >= 'A' && c <= 'Z') {
        range = 1;
    } else if (c >= 0x5B && c < 0x61) {
        skip = 37;
    } else if (c >= 'a' && c <= 'z') {
        range = 2;
    } else if (c >= 0x7B && c < 0x7F) {
        skip = 63;
    } else {
        return 0;
    }
    if (skip != 0) {
        base = skip + 31;
        result = D_0027EBD0[c - base].code;
    } else {
        result = D_0027EC58[range].code + c - D_0027EC58[range].first;
    }
    /* Dead store, but it lengthens skip's lifetime so cse keeps the
       digit branch's range = 0 (needed for a match). */
    skip = 0;
    return result;
}
