/* UNIT 0x001937E0 0x00193AB8 */
#include "animation.h"
struct Group {
    int x0;
    int x4;
    char pad8[8];
    float x10, x14, x18, x1C, x20, x24, x28;
    char pad2C[0xA0 - 0x2C];
    short xA0[24];
    char padD0[0xF0 - 0xD0];
};

struct Groups {
    int count;
    char pad4[12];
    struct Group group[100];
};

struct Obj {
    char pad0[0x5C];
    int x5C;
    char pad60[0x10088 - 0x60];
    struct Groups *x10088;
    unsigned char x1008C;
};

struct Item {
    char pad0[0x14];
    unsigned short x14;
};

struct Context {
    char pad0[0x1B8];
    char *x1B8;
};

extern char D_002839D8[];
extern char D_0028D850[];

extern struct Item *func_00163D40(struct Obj *obj, int index);
extern int func_001272F0(void *buf, struct Item *item);
extern int func_001272E0(void *buf, struct Item *item);
extern void func_00122410(struct Group *group, int index);
extern int func_00254D08(char *fmt, ...);

/* group at byte offset `off` from the start of the Groups block */
#define GROUP_AT(groups, off) ((struct Group *)((char *)(groups) + (off)))

void func_001937E0(struct Obj *obj) {
    struct {
        char buffer[104];
        struct Context context;
    } locals;
    struct Group *initGroup, *itemGroup, *group;
    struct Item *item;
    struct Context *ctx;
    char *base;
    int i, j, index, count, offset, initOffset, groupOffset, last, end, next;

    obj->x10088->count = 0;
    ctx = &locals.context;
    initOffset = 16;
    i = 99;
    do {
        initGroup = GROUP_AT(obj->x10088, initOffset);
        initGroup->x10 = 1e7f;
        initGroup->x14 = 1e7f;
        initGroup->x18 = 1e7f;
        initGroup->x20 = -1e7f;
        initGroup->x24 = -1e7f;
        initGroup->x28 = -1e7f;
        GROUP_AT(obj->x10088, initOffset)->x0 = 0;
        GROUP_AT(obj->x10088, initOffset)->x4 = 0;
        initOffset += 240;
    } while (--i >= 0);

    locals.context.x1B8 = D_002839D8;
    func_001AC780((AnimationController *)(ctx), 2, 1);

    for (i = 0; i < obj->x5C; i++) {
        item = func_00163D40(obj, i);
        if (func_001272F0(locals.buffer, item) != 10) {
            index = func_001272E0(locals.buffer, item);
            func_00122410(&obj->x10088->group[index], i);
            if (item->x14 & 1) {
                itemGroup = &obj->x10088->group[index];
                if (itemGroup->x4 < 24) {
                    count = itemGroup->x4;
                    itemGroup->xA0[count] = i;
                    itemGroup->x4 = count + 1;
                }
            }
        }
    }

    end = 0;
    last = -1;
    i = 0;
    do {
        next = i + 1;
        group = GROUP_AT(obj->x10088, i * 240 + 16);
        if (group->x0 > 0) {
            group->x10 -= 200.0f;
            group->x14 -= 200.0f;
            group->x18 -= 200.0f;
            group->x20 += 200.0f;
            group->x24 += 200.0f;
            group->x28 += 200.0f;
            if (obj->x1008C && last < i - 1) {
                for (j = last + 1; j < i; j++) {
                    base = (char *)obj->x10088;
                    offset = i * 240 + 16;
                    groupOffset = j * 240 + 16;
                    func_00254D08(D_0028D850, j, GROUP_AT(base, groupOffset)->x0, i,
                                  GROUP_AT(base, offset)->x0);
                }
            }
            last = i;
            end = next;
        }
        i = next;
    } while (i < 100);
    obj->x10088->count = end;
}
