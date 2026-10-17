/* UNIT 0x00167A00 0x00167AE0 */
#include "motion_sample.h"
extern int func_001678E0(char *base, int track, float *fraction, float time);
extern void func_00167800(char *base, float *work, char *prev, char *next, float t);
extern void func_0020ED30(int track, int a, int b, int c, float d, float e, float f, float g, float h);

int func_00167A00(void *base_view, int track, float *fraction, float *value, float *extra, float time)
{
    char *base=(char *)base_view;
    float work[12];
    int index;
    int offset;

    index = func_001678E0(base, track, fraction, time);
    offset = index * 48;
    func_00167800(base, work, base + (offset - 48), base + offset, *fraction);
    func_0020ED30(track, (int)work[4], (int)work[5], (int)work[6], 1.0f, work[0], work[1], work[7], work[2]);
    *value = work[3];
    *extra = work[10];
    return index;
}
