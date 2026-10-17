/* UNIT 0x0012D3C8 0x0012D510 */
void func_0012D510(void *);                            /* extern */
void func_0012D660(void *, int, float);                    /* extern */
void func_0012D860(void *, void *, void *, int);         /* extern */
void func_0012E160(void *);                            /* extern */
void func_0012F5D0(void *);                            /* extern */
int func_00130010(void *);                          /* extern */
float func_001385A8(void *);                          /* extern */
void func_00156580(void *);                               /* extern */
void func_00161EB8(void *);                            /* extern */
void func_001ACFA8(void *);                               /* extern */
void func_001AD8D8(void *);                            /* extern */
void func_001AF0C0(void *);                            /* extern */
void func_001AF438(void *);                            /* extern */
void func_001AF5E8(void *);                            /* extern */
void func_001AF790(void *);                            /* extern */
void func_001AF978(void *);                            /* extern */
void func_001AFBA8(void *);                            /* extern */
void func_001AFED8(void *);                            /* extern */
void func_001B01D8(void *);                            /* extern */
void func_001B0AF0(void *);                            /* extern */
void func_001B6668(void *);                            /* extern */
void func_001C8380(void *);                            /* extern */
void func_001E74B8(void *);                               /* extern */
void func_00211280(int);                               /* extern */
float func_00220D98(int);                               /* extern */
extern int D_004B2828[];

void func_0012D3C8(void *arg0) {
    void *vector;
    float temp_f0;
    float temp_f20;

    func_001E74B8(arg0 + 0x5FF0);
    func_00156580(arg0 + 0x3FD0);
    func_001ACFA8(arg0 + 0x4580);
    if (((*(unsigned char **)((char *)arg0+0x3514))[5] != 0) || (func_00130010(arg0) != 0)) {
        func_001AF0C0(arg0 + 0x49AC);
        func_001AF5E8(arg0 + 0x49E0);
        func_001AF790(arg0 + 0x4AB4);
        func_001AF978(arg0 + 0x4AC0);
        func_001AFED8(arg0 + 0x4B38);
        func_001B01D8(arg0 + 0x4C2C);
        func_001AFBA8(arg0 + 0x4D58);
        func_001B0AF0(arg0 + 0x4E54);
        func_001AF438(arg0 + 0x49C8);
    }
    func_001AD8D8(arg0 + 0x48B0);
    vector=arg0+0x40C0;
    func_0012D860(arg0, arg0 + 0x7AB0, arg0 + 0x4110, 1);
    func_0012E160(arg0);
    func_001B6668(arg0);
    func_00161EB8(arg0);
    func_00211280(*(int *)((char *)arg0+0x792C));
    func_0012F5D0(arg0);
    func_0012D510(arg0);
    func_001C8380(arg0 + 0x7720);
    if (D_004B2828[0] == 1) {
        temp_f20 = func_00220D98(1);
        temp_f0 = func_001385A8(vector);
        func_0012D660(arg0, 0, temp_f20 * (temp_f0 * temp_f0));
    }
}
