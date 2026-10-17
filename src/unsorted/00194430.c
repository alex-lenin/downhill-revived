/* UNIT 0x00194430 0x00194538 */
extern int D_004B26AC[];
extern int func_00182290(int);

int func_00194430(void *obj, int count, int arg)
{
    int result = 0;
    int mode;

    if (D_004B26AC[0] == 3) {
        if (count == 1) {
            result = 12;
        } else if (count == 2) {
            result = 10;
        } else if (count == 3) {
            result = 4;
        } else if (count == 4) {
            result = 2;
        }
    } else if (count > 0) {
        mode = func_00182290(arg);
        if (mode == 2) {
            if (count < 5) {
                result = 12 / count;
            }
        } else if (mode == 0) {
            if (count == 1) {
                result = 12;
            } else if (count == 2) {
                result = 6;
            } else if (count == 3) {
                result = 3;
            }
        } else if (mode == 1) {
            if (count == 1) {
                result = 12;
            } else if (count == 2) {
                result = 6;
            } else if (count == 3) {
                result = 3;
            }
        }
    }
    return result;
}
