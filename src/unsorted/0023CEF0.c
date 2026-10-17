/* UNIT 0x0023CEF0 0x0023CF10 */
/* Stores VU0 registers vf17..vf19 to VU0 data memory at qword 0x16. */
void func_0023CEF0(void)
{
    __asm__ volatile (
        "viaddi $vi7, $vi0, 0xF\n"
        "viaddi $vi7, $vi7, 0x7\n"
        "vsqi.xyzw $vf17, ($vi7++)\n"
        "vsqi.xyzw $vf18, ($vi7++)\n"
        "vsqi.xyzw $vf19, ($vi7++)\n"
        : : : "memory");
}
