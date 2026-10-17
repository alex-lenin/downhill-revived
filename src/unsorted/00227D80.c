/* UNIT 0x00227D80 0x00227DB8 */
typedef struct { char pad[6]; short x6; char pad8[0x38]; } Entry;
extern Entry D_0027BFC0[];
float func_00227D80(int index) {
    return index[D_0027BFC0].x6 * 0.016666668f;
}

