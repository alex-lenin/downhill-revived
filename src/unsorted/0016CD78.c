/* UNIT 0x0016CD78 0x0016CDE8 */
typedef struct { char pad[0x50]; char entries[10][0x6D0]; char pad2[0x44]; int count; } Obj;
void *func_0016CD78(Obj *obj, void *entry) {
    int i;
    void *next = 0;
    for (i = 0; i < obj->count; i++) {
        if (entry == obj->entries[i]) {
            if (i + 1 < obj->count) next = obj->entries[i + 1];
            break;
        }
    }
    return next;
}
