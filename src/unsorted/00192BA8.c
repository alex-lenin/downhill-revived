/* UNIT 0x00192BA8 0x00192D00 */
#include "model.h"
static __inline__ void set_type(int value, int *type)
{
    *type = value;
}

int func_00192BA8(int id)
{
    int type;
    set_type(0, &type);
    if (id < 0xFA0)
        goto done;
    if ((unsigned int)(id - 0xFA0) < 0x1A)
        goto done;

    if ((unsigned int)(id - 0xFBE) < 0x15) {
        set_type(1, &type);
    } else if ((unsigned int)(id - 0xFE1) < 0x15) {
        set_type(3, &type);
    } else if ((unsigned int)(id - 0xFFA) < 0x15) {
        set_type(4, &type);
    } else if (id == 0x1013) {
        set_type(5, &type);
    } else if ((unsigned int)(id - 0x1018) < 0xB) {
        set_type(6, &type);
    } else if ((unsigned int)(id - 0x1027) < 0xB) {
        set_type(7, &type);
    } else if (id == 0x1036) {
        set_type(8, &type);
    } else if ((unsigned int)(id - 0x103B) < 6) {
        set_type(9, &type);
    } else if ((unsigned int)(id - 0x104A) < 0x10) {
        set_type(10, &type);
    } else if ((unsigned int)(id - 0x105E) < 0x15) {
        set_type(11, &type);
    } else if ((unsigned int)(id - 0x1077) < 0x15) {
        set_type(12, &type);
    } else if ((unsigned int)(id - 0x1090) < 0x15) {
        set_type(13, &type);
    } else if ((unsigned int)(id - 0x10A9) < 0x15) {
        set_type(14, &type);
    } else if ((unsigned int)(id - 0x10C2) < 0x15) {
        set_type(15, &type);
    } else if (id == 0x10DB) {
        set_type(16, &type);
    } else {
        unsigned int diff;
        int limit;
        int two;
        diff = (unsigned int)(id - 0x10E0);
        set_type(20, &limit);
        two = 2;
        if (diff <= (unsigned int)limit) {
            type = two;
        }
    }

done:
    return type;
}
