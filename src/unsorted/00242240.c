/* UNIT 0x00242240 0x00242270 */
int func_00242240(unsigned char *obj) {
    unsigned char *bytes = obj + 0x16;
    return obj[0x16] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
}
