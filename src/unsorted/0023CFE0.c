/* UNIT 0x0023CFE0 0x0023D020 */
typedef struct { float x0, x4, x8; } Vec;
typedef struct { char pad0[0x10]; float x10, x14, x18; } Obj;
typedef struct { Obj *x0; char pad4[0x10]; } Entry;
extern Entry D_0075A9D0[];
void func_0023CFE0(Vec *pos, int index) {
    Entry *entry = &D_0075A9D0[index];
    entry->x0->x10 = pos->x0;
    entry->x0->x14 = pos->x4;
    entry->x0->x18 = pos->x8;
}
