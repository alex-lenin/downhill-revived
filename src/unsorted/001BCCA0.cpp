/* UNIT 0x001BCCA0 0x001BD890 .rodata=0x28F160:0x28F530 */
struct State {
    int x0, x4, x8, xC;
};

struct Obj {
    void *x0;
    unsigned int **x4;
};

struct Global {
    char pad[0x5350];
    int x5350;
    void *info() { return &x5350; }
};

extern Global D_00398C40;
extern unsigned int D_004B2840[];
extern "C" {
extern int func_001C0180(void *, unsigned int);
extern int func_00222B08(int, int);
}

void func_001BCCA0(State *state, Obj *obj) {
    int key;
    int code;

    key = func_001C0180(D_00398C40.info(), **obj->x4 >> 18);
    code = **obj->x4 >> 18;
    switch (code) {
    case 0xFB4:
    case 0xFB9:
    case 0xFA0:
    case 0xFAA:
    case 0xFAF:
    case 0xFA5:
        switch (D_004B2840[0]) {
        case 0:
        case 6:
        case 24:
            state->xC = func_00222B08(0, (key == 0) ? 1 : 2) + 0xD2;
            break;
        case 11:
        case 15:
        case 16:
            state->xC = func_00222B08(0, (key == 0) ? 2 : 1) + 0xD2;
            break;
        case 19:
        case 25:
            state->xC = func_00222B08(0, (key == 0) ? 4 : 2) + 0xD2;
            break;
        case 17:
        case 18:
        case 20:
            state->xC = func_00222B08(0, (key == 0) ? 6 : 3) + 0xA;
            break;
        case 12:
        case 21:
        case 26:
            state->xC = func_00222B08(0, (key == 0) ? 6 : 3) + 0xA;
            break;
        case 2:
        case 8:
        case 10:
            state->xC = func_00222B08(0, 3) + 0xD2;
            break;
        case 1:
        case 7:
        case 13:
            state->xC = func_00222B08(0, (key == 0) ? 6 : 3) + 0xA;
            break;
        case 27:
        case 28:
            state->xC = func_00222B08(0, (key == 0) ? 6 : 3) + 0xA;
            break;
        case 3:
        case 4:
        case 9:
            state->xC = func_00222B08(0, (key == 0) ? 6 : 3) + 0xA;
            break;
        }
        break;
    case 0xFFF:
    case 0x1009:
    case 0x100E:
    case 0x1004:
    case 0xFFA:
        switch (D_004B2840[0]) {
        case 0:
        case 6:
        case 24:
            state->xC = func_00222B08(0, (key == 0) ? 1 : 2) + 0xDC;
            break;
        case 11:
        case 15:
        case 16:
            state->xC = func_00222B08(0, (key == 0) ? 2 : 1) + 0xDC;
            break;
        case 19:
        case 25:
            state->xC = func_00222B08(0, (key == 0) ? 4 : 2) + 0xDC;
            break;
        case 17:
        case 18:
        case 20:
            state->xC = func_00222B08(0, (key == 0) ? 6 : 3) + 0x14;
            break;
        case 12:
        case 21:
        case 26:
            state->xC = func_00222B08(0, (key == 0) ? 6 : 3) + 0x14;
            break;
        case 5:
        case 14:
        case 22:
            state->xC = func_00222B08(0, (key == 0) ? 1 : 2) + 0xDC;
            break;
        }
        break;
    case 0xFC8:
    case 0xFD2:
    case 0xFCD:
    case 0xFBE:
    case 0xFC3:
        switch (D_004B2840[0]) {
        case 0:
        case 6:
        case 24:
            state->xC = func_00222B08(0, (key == 0) ? 3 : 1) + 0x1E;
            break;
        case 11:
        case 15:
        case 16:
            state->xC = func_00222B08(0, (key == 0) ? 2 : 1) + 0x1E;
            break;
        case 19:
        case 25:
            state->xC = func_00222B08(0, 2) + 0x1E;
            break;
        case 17:
        case 18:
        case 20:
            state->xC = func_00222B08(0, 3) + 0x1E;
            break;
        case 12:
        case 21:
        case 26:
            state->xC = func_00222B08(0, 5) + 0x1E;
            break;
        case 5:
        case 14:
        case 22:
            state->xC = func_00222B08(0, (key == 0) ? 1 : 2) + 0x1E;
            break;
        }
        break;
    case 0xFEB:
    case 0xFF5:
    case 0xFF0:
    case 0xFE1:
    case 0xFE6:
        switch (D_004B2840[0]) {
        case 11:
        case 15:
        case 16:
            state->xC = func_00222B08(0, (key == 0) ? 2 : 1) + 0x28;
            break;
        case 12:
        case 21:
        case 26:
            state->xC = func_00222B08(0, (key == 0) ? 3 : 2) + 0x28;
            break;
        case 3:
        case 4:
        case 9:
            state->xC = func_00222B08(0, (key == 0) ? 3 : 1) + 0x28;
            break;
        }
        break;
    case 0x1013:
        state->xC = func_00222B08(0, (key != 0) ? 0 : 2) + 0x5A;
        break;
    case 0x1018:
    case 0x1022:
    case 0x101D:
        switch (D_004B2840[0]) {
        case 0:
        case 6:
        case 24:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 11:
        case 15:
        case 16:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 19:
        case 25:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 17:
        case 18:
        case 20:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 12:
        case 21:
        case 26:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 5:
        case 14:
        case 22:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 2:
        case 8:
        case 10:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 1:
        case 7:
        case 13:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 27:
        case 28:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        case 3:
        case 4:
        case 9:
            state->xC = func_00222B08(0, 0) + 0x64;
            break;
        }
        break;
    case 0x1036:
        state->xC = func_00222B08(0, (key == 0) ? 3 : 2) + 0xB4;
        break;
    case 0x1040:
    case 0x103B:
        state->xC = func_00222B08(0, (key == 0) ? 3 : 2) + 0x32;
        break;
    case 0x104A:
    case 0x1059:
    case 0x1054:
    case 0x104F:
        state->xC = func_00222B08(0, (key != 0) ? 0 : 2) + 0x3C;
        break;
    case 0x1072:
    case 0x105E:
    case 0x1068:
    case 0x106D:
    case 0x1063:
        switch (D_004B2840[0]) {
        case 12:
        case 21:
        case 26:
            state->xC = func_00222B08(0, (key == 0) ? 4 : 5) + 0x46;
            break;
        case 3:
        case 4:
        case 9:
            state->xC = func_00222B08(0, (key == 0) ? 3 : 1) + 0x46;
            break;
        }
        break;
    case 0x10E0:
    case 0x10EA:
    case 0x10EF:
    case 0x10F4:
    case 0x10E5:
        switch (D_004B2840[0]) {
        case 2:
        case 8:
        case 10:
            state->xC = func_00222B08(0, 1) + 0x50;
            break;
        case 1:
        case 7:
        case 13:
            state->xC = func_00222B08(0, (key == 0) ? 6 : 3) + 0xA;
            break;
        }
        break;
    case 0x1086:
    case 0x108B:
    case 0x107C:
    case 0x1081:
    case 0x1077:
        switch (D_004B2840[0]) {
        case 17:
        case 18:
        case 20:
            state->xC = func_00222B08(0, key == 0) + 0x6E;
            break;
        case 5:
        case 14:
        case 22:
            state->xC = func_00222B08(0, (key == 0) ? 1 : 2) + 0x6E;
            break;
        }
        break;
    case 0x109F:
    case 0x10A4:
    case 0x1090:
    case 0x1095:
    case 0x109A:
        switch (D_004B2840[0]) {
        case 17:
        case 18:
        case 20:
            state->xC = func_00222B08(0, key == 0) + 0x78;
            break;
        case 5:
        case 14:
        case 22:
            state->xC = func_00222B08(0, (key == 0) ? 1 : 2) + 0x78;
            break;
        }
        break;
    case 0x10B3:
    case 0x10BD:
    case 0x10B8:
    case 0x10A9:
    case 0x10AE:
        switch (D_004B2840[0]) {
        case 17:
        case 18:
        case 20:
            state->xC = func_00222B08(0, 1) + 0x82;
            break;
        case 5:
        case 14:
        case 22:
            state->xC = func_00222B08(0, (key == 0) ? 1 : 2) + 0x82;
            break;
        }
        break;
    case 0x10D1:
    case 0x10C7:
    case 0x10CC:
    case 0x10D6:
    case 0x10C2:
        state->xC = func_00222B08(0, (key == 0) ? 1 : 2) + 0x8C;
        break;
    case 0x10DB:
        state->xC = func_00222B08(0, key == 0) + 0x96;
        break;
    case 0x523:
    case 0x528:
    case 0x51E:
        state->xC = func_00222B08(0, key == 0) + 0xAA;
        break;
    case 0x102C:
    case 0x1031:
    case 0x1027:
        state->xC = func_00222B08(0, 0) + 0xA0;
        break;
    }
}
