/* UNIT 0x00234F38 0x00234FB0 */
struct CrcObj {
    char pad0[0x1F8];
    int table[256];
};

void func_00234F38(struct CrcObj *obj) {
    unsigned long value;
    int index, bit;

    for (index = 0; index < 256; index++) {
        value = index;
        for (bit = 0; bit < 8; bit++) {
            if ((value & 1) == 1) value = (value >> 1) ^ 0xEDB88320UL;
            else value >>= 1;
        }
        obj->table[index] = value;
    }
}
