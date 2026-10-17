/* UNIT 0x00232B60 0x00232BF8 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Src {
    char pad0[0x34];
    u128 *x34;
    u128 *x38;
} Src;

/* quadword copy, as the game's vector-copy macro (sceVu0CopyVector style) */
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* 64-byte matrix copy (as sceVu0CopyMatrix) */
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ( \
        "lq $8, 0x0(%4)\n\t" \
        "lq $9, 0x10(%4)\n\t" \
        "lq $10, 0x20(%4)\n\t" \
        "lq $11, 0x30(%4)\n\t" \
        "sq $8, %0\n\t" \
        "sq $9, %1\n\t" \
        "sq $10, %2\n\t" \
        "sq $11, %3" \
        : "=m"(*(u128 *)(dst + 0x0)), "=m"(*(u128 *)(dst + 0x10)), \
          "=m"(*(u128 *)(dst + 0x20)), "=m"(*(u128 *)(dst + 0x30)) \
        : "r"(src) : "$8", "$9", "$10", "$11")

void func_00232B60(Src *a0, char *a1) {
    COPY_VECTOR(*(u128 *)(a1 + 0x00), a0->x34);
    COPY_MATRIX(a1 + 0x10, a0->x38);
    COPY_VECTOR(*(u128 *)(a1 + 0x50), (u128 *)((char *)a0 + 0x50));
    COPY_VECTOR(*(u128 *)(a1 + 0x60), (u128 *)((char *)a0 + 0x60));
    COPY_VECTOR(*(u128 *)(a1 + 0x70), (u128 *)((char *)a0 + 0x70));
    COPY_MATRIX(a1 + 0x80, (u128 *)((char *)a0 + 0x80));
    COPY_VECTOR(*(u128 *)(a1 + 0xC0), (u128 *)((char *)a0 + 0xC0));
    COPY_VECTOR(*(u128 *)(a1 + 0xD0), (u128 *)((char *)a0 + 0xD0));
}
