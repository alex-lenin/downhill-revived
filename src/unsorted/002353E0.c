/* UNIT 0x002353E0 0x00235418 */
typedef struct Obj {
    int x0;
    char pad4[0x3908];
    int x390C;
    int x3910;
    char x3914;
    char pad3915[3];
    int x3918;
    int x391C;
    int x3920;
    char x3924;
} Obj;
Obj *func_002353E0(Obj *obj) {
    obj->x0 = 1;
    obj->x391C = 2;
    obj->x3920 = -1;
    obj->x390C = 0;
    obj->x3910 = 0;
    obj->x3918 = 0;
    obj->x3914 = 0;
    obj->x3924 = 0;
    return obj;
}
