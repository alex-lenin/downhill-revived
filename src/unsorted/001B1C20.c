/* UNIT 0x001B1C20 0x001B1DD8 */
typedef float Vec[4];
typedef struct Obj { char pad[0x440]; int x440; } Obj;
extern int func_001B1C08(int);
extern Vec D_007684B0[],D_0076E630[];
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)
	sq $2,%0" : "=m"(dst) : "r"(src) : "$2")
void func_001B1C20(Obj *obj, Vec *out) {
    int index=func_001B1C08(obj->x440);
    COPY_VECTOR(out[1],D_007684B0+0+index);
    COPY_VECTOR(out[2],D_0076E630+0+index);
    COPY_VECTOR(out[6],D_007684B0+22+index);
    COPY_VECTOR(out[7],D_0076E630+22+index);
    COPY_VECTOR(out[11],D_007684B0+18+index);
    COPY_VECTOR(out[12],D_0076E630+18+index);
    COPY_VECTOR(out[16],D_007684B0+3+index);
    COPY_VECTOR(out[17],D_0076E630+3+index);
    COPY_VECTOR(out[21],D_007684B0+23+index);
    COPY_VECTOR(out[22],D_0076E630+23+index);
    COPY_VECTOR(out[26],D_007684B0+19+index);
    COPY_VECTOR(out[27],D_0076E630+19+index);
    COPY_VECTOR(out[31],D_007684B0+6+index);
    COPY_VECTOR(out[32],D_0076E630+6+index);
    COPY_VECTOR(out[36],D_007684B0+24+index);
    COPY_VECTOR(out[37],D_0076E630+24+index);
    COPY_VECTOR(out[41],D_007684B0+20+index);
    COPY_VECTOR(out[42],D_0076E630+20+index);
    COPY_VECTOR(out[46],D_007684B0+9+index);
    COPY_VECTOR(out[47],D_0076E630+9+index);
    COPY_VECTOR(out[51],D_007684B0+25+index);
    COPY_VECTOR(out[52],D_0076E630+25+index);
    COPY_VECTOR(out[56],D_007684B0+21+index);
    COPY_VECTOR(out[57],D_0076E630+21+index);
}
