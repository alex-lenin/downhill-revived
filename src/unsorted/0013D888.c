/* UNIT 0x0013D888 0x0013D908 */
typedef struct { int x0, x4, x8; } Entry;
typedef struct { int x0; char pad[0x570]; Entry *x574; unsigned char x578[0x64]; int x5DC; } Obj;
void func_0013D888(Obj *obj, Entry *key) {
    int i;
    for (i = 0; i < obj->x5DC; i++) {
        Entry *entry = &obj->x574[obj->x578[i]];
        if (entry->x0 == key->x0 && entry->x4 == key->x4 && entry->x8 == key->x8) {
            obj->x0 = i;
            return;
        }
    }
    obj->x0 = 0;
}
