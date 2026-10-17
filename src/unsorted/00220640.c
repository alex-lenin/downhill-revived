/* UNIT 0x00220640 0x00220768 */
extern unsigned char D_0029E2A8;
extern int D_00660D80[];
extern int func_00220618(int, int);
extern int func_002209A8(int, int);
extern int func_00220230(int);
extern int func_00220768(int, int);

static inline int IsAnyActive(int mask)
{
    return func_00220618(mask, 0) || func_00220618(mask, 1);
}

int func_00220640(int mask, int index, int mode)
{
    int result = 0;

    if (((unsigned char *)&D_0029E2A8)[index]) {
        if (index < 0) {
            result = IsAnyActive(mask);
        } else {
            if (mode == 0) {
                int *state = &D_00660D80[index];
                if (*state == 0 && func_002209A8(mask, index)) {
                    *state = 1;
                    return 1;
                }
            }
            if (func_00220230(index) & mask) {
                if (mode == 0) {
                    int *state = &D_00660D80[index];
                    result = 0;
                    if (*state == 0) {
                        result = 1;
                        *state = 1;
                    }
                } else {
                    result = func_00220768(mask, index);
                    result = result ? result : 1;
                }
            } else {
                result = 0;
            }
        }
    }
    return result;
}
