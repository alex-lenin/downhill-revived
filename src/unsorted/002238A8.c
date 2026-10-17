/* UNIT 0x002238A8 0x00223900 */
#include "format.h"
extern char D_00298E88[];
extern int func_00254D68(char *buf, const char *fmt, ...);

int func_002238A8(int secs, char *buf)
{
    int mins = secs / 60;
    int hours = mins / 60;

    return func_00254D68(buf, D_00298E88, hours % 60, mins % 60, secs % 60);
}
