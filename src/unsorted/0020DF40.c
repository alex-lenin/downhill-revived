/* UNIT 0x0020DF40 0x0020E018 .sdata=0x29DA18:0x29DA20 */
extern char D_00298140[];
extern void func_0025C7D0(int);
extern void func_00247760(int);
extern int func_0025ED48(char *);
extern int func_0025ED10(void);
extern void func_00247C28(int);
extern void func_0025DA28(void);
extern int func_00247A40(int);
extern void func_0020E018(void);

char D_0029DA18 = 0;

void func_0020DF40(void) {
    func_0025C7D0(0);
    func_00247760(0);
    while (func_0025ED48(D_00298140) == 0) {
    }
    while (func_0025ED10() == 0) {
    }
    func_0025C7D0(0);
    func_00247760(0);
    func_00247C28(2);
    func_0025DA28();
    while (func_00247A40(0) != 2) {
    }
    func_0020E018();
    D_0029DA18 = 1;
    while (func_00247A40(0) != 2) {
    }
}
