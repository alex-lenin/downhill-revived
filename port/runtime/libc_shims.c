/* C library and gcc 2.95 runtime functions the game calls that the PC's C runtime doesn't export:
   the MSVC x86 CRT has fabsf/floorf only as inline functions in its headers. */

#include <stdarg.h>
#include <stdio.h>

/* Native callers use the host varargs ABI. Keep them out of the EE sprintf
   bridge, whose register slots cannot represent a host promoted double. */
int host_sprintf(char *out, const char *format, ...)
{
    va_list args;
    int result;
    va_start(args, format);
    result = vsprintf(out, format, args);
    va_end(args);
    return result;
}

float fabsf(float x)
{
    return __builtin_fabsf(x);
}

float floorf(float x)
{
    return __builtin_floorf(x);
}

/* exception::what() of gcc 2.95's libstdc++ (in the vtables of the game's exception classes) */
const char *what__C9exception(const void *self)
{
    (void)self;
    return "9exception";
}
