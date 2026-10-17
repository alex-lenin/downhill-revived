/* UNIT 0x001E6BC8 0x001E6CC8 .sdata=0x29D744:0x29D748 */
typedef struct Pad {
    char pad0[0x724];
    int pending;
    int buttons[300];
    int codes[300];
    int history[20];
    int head;
    int historyCount;
    unsigned char enabled;
    unsigned char active;
} Pad;

int D_0029D744 = 2;

extern int func_001E6CC8(Pad *pad, int code);

void func_001E6BC8(Pad *pad, int all)
{
    int count;
    int index;
    int buttons;

    if (pad->historyCount < 20 && pad->active && pad->enabled) {
        buttons = 0;
        count = D_0029D744;
        index = pad->head - 1;
        if (!all) {
            count = pad->pending;
        }
        while (count > 0) {
            if (index < 0) {
                index += 300;
            }
            count--;
            buttons |= pad->buttons[index];
            buttons |= func_001E6CC8(pad, pad->codes[index--]);
        }
        if (buttons) {
            pad->history[pad->historyCount++] = buttons;
            pad->active = 0;
            pad->pending = 0;
        }
    }
}
