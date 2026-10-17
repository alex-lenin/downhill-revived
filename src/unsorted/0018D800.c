/* UNIT 0x0018D800 0x0018D850 */
#include "ui_style.h"
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
void func_0018D800(const UiStyle *p) {
    func_0020FB80(0, p->x, p->y, p->width, p->height);
    func_0020F518(0, p->red, p->green, p->blue, p->alpha);
}
