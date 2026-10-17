/* UNIT 0x002205B8 0x00220618 */
extern int D_00660D80[];
extern int func_002201B0(int);
int func_002205B8(int index) {
    int *state = &D_00660D80[index];
    if (*state == 0) {
        if (func_002201B0(index)) { *state = 1; return 1; }
        return 0;
    }
    return 0;
}
