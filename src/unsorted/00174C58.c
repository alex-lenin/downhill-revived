/* UNIT 0x00174C58 0x00174D48 */
struct Rider {
    char pad0[0x6610];
    char x6610[0x7978 - 0x6610];
    int x7978;
    char pad797C[0xE0];
    int x7A5C;
};

struct Obj {
    struct Rider *x0;
    char pad4[0x24];
    unsigned char x28;
    unsigned char x29;
};

extern int func_002205A0(int id);
extern int func_00220230(int handle);
extern int func_002204A8(int handle);
extern int func_001E6CC8(void *map, int code);

void func_00174C58(struct Obj *obj)
{
    struct Rider *rider = obj->x0;
    int enabled = rider->x7A5C;
    int handle;
    int buttons;
    unsigned short code;
    unsigned char *second;

    if (enabled == 1) {
        handle = func_002205A0(rider->x7978);
        buttons = func_00220230(handle);
        code = func_002204A8(handle);
        if (code != 0) {
            buttons = func_001E6CC8(rider->x6610, code);
        }
        second = &obj->x29;
        if (obj->x28 == 0 && *second == 0) {
            if (buttons & 0x10) {
                obj->x28 = enabled;
            } else if (buttons & 0x40) {
                *second = enabled;
            }
        } else if (*second == 0) {
            if (buttons & 0x40) {
                *second = enabled;
            }
        } else if (obj->x28 == 0 && (buttons & 0x10)) {
            obj->x28 = 1;
        }
    }
}
