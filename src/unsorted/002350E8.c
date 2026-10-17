/* UNIT 0x002350E8 0x00235138 */
typedef char *va_list;
#define va_start(ap,last) (ap = (char *)__builtin_next_arg(last) - (8 - __builtin_args_info(2)) * 8)
#define va_end(ap) ((void)0)
extern int func_00254DE8();
void func_002350E8(unsigned char *obj, const char *format, ...) {
 char buffer[1024];
 va_list args;
 if (obj[0x5F8]) {
 va_start(args, format);
 func_00254DE8(buffer, format, args);
 }
 va_end(args);
}
