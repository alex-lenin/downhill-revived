/* UNIT 0x00115758 0x00115D78 */
typedef struct Rider { char pad0[0x796C]; int x796C; } Rider;
typedef struct Obj {
    char pad0[0x58];
    Rider *x58;
    char pad5C[0xC4 - 0x5C];
    int xC4;
    char padC8[0xCC - 0xC8];
    unsigned int padCC:5;
    unsigned int b5:1;
    unsigned int b6:1;
    unsigned int b7:1;
    unsigned int b8:1;
    unsigned int b9:1;
    unsigned int b10:1;
    unsigned int restCC:21;
} Obj;
typedef struct Global {
    char pad0[0x5BB4];
    int x5BB4;
    char pad5BB8[0x24];
    int x5BDC;
} Global;
extern Global D_00398C40;

void func_00115758(Obj *obj)
{
    int a2;
    int flags;
    int key;

    if (D_00398C40.x5BB4 == 0)
        a2 = D_00398C40.x5BDC / 2;
    else if (D_00398C40.x5BB4 == 2)
        a2 = D_00398C40.x5BDC / 2 + 50;
    else
        a2 = D_00398C40.x5BDC / 2 + 25;
    obj->b5 = 0;
    obj->b6 = 0;
    obj->b7 = 0;
    obj->b8 = 0;
    obj->b9 = 0;
    obj->b10 = 0;
    obj->xC4 = 0;
    key = obj->x58->x796C;
    if (key == 0x100)
        goto cat100;
    if (key < 0x101) {
        if (key == 0x80)
            goto cat80;
        if (key < 0x81) {
            if (key == 0x40)
                goto cat40;
            if (key < 0x41) {
                if (key == 0x20)
                    goto cat20;
                else
                    goto fallback;
            } else {
                if (key == 0x60)
                    goto cat60;
                else
                    goto fallback;
            }
        } else {
            if (key == 0xC0)
                goto catC0;
            if (key < 0xC1) {
                if (key == 0xA0)
                    goto catA0;
                else
                    goto fallback;
            } else {
                if (key == 0xE0)
                    goto catE0;
                else
                    goto fallback;
            }
        }
    } else {
        if (key == 0x180)
            goto cat180;
        if (key < 0x181) {
            if (key == 0x140)
                goto cat140;
            if (key < 0x141) {
                if (key == 0x120)
                    goto cat120;
                else
                    goto fallback;
            } else {
                if (key == 0x160)
                    goto cat160;
                else
                    goto fallback;
            }
        } else {
            if (key == 0x1C0)
                goto fallback;
            if (key < 0x1C1) {
                if (key == 0x1A0)
                    goto cat1A0;
                else
                    goto fallback;
            } else
                goto fallback;
        }
    }
cat20:
cat60:
cat120:
    obj->b5 = a2 >= 0x10;
    obj->b8 = a2 >= 0x1A;
    obj->b9 = a2 >= 0x2E;
    obj->b6 = a2 >= 0x38;
    obj->b7 = a2 >= 0x3D;
    obj->b10 = a2 >= 0x42;
    obj->xC4 = 0x28;
    return;
cat40:
    obj->b5 = a2 >= 6;
    obj->b8 = a2 >= 0xB;
    obj->b9 = a2 >= 0x10;
    obj->b6 = a2 >= 0x12;
    obj->b7 = a2 >= 0x15;
    obj->b10 = a2 >= 0x15;
    obj->xC4 = 0xA;
    return;
cat80:
    obj->b5 = a2 >= 0xB;
    obj->b8 = a2 >= 0x15;
    obj->b9 = a2 >= 0x38;
    obj->b6 = a2 >= 0x3D;
    obj->b7 = a2 >= 0x3D;
    obj->b10 = a2 >= 0x42;
    obj->xC4 = 0x32;
    return;
catA0:
cat160:
    obj->b5 = a2 >= 0xB;
    obj->b8 = a2 >= 0x47;
    obj->b9 = a2 >= 0x4C;
    obj->b6 = a2 >= 0x4F;
    obj->b7 = a2 >= 0x3D;
    obj->b10 = a2 >= 0x51;
    obj->xC4 = 0x50;
    return;
catC0:
    obj->b5 = a2 >= 0xB;
    obj->b8 = a2 >= 0x15;
    obj->b9 = a2 >= 0x1F;
    obj->b6 = a2 >= 0x24;
    obj->b7 = a2 >= 0x25;
    obj->b10 = a2 >= 0x29;
    obj->xC4 = 0x28;
    return;
catE0:
cat140:
cat1A0:
    obj->b5 = a2 >= 0x10;
    obj->b8 = a2 >= 0x1A;
    obj->b9 = a2 >= 0x24;
    obj->b6 = a2 >= 0x29;
    obj->b7 = a2 >= 0x2B;
    obj->b10 = a2 >= 0x33;
    obj->xC4 = 0xA;
    return;
cat100:
cat180:
    obj->b5 = a2 >= 0x10;
    obj->b8 = a2 >= 0x1A;
    obj->b9 = a2 >= 0x2E;
    obj->b6 = a2 >= 0x38;
    obj->b7 = a2 >= 0x3D;
    obj->b10 = a2 >= 0x42;
    obj->xC4 = 0x1E;
    return;
fallback:
    obj->b5 = a2 >= 0x33;
    obj->b8 = a2 >= 0x51;
    obj->b9 = a2 >= 0x56;
    obj->b6 = a2 >= 0x59;
    obj->b7 = a2 >= 0x5B;
    obj->b10 = a2 >= 0x60;
    obj->xC4 = 0x14;
    return;
}
