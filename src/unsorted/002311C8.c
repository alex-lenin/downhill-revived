/* UNIT 0x002311C8 0x00231230 */
struct Params {
    int x0, x4, x8, xC, x10, x14;
};
extern int func_0025A6E0(struct Params *);
extern void func_0025A460(int, void (*)(void), int);
extern void func_002311A0(void);
extern void func_0025A720(int);
extern void func_0025A6F0(int);
void func_002311C8(unsigned short id) {
    struct Params params;
    int handle;
    params.x4 = 1;
    params.x8 = 0;
    params.x14 = 0;
    handle = func_0025A6E0(&params);
    func_0025A460(id, func_002311A0, handle);
    func_0025A720(handle);
    func_0025A6F0(handle);
}
