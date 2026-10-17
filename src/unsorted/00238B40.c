/* UNIT 0x00238B40 0x00238BC8 */
extern int func_00238F10(void);
extern int D_0029E3E4, D_0029E3E8, D_0029E3EC;
extern float D_0029E3F8;
void func_00238B40(void) {
    float elapsed, previous;
    int now, delta;
    now = func_00238F10();
    delta = now - D_0029E3E8;
    D_0029E3EC = now;
    elapsed = (float)delta * 0.00010414584f;
    D_0029E3E4 = delta;
    previous = D_0029E3F8;
    if (!(previous <= elapsed)) {
        D_0029E3F8 = previous + (elapsed-previous)*0.05f;
    } else {
        D_0029E3F8 = elapsed;
    }
}
