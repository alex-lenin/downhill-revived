/* UNIT 0x00182580 0x001825B0 */
extern unsigned int *D_0029E4BC;
void func_00182580(unsigned int *value) {
    *D_0029E4BC = (*D_0029E4BC & 0xFFFC007F) | ((*value & 0x7FF) << 7);
}
