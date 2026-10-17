/* UNIT 0x00171A18 0x00171B28 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec4 pos;
} Body;

typedef struct Owner {
    char pad0[0x29CC];
    unsigned char x29CC;
} Owner;

typedef struct Item {
    int state;
    Body *body;
    Owner *owner;
    char padC[0x94];
    Vec4 xA0;
} Item;

typedef struct Global {
    char pad0[0x5019C];
    float x5019C;
} Global;

extern Global D_002AF2C0;
extern int func_0011C0D0(Global *world, Vec4 *pos, int skip, float radius);
extern void func_00171B28(Item *item);
extern void func_00171DD0(Item *item);
extern void func_00172118(Item *item);
extern void *func_001713C8(Owner *owner);
extern void func_00171738(Item *item, void *result);
extern void func_00171958(Item *item, int state);

void func_00171A18(Item *item)
{
    void *result;
    Vec4 *src;

    if (item->state == 2) {
        return;
    }
    if (func_0011C0D0(&D_002AF2C0, &item->body->pos, 1, 750.0f)) {
        if (item->state == 0) {
            func_00171B28(item);
        } else {
            func_00171DD0(item);
        }
        src = &item->xA0;
        /* vector copy (as sceVu0CopyVector) */
        __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(item->body->pos) : "r"(src) : "$2");
        if (item->owner->x29CC == 0) {
            func_00172118(item);
        }
    } else if (D_002AF2C0.x5019C + 400.0f < item->body->pos.z) {
        result = func_001713C8(item->owner);
        if (result != 0) {
            func_00171738(item, result);
        } else {
            func_00171958(item, 2);
        }
    }
}
