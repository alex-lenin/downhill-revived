/* UNIT 0x00190900 0x00190990 */
struct Entry { int flags; int x4, x8; };
struct Obj { struct Entry entries[14]; int xA8; char xAC; char padAD[3]; int xB0; };
struct Obj *func_00190900(struct Obj *obj) {
    obj->entries[0].flags=0x10;
    obj->entries[1].flags=0x40;
    obj->entries[2].flags=0x80;
    obj->entries[3].flags=0x20;
    obj->entries[4].flags=0x4000;
    obj->entries[5].flags=0x1000;
    obj->entries[6].flags=0x2000;
    obj->entries[7].flags=0x8000;
    obj->entries[8].flags=8;
    obj->entries[9].flags=1;
    obj->entries[10].flags=0x800;
    obj->entries[11].flags=0x200;
    obj->entries[12].flags=0x400;
    obj->entries[13].flags=0x100;
    obj->xB0=0;
    obj->xAC=0;
    return obj;
}
