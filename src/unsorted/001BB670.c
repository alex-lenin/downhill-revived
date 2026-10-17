/* UNIT 0x001BB670 0x001BB750 */
struct Slots {
    int ids[15];
    unsigned char flags[15];
};

extern char D_004225A0[];
extern unsigned char func_00193FE8(void *, int);

void func_001BB670(struct Slots *slots) {
    int i;

    slots->ids[0] = 0;
    slots->ids[1] = 0;
    slots->ids[2] = 1;
    slots->ids[3] = 2;
    slots->ids[4] = 3;
    slots->ids[5] = 4;
    slots->ids[6] = 5;
    slots->ids[7] = 10;
    slots->ids[8] = 8;
    slots->ids[9] = 6;
    slots->ids[10] = 7;
    slots->ids[11] = 12;
    slots->ids[12] = 9;
    slots->ids[13] = 11;
    slots->ids[14] = 13;
    for (i = 1; i < 15; i++) {
        slots->flags[i] = func_00193FE8(D_004225A0, slots->ids[i]);
    }
}
