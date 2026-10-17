/* UNIT 0x00226C10 0x00226D08 */
typedef unsigned char u8;
typedef struct {
    float x0;
    int x4;
    float x8, xC, x10, x14, x18;
} Params;

typedef struct {
    char pad0[0x2726];
    short x2726;
    char pad2728[0x2810 - 0x2728];
    int x2810;
    char pad2814[0x2830 - 0x2814];
    float *x2830;
} Obj;

void func_00226C10(void *obj, u8 *src, int value, float a, float b,
                   float c, float d, float e)
{
    char *saved = (char *)obj;
    int i;
    float *raw;

    obj = (void *)(saved + 0x2810);
    if (*(int *)obj != 0x8005) return;
    obj = (void *)(saved + 0x2726);
    if (*(short *)obj <= 0) return;
    {
        obj = *(void **)(saved + 0x2830);
        if (src != 0) {
            obj = (void *)((char *)obj - 0x13C);
            i = 15;
            do {
                i--;
                ((float *)obj)[0] = (float)src[0];
                ((float *)obj)[1] = (float)src[1];
                ((float *)obj)[2] = (float)src[2];
                ((float *)obj)[3] = (float)src[3];
                src += 4;
                obj = (void *)((char *)obj + 0x10);
            } while (i >= 0);
        }
        raw = *(float **)(saved + 0x2830);
        obj = (void *)((char *)raw - 0x28);
        raw = obj;
        if (a == 0.0f) {
        } else {
            ((Params *)obj)->x0 = a;
        }
        if (value != 0) ((int *)obj)[1] = value;
        if (c != 0.0f) ((float *)obj)[2] = c;
        ((float *)obj)[3] = e + 138.0f;
        if (b != 0.0f) ((float *)obj)[5] = b;
        if (d != 0.0f) ((float *)obj)[6] = d;
    }
}
