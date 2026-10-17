/* UNIT 0x0021B2E0 0x0021B328 */
typedef struct { int x0; unsigned int x4; int x8; int xC; } Entry;
extern Entry D_00621310[];
extern char D_00625190[];
void func_0021B2E0(int index, int value) {
    *(int *)(D_00625190 + D_00621310[index].x8 * 8 + ((index[D_00621310].x4 >> 20) & 31) * 0x960) = value;
}
