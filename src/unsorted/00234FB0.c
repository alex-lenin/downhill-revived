/* UNIT 0x00234FB0 0x00235010 */
typedef char *va_list;
#define va_start(ap,last) (ap = (char *)__builtin_next_arg(last) - (8 - __builtin_args_info(2)) * 8)
#define va_end(ap) ((void)0)
extern int func_00254DE8(char *, const char *, va_list);
extern int func_00254D08(void *, void *, char *);
extern char D_0029DBF8[], D_0029DC00[];
void func_00234FB0(int unused, const char *fmt, ...) {
    char buffer[1024];
    va_list ap;
    va_start(ap, fmt);
    func_00254DE8(buffer, fmt, ap);
    func_00254D08(D_0029DBF8, D_0029DC00, buffer);
    va_end(ap);
}
