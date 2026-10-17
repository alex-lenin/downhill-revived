/* UNIT 0x0020F2A8 0x0020F420 */
extern char D_004F35C0[], D_004F3AC0[], D_00513B00[];
extern char *D_00513AC0[];
extern unsigned short *D_00513AD0[];
extern int D_00513AE0[], D_00513AF0[];
extern void func_002103D8(void *, int, void *);
void func_0020F2A8(int index) {
    int *state = &D_00513AF0[index];
    char **current = &D_00513AC0[index];
    char *p = *current;
    int offset;
    char *base;
    if (*state != 0) {
        if (*state == 2) *D_00513AD0[index] = *(unsigned short *)&D_00513AE0[index];
        *(unsigned long *)p = ((unsigned long)0x8000 << 45) | 0x8001;
        *(long *)(p + 8) = 14;
        p += 16;
        offset = index << 15;
        *(long *)(p + 8) = 74;
        base = D_004F3AC0 + offset;
        *(long *)p = 0;
        *(int *)base = ((p - D_004F3AC0 - offset) >> 4) | 0x60000000;
        func_002103D8(D_00513B00, index, D_004F35C0 + index * 0x140);
        func_002103D8(D_00513B00, index, base);
        *current = D_004F3AC0 + 0x30 + offset;
        *state = 1;
        D_00513AE0[index] = 0;
        D_00513AD0[index] = (unsigned short *)(D_004F3AC0 + 0x10 + offset);
    }
}
