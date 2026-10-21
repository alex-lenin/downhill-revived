/* UNIT 0x00238740 0x002388F0 */
#include "include_asm.h"

#ifdef NONMATCHING
/* Non-matching: local-alloc puts left/right/top/bottom in saved regs instead of temps, so sched1 orders the
   slt/movz clamps and stores differently; best diff 72/107 words */
/* Small-data globals are owned by src/unsorted/002370E0.c. */
__asm__(".extern D_0029DC58, 4\n.extern D_0029DC5C, 4\n.extern D_0029DC60, 4");
extern int D_0029DC58; /* screen width (12.4 fixed) */
extern int D_0029DC5C; /* screen height (12.4 fixed) */
extern int D_0029DC60;
extern int func_0023D608(void);

/* GS sprite-like record: xy0 at 0x00, uv0 at 0x10, xy1 at 0x20, uv1 at 0x30 */
typedef struct Rect {
    int x0, x4;
    char pad8[8];
    int x10, x14;
    char pad18[8];
    int x20, x24;
    char pad28[8];
    int x30, x34;
} Rect;

extern unsigned char D_0029E3A0[];
extern Rect *D_0029E3B0[];
extern Rect *D_0029E3C8[];

static inline int center(int pos, int span)
{
    int offset = (span >> 1) - 0x8000;
    return pos + offset;
}

void func_00238740(int argX, int argY, int argIndex)
{
    int x = argX, y = argY, index = argIndex;
    int width, height;
    int centerX, centerY, left, right, top, bottom;
    int half;
    int rightInside, bottomInside, negativeLeft, negativeTop;
    int rightUV, bottomUV;
    int uvX0, uvY0, uvX1, uvY1, uvBase;
    Rect *rect, *other;

    if (func_0023D608() >= 2)
        return;
    width = D_0029DC58;
    height = D_0029DC5C;
    centerX = center(x, width);
    centerY = center(y, height);
    left = centerX - 0x400;
    right = centerX + 0x400;
    bottom = centerY + 0x200;
    top = centerY - 0x200;
    if (width < left)
        return;
    if (right < 0)
        return;
    if (height < top)
        return;
    if (bottom < 0)
        return;
    rightInside = right < width;
    rightUV = right > width ? right : width;
    rect = D_0029E3B0[index];
    negativeLeft = left > -1 ? 0 : left;
    bottomInside = bottom < height;
    bottomUV = bottom > height ? bottom : height;
    negativeTop = top > -1 ? 0 : top;
    half = D_0029DC60 >> 1;
    uvBase = 0x8200;
    bottomUV -= uvBase;
    uvY1 = height - bottomUV;
    uvX0 = 0x7E00 - (negativeLeft >> 1);
    uvY0 = 0x7E00 - negativeTop;
    uvX1 = ((width - rightUV) >> 1) + uvBase;
    D_0029E3A0[index] = 1;
    rect->x34 = uvY1;
    rect->x10 = uvX0;
    rect->x14 = uvY0;
    rect->x30 = uvX1;
    if (left < 0)
        left = 0;
    if (top < 0)
        top = 0;
    rect->x0 = left;
    rect->x4 = top;
    rect->x20 = rightInside ? right : width;
    rect->x24 = bottomInside ? bottom : height;
    other = D_0029E3C8[index];
    other->x10 = x - D_0029DC60;
    other->x30 = x + D_0029DC60;
    other->x34 = y + half;
    other->x14 = y - half;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_00238740);
#endif
