/* UNIT 0x00134E40 0x00134E88 */
struct Vector { float x, y, z, w; };
struct Obj { char pad0[8]; float x8; char padC[0x64]; float x70; unsigned char x74; unsigned char x75; };
extern struct Vector D_0026DDA0[], D_0026DDE0[];
float func_00134E40(struct Obj *obj) {
    int offset = obj->x75 * 16;
    return ((obj->x8 - ((struct Vector *)(offset + (int)D_0026DDA0))->z) - obj->x70) / ((struct Vector *)(offset + (int)D_0026DDE0))->z;
}
