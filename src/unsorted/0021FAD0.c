/* UNIT 0x0021FAD0 0x00220018 .rodata=0x298C60:0x298DF0 */
typedef struct {
    unsigned char x0;
    unsigned char x1;
    unsigned char pad[30];
} PlayerInput;

extern int D_00660C80[2][4];
extern int D_00660CE0[];
extern int D_00660D00[];
extern int D_00660D20[];
extern int D_00660DC0[];
extern unsigned char D_00660DE0[8][6];
extern PlayerInput D_00660B80[];
extern unsigned char D_0029E2A8[8];

extern int func_002451B0(int, int, PlayerInput *);
extern int func_00245230(int, int);
extern int func_00245318(int, int);
extern int func_00245370(int, int, int, int);
extern int func_00245498(int, int, int, int);
extern int func_002455D0(int, int, int, int);
extern void func_00245748(int, int, unsigned char *);
extern int func_00245988(int, int);
extern int func_002459E8(int, int);
extern void func_002202F8(int);

void func_0021FAD0(int player, int slot)
{
    int id;
    int mode;
    int result;
    int i;
    int active;
    int type;
    unsigned char input_code;
    PlayerInput *input;

    id = D_00660C80[player][slot];
    /* slot is still in $a1 here, so the argument move vanishes (jump2
       deletes it after scheduling); it still shapes the prologue order. */
    mode = func_00245230(player, slot);
    D_00660CE0[id] = mode;
    if (mode == 0) {
        D_00660D00[id] = 0;
        D_0029E2A8[id] = 0;
    }

    switch (D_00660D00[id]) {
    case 0:
        type = D_00660CE0[id];
        if (type == 6 || type == 2) {
            result = func_00245498(player, slot, 1, 0);
            D_00660D20[id] = result;
            if (result != 0) {
                result = func_00245498(player, slot, 2, 0);
                if (result > 0) {
                    D_00660D20[id] = result;
                }
                switch (D_00660D20[id]) {
                case 4:
                    D_00660D00[id] = 40;
                    break;
                case 7:
                    D_00660D00[id] = 70;
                    break;
                default:
                    D_00660D00[id] = 99;
                    break;
                }
            }
        }
        break;
    case 40:
        if (func_00245498(player, slot, 2, 0) == 0) {
            D_00660D00[id] = 99;
            break;
        }
        D_00660D00[id]++;
        /* fall through */
    case 41:
        if (func_002455D0(player, slot, 1, 3) == 1) {
            D_00660D00[id]++;
        }
        break;
    case 42:
        if (func_00245318(player, slot) == 1) {
            D_00660D00[id]--;
        }
        if (func_00245318(player, slot) == 0) {
            D_00660D00[id] = 0;
        }
        break;
    case 70:
        if (func_00245370(player, slot, -1, 0) == 0) {
            D_00660D00[id] = 99;
        }
        for (i = 0; i < 6; i++) {
            D_00660DE0[id][i] = 255;
        }
        D_00660DE0[id][0] = 0;
        D_00660DE0[id][1] = 1;
        func_00245748(player, slot, D_00660DE0[id]);
        D_00660D00[id]++;
        func_00245318(player, slot);
        break;
    case 71:
        if (func_00245318(player, slot) == 1) {
            D_00660D00[id]--;
        }
        if (func_00245318(player, slot) == 0) {
            D_00660D00[id]++;
        }
        break;
    case 72:
        if (func_00245988(player, slot) == 0) {
            D_00660D00[id] = 99;
            D_00660DC0[id] = 0;
        }
        D_00660D00[id] = 75;
        break;
    case 75:
        if (func_00245988(player, slot) == 1) {
            D_00660D00[id] = 76;
        } else {
            D_00660D00[id] = 99;
        }
        break;
    case 76:
        if (func_002459E8(player, slot) == 1) {
            D_00660D00[id]++;
        }
        break;
    case 77:
        if (func_00245318(player, slot) == 1) {
            D_00660D00[id]--;
        }
        if (func_00245318(player, slot) == 0) {
            D_00660D00[id] = 78;
        }
        break;
    case 78:
        D_00660D00[id] = 99;
        break;
    case 99:
    default:
        active = 0;
        type = D_00660CE0[id];
        if (type != 7 && (type == 6 || type == 2)) {
            input = &D_00660B80[id];
            if (func_002451B0(player, slot, input) && input->x0 == 0) {
                input_code = input->x1;
                if (D_00660DC0[id] != 0 && D_00660DC0[id] != input_code) {
                    D_00660DC0[id] = 0;
                    D_00660D00[id] = 0;
                } else {
                    active = 1;
                }
                D_00660DC0[id] = input_code;
                func_002202F8(id);
            }
        }
        if (active) {
            if (!D_0029E2A8[id]) {
                D_0029E2A8[id] = 1;
            }
        } else {
            if (D_0029E2A8[id]) {
                D_0029E2A8[id] = 0;
            }
        }
        break;
    }
}
