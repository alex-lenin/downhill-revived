/* UNIT 0x0018E5A8 0x0018E638 */
#include "ui_style.h"
class Handler { public: virtual void update(void *); };
struct Entry { int x0; short x4; char pad[10]; };
struct Obj { int x0, x4, x8; char padC[0x438-12]; Entry x438, x448; char pad458[0x4c8-0x458]; Handler *x4c8; };
extern void func_0018D970(Obj *, Entry *);
void func_0018E5A8(Obj *obj) {
    Entry *entry;
    if (obj->x4c8) obj->x4c8->update(obj);
    entry = &obj->x438;
    if (entry->x4) { func_0018D800((const UiStyle *)(void *)(obj->x4)); func_0018D970(obj,entry); }
    entry = &obj->x448;
    if (entry->x4) { func_0018D800((const UiStyle *)(void *)(obj->x8)); func_0018D970(obj,entry); }
}
