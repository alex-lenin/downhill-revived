/* UNIT 0x0023CF10 0x0023CF30 */
void func_0023CF10(void)
{
    __asm__ volatile (
        "viaddi $vi7, $vi0, 0xF\n"
        "viaddi $vi7, $vi7, 0xA\n"
        "vsqi.xyzw $vf17, ($vi7++)\n"
        "vsqi.xyzw $vf18, ($vi7++)\n"
        "vsqi.xyzw $vf19, ($vi7++)\n");
}
