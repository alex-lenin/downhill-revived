/* UNIT 0x001DEDE0 0x001DEE20 .rodata=0x2938E0:0x293950 */
extern int D_004B2840[];

int func_001DEDE0(void) {
    switch (D_004B2840[0]) {
    case 0: case 1: case 2: case 3: case 5: case 11: case 12: case 19: case 20:
        return 1;
    default:
        return 0;
    }
}

/* Unreferenced string that shares this file's .rodata (it follows the jump table). */
const char D_00293938[] = "fileCdSearchFile fail: ";
