/* UNIT 0x00232BF8 0x00232C98 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Matrix {
    u128 row[4];
} Matrix;

typedef struct State {
    u128 x0;
    Matrix x10;
    u128 x50;
    u128 x60;
    u128 x70;
    Matrix x80;
    u128 xC0;
    u128 xD0;
} State;

typedef struct Obj {
    char pad0[0x34];
    u128 *x34;
    Matrix *x38;
    char pad3C[0x50 - 0x3C];
    u128 x50;
    u128 x60;
    u128 x70;
    Matrix x80;
    u128 xC0;
    u128 xD0;
} Obj;

/* vector/matrix copies are libvu0-style lq/sq inline asm */
void func_00232BF8(Obj *obj, State *state)
{
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(*obj->x34) : "r"(&state->x0) : "$2");
    __asm__ volatile (
        "lq $8, 0x0(%4)
	"
        "lq $9, 0x10(%4)
	"
        "lq $10, 0x20(%4)
	"
        "lq $11, 0x30(%4)
	"
        "sq $8, %0
	"
        "sq $9, %1
	"
        "sq $10, %2
	"
        "sq $11, %3"
        : "=m"(obj->x38->row[0]), "=m"(obj->x38->row[1]), "=m"(obj->x38->row[2]), "=m"(obj->x38->row[3])
        : "r"(&state->x10)
        : "$8", "$9", "$10", "$11");
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(obj->x50) : "r"(&state->x50) : "$2");
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(obj->x60) : "r"(&state->x60) : "$2");
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(obj->x70) : "r"(&state->x70) : "$2");
    __asm__ volatile (
        "lq $8, 0x0(%4)\n\t"
        "lq $9, 0x10(%4)\n\t"
        "lq $10, 0x20(%4)\n\t"
        "lq $11, 0x30(%4)\n\t"
        "sq $8, %0\n\t"
        "sq $9, %1\n\t"
        "sq $10, %2\n\t"
        "sq $11, %3"
        : "=m"(obj->x80.row[0]), "=m"(obj->x80.row[1]), "=m"(obj->x80.row[2]), "=m"(obj->x80.row[3])
        : "r"(&state->x80)
        : "$8", "$9", "$10", "$11");
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(obj->xC0) : "r"(&state->xC0) : "$2");
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(obj->xD0) : "r"(&state->xD0) : "$2");
}
