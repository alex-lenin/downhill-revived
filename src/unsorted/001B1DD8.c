/* UNIT 0x001B1DD8 0x001B1F68 */
typedef struct Row {
    char pad0[0xD0];
    int xD0[3];
    char padDC[0x28];
    float x104;
    float x108;
    char pad10C[4];
} Row;

#include "rider.h"

typedef struct Game {
    Row rows[4];
    RiderComponent *x440;
} Game;

typedef struct Entry {
    int x0;
    int x4;
    int x8;
    char padC[4];
    int x10;
    int x14;
    signed char x18;
    char pad19[0x37];
} Entry;

typedef struct Group {
    char pad0[4];
    short x4;
    char pad6[2];
    short x8;
    char xA;
    char padB[0x15];
} Group;

typedef struct Header {
    int x0;
    int x4;
    int x8;
    char padC[4];
    float x10;
} Header;

typedef struct Slot {
    float x0;
    int x4;
} Slot;

typedef struct Course {
    char pad0[0x30];
    Entry entries[12];
    char pad3F0[0x260];
    Header x650;
    char pad664[0x1C];
    Group groups[4];
    char pad700[0x4B0];
    Slot xBB0[1];
} Course;

extern void func_001B1C20(Game *, Course *);
extern int func_0023B050(Course *);

void func_001B1DD8(Game *game, Course *course)
{
    Slot *table;
    int base;
    Header *header;
    Entry *e;
    Row *row;
    char *groupBase;
    int k;
    int i;
    int j;
    int next;
    int twice;
    int block;
    int first;
    int stride = 0x110;
    int rowOffset;
    Row *rowAfter;
    float selected;

    header = &course->x650;
    table = course->xBB0;
    func_001B1C20(game, course);
    base = game->x440->rider->riderIndex * 26;
    k = -1;
    i = 0;
    do {
        next = i + 1;
        j = 0;
        twice = i << 1;
        block = i << 5;
        rowOffset = i * stride;
        row = (Row *)((char *)game + rowOffset);
        for (; j < 3; j++) {
            e = &course->entries[++k];
            e->x0 = 0;
            e->x4 = 0;
            e->x8 = 0;
            e->x10 = row->xD0[j];
            rowOffset = j;
            if (rowOffset < 3) {
                e->x18 = func_0023B050(course);
                rowOffset = i * stride;
                rowAfter = (Row *)((char *)game + rowOffset);
                if (j != 0) {
                    selected = rowAfter->x108;
                } else {
                    selected = rowAfter->x104;
                }
                *(float *)((char *)table + ((int)e->x18 << 3)) = selected;
                e->x14 = -1;
            }
        }
        first = twice + i + base;
        groupBase = (char *)course + block;
        *(short *)(groupBase + 0x684) = first;
        *(short *)(groupBase + 0x688) = first + 2;
        *(char *)(groupBase + 0x68A) = 0;
        i = next;
    } while (i < 4);
    course->x650.x0 = 0;
    header->x4 = 0;
    header->x8 = 0;
    course->x650.x10 = 1.0f;
}
