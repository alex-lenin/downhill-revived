/* UNIT 0x0011C488 0x0011C540 */
typedef struct Body { char pad0[0x18]; float x18; } Body;
typedef struct Rider {
    char pad0[0x792C];
    Body *x792C;
    char pad7930[0x3C];
    int x796C;
    char pad7970[0xEC];
    int x7A5C;
    char pad7A60[0x170];
} Rider;
typedef struct Obj { char pad0[0x501A8]; int x501A8; } Obj;
typedef struct Vec { float x0; float x4; float x8; } Vec;

int func_0011C488(Obj *obj, Vec *pos)
{
    int i;
    int result = 0;

    for (i = 0; i < obj->x501A8; i++) {
        Rider *rider = &((Rider *)obj)[i];
        if (rider->x7A5C == 3) {
            int select = 0;
            int value = rider->x796C;
            if (value == 0xE0 || value == 0x100 || value == 0x140 || value == 0x180 || value == 0x1A0) select = 1;
            if (select && pos->x8 < rider->x792C->x18) {
                result = 1;
                break;
            }
        }
    }
    return result;
}
