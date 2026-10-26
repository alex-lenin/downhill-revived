/* Native diagnostics used by the host device threads. These cannot call back
   into the module's varargs logger or its emulated register snapshot. */
#include <stdarg.h>
#include <stdio.h>
int hle_verbose;
void hle_log(const char *format,...) {
    va_list args; va_start(args,format); vfprintf(stderr,format,args); va_end(args);
}
