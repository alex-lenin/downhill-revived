/* UNIT 0x00182920 0x00182990 */
extern void func_001831A0(void *);
extern void func_0021F788(void);
extern void func_0021F820(void);
extern void func_002215D8(float);
extern void func_00243CE8(int);
void func_00182920(void *obj) {
    int count = 5;
    func_0021F788();
    func_002215D8(1.0f);
    do {
        func_00243CE8(0);
        count--;
    } while (count >= 0);
    func_001831A0(obj);
    func_0021F820();
}
