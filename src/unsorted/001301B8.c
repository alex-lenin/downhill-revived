/* UNIT 0x001301B8 0x001301E0 */
typedef struct Obj {
    char pad0[0x7AC8];
    float x7AC8;
    float x7ACC;
    float x7AD0;
    float x7AD4;
    float x7AD8;
} Obj;
float func_001301B8(Obj *obj) {
    return obj->x7ACC + obj->x7AD0 + obj->x7AC8 + obj->x7AD8 + obj->x7AD4;
}
