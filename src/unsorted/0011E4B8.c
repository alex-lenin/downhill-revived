/* UNIT 0x0011E4B8 0x0011E528 .rodata=0x288C50:0x288C70 */
typedef struct { int x0; int x4; } Obj;
float func_0011E4B8(Obj *obj) {
    switch (obj->x4) {
    case 2: case 7: return 0.1f;
    case 1: case 3: case 4: return 1.0f;
    case 5: return 0.75f;
    default: return 1.0f;
    }
}
