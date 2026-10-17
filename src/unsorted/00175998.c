/* UNIT 0x00175998 0x001759E8 */
typedef struct Entry { char pad[0x10]; int x10; } Entry;
typedef struct Obj { char pad[0x1260]; Entry *x1260; Entry *x1264; Entry *x1268; Entry *x126C; Entry *x1270; Entry *x1274; } Obj;
void func_00175998(Obj *obj)
{
    if (obj->x1260) obj->x1260->x10 = 0;
    if (obj->x1264) obj->x1264->x10 = 0;
    if (obj->x1268) obj->x1268->x10 = 0;
    if (obj->x126C) obj->x126C->x10 = 0;
    if (obj->x1270) obj->x1270->x10 = 0;
    if (obj->x1274) obj->x1274->x10 = 0;
}
