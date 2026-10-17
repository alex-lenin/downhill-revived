/* UNIT 0x002102A8 0x002103D8 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct Packets {
    u128 *ptr[4];
    u128 buf[4][50];
    u128 *endPtr;
    char padC94[0xC];
    u128 end;
};

extern char *func_0023D5F0(int index);

void func_002102A8(struct Packets *pk, int count, int alt)
{
    int i;
    int j;
    u128 *p;
    u128 *src;
    char *e;

    for (i = 0; i < count; i++) {
        p = pk->ptr[i] = pk->buf[i];
        *p = 0;
        *(int *)p = 0x10000009;
        p++;
        e = func_0023D5F0(i);
        src = alt == 0 ? (u128 *)(e + 0x80) : (u128 *)(e + 0x1F0);
        for (j = 0; j < 9; j++) {
            *p = *src;
            if (j == 0) {
                *(unsigned short *)p = 0x8008;
            }
            p++;
            src++;
        }
        *p = 0;
        *(int *)p = 0x70000000;
        pk->ptr[i] = p;
    }
    pk->endPtr = &pk->end;
    pk->end = 0;
    *(int *)&pk->end = 0x70000000;
}
