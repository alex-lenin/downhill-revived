/* UNIT 0x0017C6B0 0x0017C7A0 */
extern int func_0017C830(void *, int);
extern void func_0017C8A8(void *, void *, int);
extern int func_00215B58(int);
extern int func_00254D68(char *, char *, int);
extern char D_0029C9C8[];
void func_0017C6B0(char *obj, int arg1, int arg2, int value) {
    char text[16];
    unsigned int digit;
    int result;
    if (value < 0) digit = 10;
    else {
        func_00254D68(text, D_0029C9C8, value);
        digit = text[0] - '0';
        if (digit < 10) {} else digit = 0;
    }
    result = func_0017C830(obj, func_00215B58(arg1));
    func_0017C8A8(obj, obj + (digit * 16 + 0xD34), result);
    if (value >= 0) {
        digit = text[1] - '0';
        if (digit >= 10) digit = 0;
    }
    result = func_0017C830(obj, func_00215B58(arg2));
    func_0017C8A8(obj, obj + (digit * 16 + 0xD34), result);
}
