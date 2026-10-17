/* UNIT 0x0013CE48 0x0013D5A0 */
#include "ui_style.h"
typedef struct Entry { int x0, x4, x8; } Entry;
typedef struct Obj {
    int x0; char pad4[0x460-4]; int x460, x464;
    char pad468[0x4BC-0x468]; unsigned char x4BC;
    char pad4BD[0x510-0x4BD]; int x510, x514, x518;
    char pad51C[0x56C-0x51C]; unsigned char x56C;
    char pad56D[7]; Entry *x574; unsigned char x578[100]; int x5DC;
} Obj;
typedef struct Global { char pad[0x10C]; int x10C; char pad110[0x290-0x110]; int x290; } Global;
typedef struct Group { char pad[0x30]; Entry entries[7]; int count; char pad88[0x18]; } Group;
typedef struct Render { char pad[0x20]; int x20; } Render;
extern Global D_004B25A0;
extern Group D_004C8508[4];
extern Render D_003F52D0;
extern int D_0039E7F4[];
extern char **D_0029DD30;
extern Entry D_00275D20[], D_00275D48[], D_00275D88[];
extern char D_002761E8[][32], D_002764A8[][40], D_00276DF8[][30], D_00277270[][30];
extern char D_00365DE0[], D_0036B120[], D_0029C4C8[], D_00289A88[];
extern void func_0018DC20(Obj *);
extern void func_001406F8(void *);
extern int func_001F9E50(void *,Entry *), func_001F9F20(void *,Entry *);
extern int func_001F9C60(), func_001F9C68(), func_001F9C70();
extern int func_00254D68(char *,const char *,...);
extern void func_0020FA98(int,int,int,int,const char *), func_0020F9F0(int,int,int,int,const char *);
extern void func_0013B5E8(Entry *,int,int,int), func_0013D908(Obj *,int);
static inline char *entryName(Entry *e) { return D_00276DF8[e->x0]; }
static inline char *entryKind(Entry *e) { return D_002764A8[e->x4]; }
static inline char *entryDetail(Entry *e) { return D_002761E8[e->x8]; }
static inline char *textAt(int index) { return D_0029DD30[index]; }
static inline Entry *table0() { return D_00275D20; }
static inline Entry *table1() { return D_00275D48; }
static inline Entry *table2() { return D_00275D88; }
void func_0013CE48(Obj *obj) {
    int color1[8], color2[8], color3[8];
    char text[32];
    int special;
    Entry *entry;
    if (!obj->x5DC || !obj->x574) { func_0018DC20(obj); return; }
    special = 0;
    func_0018DC20(obj);
    if (D_004B25A0.x290 == 0xE0 || D_004B25A0.x290 == 0x100 || D_004B25A0.x290 == 0x140 || D_004B25A0.x290 == 0x180 || D_004B25A0.x290 == 0x1A0) special = 1;
    if (!obj->x56C) {
        obj->x4BC = 1;
        func_0018D7D0((UiStyle *)(void *)(color1),317,176,-112,-32,70,70,130,198);
        func_0018D7D0((UiStyle *)(void *)(color2),317,176,-112,-32,143,109,56,198);
        func_0018D7D0((UiStyle *)(void *)(color3),317,176,-112,-32,20,70,30,178);
        entry = &obj->x574[obj->x578[obj->x0]];
        func_001406F8(D_00365DE0);
        if (!special) {
            if (D_004B25A0.x10C == 4) {
                func_00254D68(text,D_0029C4C8,textAt(D_0039E7F4[0]+0x5C));
                func_0018D800((const UiStyle *)(void *)(D_0036B120));
            } else if (func_001F9E50(D_004C8508,entry)) {
                /* find the last group (of 4) containing this entry */
                int found=-1, i;
                i=0;
                do {
                    Group *group=&D_004C8508[i];
                    int j, count;
                    Entry *entries;
                    j=0;
                    /* do/while(0): loop notes act as a scheduling barrier (keeps i+1 last) */
                    do { count=group->count; } while (0);
                    entries=group->entries;
                    goto check;
                    for(;;) {
                        Entry *test;
                        j++;
                    check:
                        if(j>=count) goto nextGroup;
                        test=j+entries;
                        if(test->x4==entry->x4 && test->x0==entry->x0 && test->x8==entry->x8) break;
                    }
                    found=i;
                    /* do/while(0): raises i+1's allocation priority over i */
                    do {
                    nextGroup:
                        i++;
                    } while (0);
                } while (i<4);
                if ((entry->x4==14 && entry->x0==27 && entry->x8==1) || (entry->x4==6 && entry->x0==32 && entry->x8==6))
                    func_00254D68(text,D_0029C4C8,D_0029DD30[0x55]);
                else func_00254D68(text,D_0029C4C8,textAt(found+0x51));
                func_0018D800((const UiStyle *)(void *)(color1));
            } else if (func_001F9F20(D_004C8508,entry)) {
                func_00254D68(text,D_0029C4C8,D_0029DD30[0x4F]);
                func_0018D800((const UiStyle *)(void *)(color2));
            } else if ((unsigned)(entry->x0-35)<3 || entry->x0==33) {
                func_00254D68(text,D_0029C4C8,D_0029DD30[0x57]);
                func_0018D800((const UiStyle *)(void *)(D_0036B120));
            } else {
                Entry *tables[3]={table0(),table1(),table2()};
                int counts[3]={func_001F9C60(),func_001F9C68(),func_001F9C70()};
                int found=0x56;
                int i,j;
                for (i=0;i<3;i++) {
                    for (j=0;j<counts[i];j++) {
                        Entry *test=(Entry *)(j*12+(int)tables[i]);
                        if(test->x0==entry->x0 && test->x4==entry->x4 && test->x8==entry->x8) found=i+0x5C;
                    }
                }
                if(entry->x0==8 && entry->x4==4 && entry->x8==0) found=0x4F;
                func_00254D68(text,D_0029C4C8,D_0029DD30[found]);
                func_0018D800((const UiStyle *)(void *)(color3));
            }
            if (!special) {
                char numbered[48];
                func_00254D68(numbered,D_00289A88,text,obj->x0+1,obj->x5DC);
                func_0020FA98(0,D_003F52D0.x20,obj->x464,obj->x460-10,numbered);
            }
        }
        func_0018D800((const UiStyle *)(void *)(D_0036B120));
        {
            char *name=entryName(entry);
            char *kind=entryKind(entry);
            char *detail=entryDetail(entry);
            func_0020FA98(0,D_003F52D0.x20,obj->x464,obj->x460,name);
            func_0020FA98(0,D_003F52D0.x20,obj->x464,obj->x460+10,kind);
            func_0020FA98(0,D_003F52D0.x20,obj->x464,obj->x460+20,detail);
        }
    } else {
        int a,b;
        entry=obj->x574;
        entry+=obj->x578[obj->x0];
        a=func_001F9F20(D_004C8508,entry);
        b=func_001F9E50(D_004C8508,entry);
        obj->x4BC=0;
        func_0020F9F0(0,D_003F52D0.x20,obj->x464,obj->x460,D_00277270[entry->x0]);
        if((unsigned)(entry->x0-35)>=3 && !a && !b && D_004B25A0.x10C!=4)
            func_0020F9F0(0,D_003F52D0.x20,obj->x464,obj->x460+8,D_0029DD30[0x71]);
    }
    func_0013B5E8(&obj->x574[obj->x578[obj->x0]],obj->x510,obj->x514,obj->x518);
    func_0013D908(obj,1);
}
