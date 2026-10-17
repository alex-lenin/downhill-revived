/* UNIT 0x0020DB78 0x0020DC28 */
extern int func_0025DA60(void *, int);
extern void func_0020D028(char *, ...);
extern void func_0025E300(int, void *, int);
extern void func_0025DCE8(int);
extern char D_002980A8[], D_002980E8[];
int func_0020DB78(void *name, void *buffer, int size) {
    int fd = func_0025DA60(name, 0x602);
    if (fd < 0) {
        if (fd == (int)0xFFFF0001) {
            func_0020D028(D_002980A8, name);
            return -2;
        }
        func_0020D028(D_002980E8, fd, name);
        return -2;
    }
    func_0025E300(fd, buffer, size);
    func_0025DCE8(fd);
    return 0;
}
