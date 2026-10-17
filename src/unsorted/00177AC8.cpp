/* UNIT 0x00177AC8 0x00177BD8 */
class Obj {
public:
    char pad0[0xF20];
    char xF20[0x28];
    char xF48[0x1168 - 0xF48];
    int x1168;
    char pad116C[0x1290 - 0x116C];
    int x1290;
    int x1294;
    char pad1298[0x130C - 0x1298];
    int x130C;
    char pad1310[0x1344 - 0x1310];
    char x1344[3];
    char x1347[3];
    char x134A[5];
    char x134F[5];
    unsigned char x1354;
    unsigned char x1355;

    void func_00177AC8();
    void func_00177DA0();
    void func_00177BD8();
    void func_00177F80();
    void func_00177F08();
    void func_00177D28();
    void func_00178090(char *, char *, char *, int);
    void func_00178100(char *, char *, char *, int);
    void func_001781E0();
};

void Obj::func_00177AC8()
{
    int count;

    if (x1168 == 0) {
        return;
    }
    count = --x1290;
    if (count < 0) {
        this->func_00177DA0();
        if (x130C != 0) {
            func_00177BD8();
        }
        func_00177F80();
        x1290 = x1294;
    } else if (x1354 < count) {
        func_00178090(x1344, x1347, xF20, 2);
        func_00178090(x134A, x134F, xF48, 4);
    } else if (count == x1354) {
        this->func_00177F08();
        this->func_00177D28();
    } else if (x1355 < count) {
        func_00178100(x1344, x1347, xF20, 2);
        func_00178100(x134A, x134F, xF48, 4);
    }
    this->func_001781E0();
}
