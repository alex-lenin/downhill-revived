/* UNIT 0x0018D7D0 0x0018D800 */
#include "ui_style.h"
UiStyle *func_0018D7D0(UiStyle *rec, int x, int y, int width, int height, int red, int green, int blue, int alpha)
{
    rec->x = x;
    rec->y = y;
    rec->width = width;
    rec->height = height;
    rec->red = red;
    rec->green = green;
    rec->blue = blue;
    rec->alpha = alpha;
    return rec;
}
