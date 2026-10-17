/* UNIT 0x00220AE0 0x00220CB8 */
extern unsigned char D_0029E2A8[8];
extern int D_00660D20[];
extern int D_0029E290;
extern int func_002208E8(int, int);

/* Maps a raw 0..255 analog reading to -127..127 with a dead zone around the centre. */
int func_00220AE0(int arg, int index) {
    int value = 0;
    float input, offset, scaled;

    if (D_0029E2A8[index] && D_00660D20[index] != 4) {
        value = func_002208E8(arg, index);
        if (value >= 226) {
            offset = (float)value - 225.0f;
            value = (int)(offset * 2.133333444595337f + 64.0f);
            if (value >= 128) value = 127;
        } else if (value >= 163) {
            scaled = (float)value - 162.0f;
            value = (int)scaled;
            if (value >= 65) value = 64;
        } else if (value < 30) {
            offset = (float)value;
            scaled = offset * 2.133333444595337f + -127.0f;
            value = (int)scaled;
            if (value <= -128) value = -127;
        } else if (value < 92) {
            input = (float)value;
            offset = 92.0f - input;
            scaled = offset * -1.0322580337524414f;
            value = (int)scaled;
            if (value <= -65) value = -64;
        } else {
            value = 0;
        }
        if (value >= 128) value = 127;
        else if (value <= -128) value = -127;
        if (__builtin_abs(value) <= 1) value = 0;
    }
    if (!D_0029E290) {
        if (__builtin_abs(value) <= 119) value = 0;
    }
    return value;
}
