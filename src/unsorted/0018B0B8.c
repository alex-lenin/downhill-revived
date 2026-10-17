/* UNIT 0x0018B0B8 0x0018B280 */
/* func_0018B0B8: init 14 volume levels, then create 4x14 nodes.
 * D_003F4780 is int[4][14] of node pointers (sibling 0018B280 agrees).
 * D_003F4860 holds 14 floats (volume levels, .bss size 0x40).
 * D_004B2FB4 holds 0x28-byte entries; inner loop reads entries [4..17].x0.
 * Each node: first word = entry id, flags (long @0, bit 60 cleared),
 * then two setup calls; nodes past the first get x40/x54 = -1.0f.
 * The 0xEFFFFFFFFFFFFFFFUL flags mask matches neighbor func_0018B448.
 */
extern float D_003F4860[];
extern int D_003F4780[][14];

typedef struct Entry {
    int x0;
    char pad4[0x24];
} Entry;
extern Entry D_004B2FB4[];

extern void func_00203590(void *, int, int, int);
extern void func_002038E0(void *, int);
extern void func_00203B68(void *, int, float);
extern float func_00203C28(void *);
extern void func_00203CB8(void *);
extern void func_00203DC8(void *, int);
extern int func_00208FB8(void);
extern void func_002185C0(void *, int);
extern void func_00221F20(void *);

void func_0018B0B8(void)
{
    int sound[4];
    int i;
    int j;
    int id;
    float *volume;
    int *row;
    int id2;
    int next;
    int stride;
    int *objectBase;
    Entry *sourceBase;
    unsigned long flagMask;
    float negativeOne;

    volume = D_003F4860;
    id = 0x2352;
    i = 13;
    do {
        sound[2] = 0;
        func_00203590(sound, id, 0, 5);
        id += 10;
        *volume++ = func_00203C28(sound);
        func_002038E0(sound, 0);
        func_00203B68(sound, 1, 1.0f);
        func_00203CB8(sound);
        i--;
    } while (i >= 0);
    j = 0;
    negativeOne = -1.0f;
    objectBase = &D_003F4780[0][0];
    sourceBase = D_004B2FB4;
    flagMask = 0xEFFFFFFFFFFFFFFFUL;
    stride = 0x38;
    do {
        stride = 0x38;
        i = 0;
        next = j + 1;
        row = (int *)((char *)objectBase + j * stride);
        id2 = 0x2352;
        for (; i < 14; i++) {
            *row = func_00208FB8();
            *(int *)*row = sourceBase[i + 4].x0;
            *(long *)*row &= flagMask;
            func_002185C0((void *)*row, 0);
            func_00221F20((void *)(*row + 0x40));
            if (i > 0) {
                *(float *)(*row + 0x54) = negativeOne;
                *(float *)(*row + 0x40) = negativeOne;
            }
            sound[2] = 0;
            func_00203590(sound, id2, 0, 1);
            row++;
            func_002038E0(sound, 1);
            id2 += 10;
            func_00203DC8(sound, 1);
        }
        j = next;
        stride = 0x38;
    } while (j < 4);
}
