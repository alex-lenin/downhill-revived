/* UNIT 0x0020D290 0x0020D2D8 */
#include "disc_path.h"

extern void func_0020D2D8(void *, char *, int);
void func_0020D290(void *arg0, const char *arg1, int arg2, int arg3, int arg4, unsigned char value) {
    char name[64];
    BuildDiscPathWithMode((char *)(name), (const char *)(arg1), (const char *)(arg2), (const char *)(arg3), arg4);
    func_0020D2D8(arg0, name, value);
}
