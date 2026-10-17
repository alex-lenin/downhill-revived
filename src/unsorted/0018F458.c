/* UNIT 0x0018F458 0x0018F9C0 */
#include "ui_style.h"
typedef struct Entry {
    char pad0[6];
    unsigned char enabled;
    char pad7[9];
} Entry;
typedef struct EntryView { char pad0[0x1E]; unsigned char enabled; } EntryView;
typedef struct Item {
    int index;
    char pad4[8];
    int xC, x10, x14;
    Entry entries[64];
    int count, visible, start;
    char pad424[0x43A-0x424];
    short x43A;
    char pad43C[0x460-0x43C];
    int y, x, spacing, x46C, x470, x474, x478, x47C;
    char pad480[0x4BB-0x480];
    unsigned char x4BB, x4BC;
} Item;
typedef struct Global { char pad0[0x20]; int x20; } Global;
typedef struct Res { char pad0[0x254]; char *x254; } Res;
typedef struct Owner { int x0, x4; } Owner;
extern char D_0029D050[], D_0029D058[], D_0029D060[], D_0029D068[];
extern char D_003F4DD0[], D_00513B00[];
extern int D_003F52F0[];
extern Global D_003F52D0;
extern Res *D_0029DE68;
extern char *func_0018D860(Entry *);
extern int func_00254D68(char *, const char *, ...);
extern int func_0020F860(int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern void func_0020FA98(int, int, int, int, char *);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0018F9C0(Owner *, int, int, int, int);
extern void func_00190E48(Global *, char *, int, int, int, int);
extern void func_00190DB0(Global *, int);
extern void func_002103D8(char *, int, char *);
static inline int MIN(int a,int b) { if (a < b) return a; return b; }
static inline int MAX(int a,int b) { if (a > b) return a; return b; }
void func_0018F458(Owner *owner, Item *item)
{
    char text[0x70];
    char number[0x10];
    int maxWidth = 0;
    int count = item->count;
    int visible = item->visible;
    int start;
    int end;
    int x = item->x;
    int y = item->y;
    int spacing = item->spacing;
    int i;
    int width;
    int drawX;
    int offset;
    int rawWidth;
    int padding;
    int height;
    int boxY;
    {
        /* keep the selected entry inside the visible window */
        int prev = visible - 1;
        int last = count - visible + 1;
        start = MAX(item->index - prev, MIN(item->index, item->start));
        last = MIN(last, start);
        if (last < 0) last = 0;
        start = last;
    }
    item->start = start;
    if (start > 0) {
        func_0018D800((const UiStyle *)(void *)(item->xC));
        if (item->x4BB) func_0020F9A8(0,D_003F52F0[0],x,y-spacing,D_0029DE68->x254);
        else if (item->x4BC) func_0020FA98(0,D_003F52F0[0],x,y-spacing,D_0029DE68->x254);
        else func_0020F9F0(0,D_003F52F0[0],x,y-spacing,D_0029DE68->x254);
    }
    end = MIN(count,start+visible);
    for (i=start;i<end;i++) {
        drawX = x;
        func_00254D68(number,D_0029D050,i+1);
        offset = i*16;
        if (i == item->index && ((EntryView *)(offset+(int)item))->enabled) {
            func_0018D800((const UiStyle *)(void *)(item->x10));
            if (item->x478) {
                drawX = x-22;
                func_00254D68(text,D_0029D058,func_0018D860((Entry *)((char *)item+(offset+0x18))));
            } else if (item->x47C) {
                func_00254D68(text,D_0029D060,number,func_0018D860((Entry *)((char *)item+(offset+0x18))));
            } else {
                func_00254D68(text,D_0029D068,func_0018D860((Entry *)((char *)item+(offset+0x18))));
            }
            rawWidth = func_0020F860(0,text);
            width = rawWidth >> 4;
            if (!item->x478 && item->x46C) {
                if (item->x4BB) func_0018F9C0(owner,x,(i-start)*spacing+y,width+4,9);
                else func_0018F9C0(owner,x-(rawWidth>>5),(i-start)*spacing+y,width+4,9);
            }
        } else {
            if (((Entry *)((char *)item+(offset+0x18)))->enabled == 0) func_0018D800((const UiStyle *)(void *)(item->x14));
            else func_0018D800((const UiStyle *)(void *)(item->xC));
            if (item->x47C) func_00254D68(text,D_0029D060,number,func_0018D860((Entry *)((char *)item+(offset+0x18))));
            else func_00254D68(text,D_0029D068,func_0018D860((Entry *)((char *)item+(offset+0x18))));
            width = func_0020F860(0,text)>>4;
        }
        maxWidth = MAX(width,maxWidth);
        if (item->x4BB) func_0020F9A8(0,D_003F52D0.x20,drawX,(i-start)*spacing+y,text);
        else if (item->x4BC) func_0020FA98(0,D_003F52D0.x20,drawX,(i-start)*spacing+y,text);
        else func_0020F9F0(0,D_003F52D0.x20,x,(i-start)*spacing+y,text);
    }
    if (end<count) {
        func_0018D800((const UiStyle *)(void *)(item->xC));
        if (item->x4BB) func_0020F9A8(0,D_003F52F0[0],x,(end-start)*spacing+y,D_0029DE68->x254);
        else if (item->x4BC) func_0020FA98(0,D_003F52F0[0],x,(end-start)*spacing+y,D_0029DE68->x254);
        else func_0020F9F0(0,D_003F52F0[0],x,(end-start)*spacing+y,D_0029DE68->x254);
    }
    boxY = y;
    padding = owner->x4 + 8;
    height = count*spacing + padding;
    if (item->x474) {
        height += boxY-item->x43A;
        boxY = item->x43A;
    }
    if (item->x46C && count>0) {
      if (maxWidth>0) {
        if (item->x4BB) func_00190E48(&D_003F52D0,D_003F4DD0,x-6,boxY-4,maxWidth+12,height);
        else func_00190E48(&D_003F52D0,D_003F4DD0,x-(maxWidth>>1)-6,boxY-4,maxWidth+12,height);
        func_00190DB0(&D_003F52D0,90);
        func_002103D8(D_00513B00,D_003F52D0.x20,D_003F4DD0);
      }
    }
}
