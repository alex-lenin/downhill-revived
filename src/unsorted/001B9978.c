/* UNIT 0x001B9978 0x001B99B8 */
typedef struct { int x0, x4, x8; } Entry;
typedef struct { char pad0[0x10C]; int x10C; char pad110[0xD8]; Entry x1E8[4]; } Obj;
void func_001B9978(Obj *obj, int value, int index, int arg3, int arg4) {
    Entry *entry = &obj->x1E8[index];
    entry->x0 = (value >> 5) - 1;
    entry->x8 = arg4;
    if (obj->x10C == 10) { entry->x4 = 0; return; }
    entry->x4 = arg3;
}
