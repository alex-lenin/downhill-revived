/* UNIT 0x00128F50 0x00129058 */
struct Item {
    char pad[0x17C];
    int active;
    char tail[0x20];
    void SetActive(int on) { active = on; }
};
struct Result { char pad[8]; float x8; };
struct Group {
    Item items[30];
    unsigned int **defs[30];      /* 0x30C0 */
    int x3138[30];
    int x31B0;
    char pad31B4[0x31C8 - 0x31B4];
    float x31C8;
    int x31CC[30];
    int count;          /* 0x3244 */
    int x3248;
    int reset;          /* 0x324C */
    int x3250;
    int x3254;
    void func_00128F50();
};

extern float D_0029C248[];
extern char D_003041B0[];
extern int func_00122258(void *obj, int id);
extern Result *func_00129058(Group *group, int id);
extern void func_00129500(Item *item, Group *group, unsigned int **def, int a, int b, Result *result);

void Group::func_00128F50()
{
    int i;
    Result *result;
    Item *item;

    x31C8 = D_0029C248[0];
    x3250 = -1;
    x31B0 = 0;
    x3254 = 0;
    reset = 0;
    for (i = 0; i < count; i++) {
        if (defs[i]) {
            result = func_00129058(this, func_00122258(D_003041B0, **defs[i] >> 18));
        } else {
            result = func_00129058(this, 0);
        }
        if (result) {
            x31C8 = result->x8;
            item = &items[i];
            func_00129500(item, this, defs[i], x3138[i], x31CC[i], result);
            item->SetActive(1);
        }
    }
}
