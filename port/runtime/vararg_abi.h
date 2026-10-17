/* Native formatted varargs -> EE argument slots. The EE uses one GPR for
 * a double/DI/TI value; the host vararg stack uses 8/8/16 bytes respectively. */
#ifndef DHD_VARARG_ABI_H
#define DHD_VARARG_ABI_H
#include <stdarg.h>
#include "quad_abi.h"
#include "recomp.h"

typedef struct ps2_vararg_slots { unsigned reg, stack; } ps2_vararg_slots;
static inline void ps2_vararg_word(ps2_vararg_slots *slots, uint64_t bits) {
    if (slots->reg <= 11) GPR(slots->reg++) = bits;
    else { M64(ADDR(29, slots->stack)) = bits; slots->stack += 8; }
}
static inline void ps2_vararg_quad(ps2_vararg_slots *slots, ps2_u128 value) {
    if (slots->reg <= 11) ps2.gpr[slots->reg++] = ps2_scalar_quad(value);
    else {
        slots->stack = (slots->stack + 15) & ~15u;
        M128(ADDR(29, slots->stack)) = ps2_scalar_quad(value);
        slots->stack += 16;
    }
}
static inline void ps2_format_varargs(const char *format, va_list *ap,
                                     unsigned first_reg, unsigned stack) {
    ps2_vararg_slots slots = {first_reg, stack};
    if (!format) return;
    while (*format) {
        int length = 0;
        if (*format++ != '%') continue;
        if (*format == '%') { format++; continue; }
        while (*format && (*format == '-' || *format == '+' || *format == ' ' ||
                           *format == '#' || *format == '0')) format++;
        if (*format == '*') { ps2_vararg_word(&slots, S32(va_arg(*ap, uint32_t))); format++; }
        else while (*format >= '0' && *format <= '9') format++;
        if (*format == '.') {
            format++;
            if (*format == '*') { ps2_vararg_word(&slots, S32(va_arg(*ap, uint32_t))); format++; }
            else while (*format >= '0' && *format <= '9') format++;
        }
        if (*format == 'h') { format++; if (*format == 'h') format++; }
        else if (*format == 'l') { length=1; format++; if (*format == 'l') { length=2; format++; } }
        else if (*format == 'q') { length=2; format++; }
        else if (*format == 'L') { length=3; format++; }
        else if (*format == 'j') { length=1; format++; }
        else if (*format == 'z' || *format == 't') format++;
        if (!*format) break;
        switch (*format++) {
        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A':
            ps2_vararg_word(&slots, ps2_d2u(va_arg(*ap, double))); break;
        case 'd': case 'i': case 'u': case 'o': case 'x': case 'X':
            if (length == 2) {
                /* clang i386 aligns a TI vararg to 16 bytes at the call site;
                 * va_arg(mode(TI)) itself advances without that padding. */
                *ap = (va_list)(((uintptr_t)*ap + 15u) & ~(uintptr_t)15u);
                ps2_vararg_quad(&slots, va_arg(*ap, ps2_u128));
            }
            else if (length == 1) ps2_vararg_word(&slots, va_arg(*ap, uint64_t));
            else ps2_vararg_word(&slots, S32(va_arg(*ap, uint32_t)));
            break;
        case 's': case 'p': case 'n':
            ps2_vararg_word(&slots, (uintptr_t)va_arg(*ap, void *)); break;
        case 'c': ps2_vararg_word(&slots, S32(va_arg(*ap, uint32_t))); break;
        default: break;
        }
    }
}
#endif
