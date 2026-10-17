/* UNIT 0x00161960 0x001619B0 */
#include "particle.h"
extern int func_0022DD88(int handle);

int func_00161960(char *obj) {
    /* unused 16-byte local: retail frame is 0x20 with $ra at 0x10 */
    char unused[16];
    int result;

    if (!obj) {
        result = 0;
    } else {
        result = func_0022DD88(func_0022CFB8((Vector4 *)(obj + 0x10), obj + 0x40, 0x11, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0)));
    }
    return result;
}
