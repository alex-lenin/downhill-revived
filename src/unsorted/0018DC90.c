/* UNIT 0x0018DC90 0x0018DCE8 */
#include "ui_style.h"
struct Pair { short x0; short x2; short x4; };
struct Obj { int x0; int x4; char pad8[0x430]; struct Pair x438; char pad43e[0x7c]; unsigned char x4BA; };
extern void func_0018D970(struct Obj *, struct Pair *);
void func_0018DC90(struct Obj *obj) {
    struct Pair *pair = &obj->x438;
    if (obj->x4BA && pair->x4) {
        func_0018D800((const UiStyle *)(void *)(obj->x4));
        func_0018D970(obj, pair);
    }
}
