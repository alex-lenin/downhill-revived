/* UNIT 0x00110EF8 0x00110F88 */
typedef struct Part { char pad0[0x6C]; float x6C; char pad70[0x5C]; int xCC; } Part;
typedef struct Obj { char *x0; char pad4[0x2B8]; float x2BC; char pad2C0[0x14]; Part x2D4; char pad3A4[0x9C]; unsigned long x440; } Obj;
extern float func_001E7C38(char *, int);
int func_00110EF8(Obj *obj, int index) {
    Part *part = &obj->x2D4;
    char *other;
    float time;
    int result = 0;
    if ((part->xCC >> 6) & 1) {
        other = obj->x0+0x5FF0;
        if (other) {
            time = part->x6C;
            if (time + func_001E7C38(other, index) < obj->x2BC) result = 1;
        }
    }
    obj->x440 |= (unsigned long)-1 << 63;
    return result;
}
