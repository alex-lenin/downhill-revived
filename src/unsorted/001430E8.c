/* UNIT 0x001430E8 0x00143250 */
typedef struct { char pad[0x16C]; int x16C; } Entry;
typedef struct { int x0, x4; char pad[0x60]; float x68, x6C; Entry entries[10]; } Obj;
extern int func_00141D60(Entry *, unsigned int *);
extern float D_0029DD3C, D_0029DD40, D_0029DD44, D_0029DD48;
void func_001430E8(Obj *obj, unsigned int *data) {
    Entry *entry = 0;
    if (obj->x4 < 10 && func_00141D60(&obj->entries[obj->x4], data)) {
        obj->x0 = 2;
        entry = &obj->entries[obj->x4++];
    }
    if (obj->x4 == 1) {
        if (entry->x16C == 0) {
            obj->x68 = 100.0f;
            obj->x6C = 300.0f;
            D_0029DD44 = 45.0f;
            D_0029DD3C = 0.0078125f;
            D_0029DD40 = 0.0018f;
            D_0029DD48 = 6.0f;
        } else {
            obj->x68 = 75.0f;
            obj->x6C = 150.0f;
            D_0029DD44 = 70.0f;
            D_0029DD3C = 0.012f;
            D_0029DD40 = 0.002f;
            D_0029DD48 = 10.0f;
            if (entry->x16C == 2) D_0029DD44 = 35.0f;
        }
    }
}
