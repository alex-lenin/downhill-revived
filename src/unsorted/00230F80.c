/* UNIT 0x00230F80 0x00230FC8 */
#include "disc_path.h"

extern void func_00230F18(void *, void *, unsigned char);
void func_00230F80(void *p, int a1, int a2, int a3, int a4, unsigned char flags) {
    int local[16];
    BuildDiscPathWithMode((char *)(local), (const char *)(a1), (const char *)(a2), (const char *)(a3), a4);
    func_00230F18(p, local, flags);
}
