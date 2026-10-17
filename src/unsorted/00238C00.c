/* UNIT 0x00238C00 0x00238CB0 */
#define T1_COUNT ((volatile int *)0x10000800)

extern int D_0029E3E0;
extern int D_0029E3D8;
extern int func_00243CE8(int);

int func_00238C00(int *result)
{
    int count;
    int frames;
    int i;

    i = 2;
    count = *T1_COUNT;
    D_0029E3E0 += count;
    *T1_COUNT = 0;
    if (result != 0) {
        *result = func_00243CE8(0) == 0;
    } else {
        func_00243CE8(0);
    }
    frames = D_0029E3D8;
    do {
        if (count <= 0) {
            break;
        }
        frames++;
        count -= 9619;
    } while (--i >= 0);
    D_0029E3D8 = frames;
    return 1;
}
