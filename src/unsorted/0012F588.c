/* UNIT 0x0012F588 0x0012F5D0 */
extern char *func_00158618(void *);
extern void func_00254544(char *, char *);
void func_0012F588(char *dest, void *obj) {
    if (obj == 0) {
        *dest = 10;
        return;
    }
    func_00254544(dest, func_00158618(obj));
}
