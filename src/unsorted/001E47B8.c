/* UNIT 0x001E47B8 0x001E47F0 */
typedef struct Obj { char pad[0x51C]; char x51C; char x51D; char x51E; } Obj;
void func_001E47B8(Obj *obj, int index) {
    if (index == 0) obj->x51C = 1;
    else if (index == 1) obj->x51D = 1;
    else if (index == 2) obj->x51E = 1;
}
