/* UNIT 0x0015A4F0 0x0015A608 .rodata=0x28A310:0x28A340 */
typedef struct Anim {
    char pad0[0x10];
    char x10[1];
} Anim;

typedef struct Item {
    char pad0[0x4];
    int state;
    Anim *anim;
    char padC[0xDC - 0xC];
    int xDC;
    char padE0[0x1E0 - 0xE0];
    unsigned short timer;
    char pad1E2[0x1F0 - 0x1E2];
    char x1F0[1];
} Item;

extern int func_00238F60(void);
extern void func_0015A608(Item *, int);
extern void func_00159CE0(Item *, int);
extern void func_0015A7C0(Item *);
extern void func_0015AB08(Item *);
extern void func_0015AB50(Item *);
extern void func_0015AC68(Item *);
extern void func_0015AD60(Item *);
extern void func_0015AEE8(Item *);
extern void func_0015B100(Item *);
extern void func_0015B1A0(Item *);
extern void func_001CFB60(void *, void *, int, int);

void func_0015A4F0(Item *item) {
    item->timer += func_00238F60();
    switch (item->state) {
    case 0:
        if (item->anim != 0) {
            func_0015A608(item, item->xDC);
            func_00159CE0(item, 1);
        }
        break;
    case 1:
        func_0015A7C0(item);
        break;
    case 2:
        func_0015AB08(item);
        break;
    case 3:
        func_0015AB50(item);
        break;
    case 4:
        func_0015AC68(item);
        break;
    case 5:
        func_0015AD60(item);
        break;
    case 6:
    case 7:
        func_0015AEE8(item);
        break;
    case 8:
        func_0015B100(item);
        break;
    case 9:
        func_0015B1A0(item);
        break;
    case 10:
        break;
    }
    func_001CFB60(item->x1F0, item->anim->x10, item->state, 8);
}
