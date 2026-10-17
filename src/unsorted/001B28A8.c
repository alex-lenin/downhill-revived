/* UNIT 0x001B28A8 0x001B2E48 .rodata=0x28E940:0x28E960 */
static __inline__ float *element(void *p, int index) { return (float *)p + index; }
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
extern void *func_001B3050(void *, int);
int func_001B28A8(void *arg0, void *arg1) {
    void *row, *input, *row2, *input2;
    int i, j, n;
    int result = -1;
    switch (I(arg1,0)) {
    case 0x1B59:
        F(P(arg0,0x80C0),0x1C88) = F(arg1,0x8);
        F(P(arg0,0x80C0),0x2124) = F(arg1,0xC);
        F(P(arg0,0x80C0),0x1C28) = F(arg1,0x14);
        F(P(arg0,0x80C0),0x1C30) = F(arg1,0x18);
        F(P(arg0,0x80C0),0x1C2C) = F(arg1,0x1C);
        F(P(arg0,0x80C0),0x1C48) = F(arg1,0x24);
        F(P(arg0,0x80C0),0x1C50) = F(arg1,0x28);
        F(P(arg0,0x80C0),0x1C4C) = F(arg1,0x2C);
        F(P(arg0,0x80C0),0x1C68) = F(arg1,0x34);
        F(P(arg0,0x80C0),0x1C70) = F(arg1,0x38);
        F(P(arg0,0x80C0),0x1C6C) = F(arg1,0x3C);
        F(P(P(arg0,0x80C0),0x16D0),0x45CC) = F(arg1,0x4C);
        F(P(P(arg0,0x80C0),0x16D0),0x45D4) = F(arg1,0x60);
        result = I(arg1,4);
        break;
    case 0x1B5A:
        F(P(arg0,0x80C0),0x1C38) = F(arg1,0x14);
        F(P(arg0,0x80C0),0x1C58) = F(arg1,0x18);
        F(P(arg0,0x80C0),0x1C8C) = F(arg1,0x28);
        F(P(arg0,0x80C0),0x1C3C) = F(arg1,0x5C);
        F(P(arg0,0x80C0),0x1C5C) = F(arg1,0x60);
        F(P(P(arg0,0x80C0),0x16D0),0x48C8) = F(arg1,0x64);
        F(P(P(arg0,0x80C0),0x16D0),0x48C4) = F(arg1,0x68);
        F(P(P(arg0,0x80C0),0x16D0),0x623C) = F(arg1,0x6C);
        F(P(P(arg0,0x80C0),0x16D0),0x6240) = F(arg1,0x70);
        F(P(P(arg0,0x80C0),0x16D0),0x5E5C) = F(arg1,0x80);
        F(P(P(arg0,0x80C0),0x16D0),0x6244) = F(arg1,0x84);
        F(P(P(arg0,0x80C0),0x16D0),0x48DC) = F(arg1,0x88);
        F(P(P(arg0,0x80C0),0x16D0),0x49BC) = F(arg1,0x8C);
        F(P(P(arg0,0x80C0),0x16D0),0x48D0) = F(arg1,0x90);
        F(P(P(arg0,0x80C0),0x16D0),0x48CC) = F(arg1,0x94);
        F(P(P(arg0,0x80C0),0x16D0),0x6234) = F(arg1,0x98);
        F(P(arg0,0x80C0),0x1804) = F(arg1,0xCC);
        F(P(arg0,0x80C0),0x1808) = F(arg1,0xD0);
        F(P(arg0,0x80C0),0x180C) = F(arg1,0xD4);
        result = I(arg1,4);
        break;
    case 0x1B5B:
        F(P(P(arg0,0x80C0),0x16D0),0x4780) = F(arg1,0x8);
        F(P(P(arg0,0x80C0),0x16D0),0x4784) = F(arg1,0x28);
        F(P(P(arg0,0x80C0),0x16D0),0x4788) = F(arg1,0x2C);
        F(P(P(arg0,0x80C0),0x16D0),0x5E7C) = F(arg1,0xC);
        F(P(P(arg0,0x80C0),0x16D0),0x4874) = F(arg1,0x10);
        F(P(P(arg0,0x80C0),0x16D0),0x4E50) = F(arg1,0x1C);
        F(P(P(arg0,0x80C0),0x16D0),0x49C0) = F(arg1,0x20);
        F(P(P(arg0,0x80C0),0x16D0),0x4790) = F(arg1,0x24);
        result = I(arg1,4);
        break;
    case 0x1B5C:
        F(P(P(arg0,0x80C0),0x16D0),0x5EF4) = F(arg1,0x8);
        F(P(P(arg0,0x80C0),0x16D0),0x5EF8) = F(arg1,0xC);
        F(P(P(arg0,0x80C0),0x16D0),0x5EFC) = F(arg1,0x10);
        F(P(P(arg0,0x80C0),0x16D0),0x5EA4) = F(arg1,0x14);
        F(P(P(arg0,0x80C0),0x16D0),0x5EA8) = F(arg1,0x18);
        F(P(P(arg0,0x80C0),0x16D0),0x5EAC) = F(arg1,0x1C);
        F(P(P(arg0,0x80C0),0x16D0),0x5ECC) = F(arg1,0x20);
        F(P(P(arg0,0x80C0),0x16D0),0x5ED0) = F(arg1,0x24);
        F(P(P(arg0,0x80C0),0x16D0),0x5ED4) = F(arg1,0x28);
        F(P(P(arg0,0x80C0),0x16D0),0x5F1C) = F(arg1,0x2C);
        F(P(P(arg0,0x80C0),0x16D0),0x5F20) = F(arg1,0x30);
        F(P(P(arg0,0x80C0),0x16D0),0x5F24) = F(arg1,0x34);
        result = I(arg1,4);
        break;
    case 0x1B5D:
        F(P(P(arg0,0x80C0),0x16D0),0x4210) = F(arg1,0x8);
        F(P(arg0,0x80C0),0x2104) = F(arg1,0xC);
        F(P(arg0,0x80C0),0x2108) = F(arg1,0x10);
        F(P(arg0,0x80C0),0x210C) = F(arg1,0x14);
        F(P(arg0,0x80C0),0x2110) = F(arg1,0x18);
        F(P(arg0,0x80C0),0x2114) = F(arg1,0x1C);
        F(P(arg0,0x80C0),0x2118) = F(arg1,0x20);
        F(P(arg0,0x80C0),0x211C) = F(arg1,0x24);
        F(P(arg0,0x80C0),0x2120) = F(arg1,0x28);
        result = I(arg1,4);
        break;
    case 0x1B5F:
        input = arg1 + 8;
        for(i=0;i<13;i++) {
            row = func_001B3050(P(arg0,0x80C0)+0x16F0,i);
            n = i * 6 + 5;
            F(row,0) = ((float *)input)[n-5];
            F(row,8) = ((float *)input)[n-4];
            F(row,16) = ((float *)input)[n-3];
            F(row,4) = ((float *)input)[n-2];
            F(row,12) = ((float *)input)[n-1];
            F(row,20) = ((float *)input)[n-0];
        }
        result = I(arg1,4);
        break;
    case 0x1B60:
        j = 0;
        input2 = arg1 + 8;
        for(;j<13;j++) {
            row2 = func_001B3050(P(arg0,0x80C0)+0x16F0,j);
            n = j * 12;
            F(row2,24) = F(input2,j*48);
            F(row2,28) = *element(input2,n+1);
            F(row2,32) = *element(input2,n+2);
            F(row2,48) = *element(input2,n+3);
            F(row2,52) = *element(input2,n+4);
            F(row2,56) = *element(input2,n+5);
            F(row2,36) = *element(input2,n+6);
            F(row2,40) = *element(input2,n+7);
            F(row2,44) = *element(input2,n+8);
            F(row2,60) = *element(input2,n+9);
            F(row2,64) = *element(input2,n+10);
            F(row2,68) = *element(input2,n+11);
        }
        result = I(arg1,4);
        break;
    }
    return result;
}
