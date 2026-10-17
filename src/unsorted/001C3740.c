/* UNIT 0x001C3740 0x001C4E58 .rodata=0x28FD20:0x28FF90 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Obj {
    int x0, x4, x8;
    float *xC, *x10;
    int x14;
    float x18, x1C;
    unsigned int x20[5];
    unsigned int x34;
    int x38, x3C, x40, x44;
    char x48[1];
} Obj;
typedef struct Entry { char pad[0x597C]; int x597C; } Entry;
extern int D_004B2828[];
extern int D_004BD8B4[];
extern char D_004BD840[];
typedef struct Slot { int state; char pad[0x80]; } Slot;
typedef struct World { char pad[0x597C]; Slot slots[1]; } World;
extern World D_00398C40;
extern int D_004B2A90[];
extern char D_0028FD10[];
extern int func_0023CF78(int);
extern char *func_0023CF58(int);
extern float func_00222640(char *);
#if defined(__mips__)
extern int func_00208C60(float, int, int);
#else
#include "sound_math.h"
#define func_00208C60 SoundHeadingFirst
#endif
extern int func_00208E40(int, int, int, int);
extern void func_00201DE8(unsigned int, int, int, int, int, int);
extern void func_00208BF8(char *, unsigned int *);
extern void func_001C3550(Obj *, u128);
extern void func_001C3668(Obj *);
extern int func_00238F48(void);
extern int func_001C1F50(char *, int, int, int);
extern int func_001C5040(int *, int);
extern int func_00222B08(int, int);
#define SOUND_S_CHAR_WOOSH_53 (((u128)0x33355F48534F4FUL << 64) | (u128)0x575F524148435F53UL)
#define SOUND_S_BIKE_ENGAGE (((u128)0x454741474EUL << 64) | (u128)0x455F454B49425F53UL)
#define SOUND_B_DIRT_LAND_BG (((u128)0x47425F444E41UL << 64) | (u128)0x4C5F545249445F42UL)
#define SOUND_B_DIRT_SKID (((u128)0x44494BUL << 64) | (u128)0x535F545249445F42UL)
#define SOUND_S_KICK_01 (((u128)0x31UL << 64) | (u128)0x305F4B43494B5F53UL)
#define SOUND_S_FOOTSTEP_02 (((u128)0x32305F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_CELLBEEP_03 (((u128)0x33305F5045UL << 64) | (u128)0x45424C4C45435F53UL)
#define SOUND_S_CELL_RING_05 (((u128)0x35305F474E49UL << 64) | (u128)0x525F4C4C45435F53UL)
#define SOUND_S_JUMPWHOOSH_07 (((u128)0x37305F48534F4FUL << 64) | (u128)0x4857504D554A5F53UL)
#define SOUND_S_JUMPWHOOSH_08 (((u128)0x38305F48534F4FUL << 64) | (u128)0x4857504D554A5F53UL)
#define SOUND_S_JUMPTICK_09 (((u128)0x39305F4B43UL << 64) | (u128)0x4954504D554A5F53UL)
#define SOUND_S_FOOTSTEP_10 (((u128)0x30315F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_BALLHAND_12 (((u128)0x32315F444EUL << 64) | (u128)0x41484C4C41425F53UL)
#define SOUND_S_BALLGROUND_13 (((u128)0x33315F444E554FUL << 64) | (u128)0x52474C4C41425F53UL)
#define SOUND_S_FOOTSTEP_51 (((u128)0x31355F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_PUMP_52 (((u128)0x32UL << 64) | (u128)0x355F504D55505F53UL)
#define SOUND_S_FOOTSTEP_20 (((u128)0x30325F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_FOOTSCUFF_14 (((u128)0x34315F464655UL << 64) | (u128)0x4353544F4F465F53UL)
#define SOUND_S_FOOTSCUFF_15 (((u128)0x35315F464655UL << 64) | (u128)0x4353544F4F465F53UL)
#define SOUND_S_FOOTSTEP_16 (((u128)0x36315F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_SLURP_22 (((u128)0x3232UL << 64) | (u128)0x5F5052554C535F53UL)
#define SOUND_S_HACKYGRND_23 (((u128)0x33325F444E52UL << 64) | (u128)0x47594B4341485F53UL)
#define SOUND_S_HACKYFOOT_24 (((u128)0x34325F544F4FUL << 64) | (u128)0x46594B4341485F53UL)
#define SOUND_S_FOOTSTEP_25 (((u128)0x35325F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_ROCKBURST_59 (((u128)0x39355F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_FOOTSTEP_73 (((u128)0x33375F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_HANDSTEP_74 (((u128)0x34375F5045UL << 64) | (u128)0x5453444E41485F53UL)
#define SOUND_S_ROCKBURST_66 (((u128)0x36365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_ROCKBURST_67 (((u128)0x37365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_ROCKBURST_68 (((u128)0x38365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_ROCKBURST_69 (((u128)0x39365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_EYE_OUT_70 (((u128)0x30375F54UL << 64) | (u128)0x554F5F4559455F53UL)
#define SOUND_S_EYE_CATCH_71 (((u128)0x31375F484354UL << 64) | (u128)0x41435F4559455F53UL)
#define SOUND_S_GULP_28 (((u128)0x38UL << 64) | (u128)0x325F504C55475F53UL)
#define SOUND_S_BLENDER_29 (((u128)0x39325F52UL << 64) | (u128)0x45444E454C425F53UL)
#define SOUND_S_ROCKBURST_60 (((u128)0x30365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_FOOTSTEP_30 (((u128)0x30335F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_SHOVELDIG_34 (((u128)0x34335F474944UL << 64) | (u128)0x4C45564F48535F53UL)
#define SOUND_S_SHOVELPACK_36 (((u128)0x36335F4B434150UL << 64) | (u128)0x4C45564F48535F53UL)
#define SOUND_S_SHOVELGRND_37 (((u128)0x37335F444E5247UL << 64) | (u128)0x4C45564F48535F53UL)
#define SOUND_S_ROCKBURST_61 (((u128)0x31365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_TIREBNCE_39 (((u128)0x39335F4543UL << 64) | (u128)0x4E42455249545F53UL)
#define SOUND_S_ROCKBURST_62 (((u128)0x32365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_TIRESPIN_40 (((u128)0x30345F4E49UL << 64) | (u128)0x5053455249545F53UL)
#define SOUND_S_ROCKBURST_63 (((u128)0x33365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_HANDBAR_43 (((u128)0x33345F52UL << 64) | (u128)0x4142444E41485F53UL)
#define SOUND_S_FOOTSTEP_45 (((u128)0x35345F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_ROCKBURST_64 (((u128)0x34365F545352UL << 64) | (u128)0x55424B434F525F53UL)
#define SOUND_S_WHOOSH_48 (((u128)0x38345FUL << 64) | (u128)0x48534F4F48575F53UL)
#define SOUND_S_FOOTSTEP_50 (((u128)0x30355F5045UL << 64) | (u128)0x5453544F4F465F53UL)
#define SOUND_S_ROCKBURST_65 (((u128)0x35365F545352UL << 64) | (u128)0x55424B434F525F53UL)

void func_001C3740(Obj *obj, int player)
{
    u128 name = 0;
    int event = 0;
    char *data;
    int soundSystem;
    int volume, pitch, current;
    int count;
    int elapsed;
    unsigned int *handle, *out;
    unsigned int old;
    float currentFloat;
    __builtin_memcpy((char *)&name, D_0028FD10, 9);
    soundSystem = func_0023CF78(0);
    data = func_0023CF58(0) + 0x10;
    if (D_004B2828[0] != 1) return;
    if (obj->x10) {
        pitch = func_00208C60(func_00222640(data), soundSystem, obj->x4);
        volume = func_00208E40(soundSystem, obj->x4, 100, 150);
        event = (int)(*obj->x10 + 0.5f);
        if (obj->x8 && event && obj->x1C == (float)event) {
            switch (event) {
            case 53:
                func_00201B68(D_004BD8B4[0], 0, SOUND_S_CHAR_WOOSH_53, volume, pitch, 0, 0);
                obj->x8 = 0;
                break;
            case 54:
                func_00201B68(D_004BD8B4[0], 0, SOUND_S_BIKE_ENGAGE, volume, pitch, 0, 0);
                obj->x8 = 0;
                break;
            case 55:
                func_001C3668(obj);
                break;
            case 56:
                func_00208BF8(D_004BD840, &obj->x34);
                break;
            case 57:
                func_00201B68(D_004BD8B4[0], 0, SOUND_B_DIRT_LAND_BG, volume, pitch, 0, 0);
                obj->x8 = 0;
                break;
            case 58:
                func_00201B68(D_004BD8B4[0], 0, SOUND_B_DIRT_SKID, volume, pitch, 0, 0);
                obj->x8 = 0;
                break;
            }
        }
    }
    if (obj->x34 != 0xFFFFFFFF && obj->x34 != 0) {
        func_00201DE8(obj->x34, 5, 0x400,
            func_00208C60(func_00222640(data), soundSystem, obj->x4), 0, 0);
    }
    pitch = func_00208C60(func_00222640(data), soundSystem, obj->x0);
    volume = func_00208E40(soundSystem, obj->x0, 13, 25);
    elapsed = func_00238F48() - D_004B2A90[2];
    if (volume > 0 && player != obj->x14 && elapsed > 120 &&
        func_00238F48() - obj->x38 > (obj->x3C + obj->x40) * 60) {
        if (func_001C1F50(obj->x48, 26, obj->x0,
                func_001C5040(D_004B2A90, obj->x14)) != 5) {
            obj->x38 = func_00238F48();
            obj->x40 = func_00222B08(3, 9);
        }
    }
    pitch = func_00208C60(func_00222640(data), soundSystem, obj->x0);
    volume = func_00208E40(soundSystem, obj->x0, 13, 17);
    if (obj->xC) {
        current = (int)(*obj->xC + 0.5f);
        for (count = 0; count < 5; count++) {
            old = obj->x20[count];
            if (old != 0xFFFFFFFF && old != 0) {
                obj->x20[count] = 0xFFFFFFFF;
                func_00201E08(old, 5, volume, pitch, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->x20[count]));
            }
        }
        currentFloat = (float)current;
        if (current && obj->x18 == currentFloat && current == 200) {
            if (obj->x44 != 5) {
                D_00398C40.slots[obj->x44].state = 0;
                obj->x44 = 5;
                obj->x8 = 0;
            }
        }
        if (obj->x8 && current && obj->x18 == currentFloat) {
            switch (obj->x14) {
            case 1:
                switch (current) {
                case 1:
                    func_001C3550(obj, SOUND_S_KICK_01);
                    break;
                case 2:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_02);
                    break;
                case 3:
                    func_001C3550(obj, SOUND_S_CELLBEEP_03);
                    break;
                case 5:
                    func_001C3550(obj, SOUND_S_CELL_RING_05);
                    break;
                }
                break;
            case 2:
                switch (current) {
                case 7:
                    func_001C3550(obj, SOUND_S_JUMPWHOOSH_07);
                    break;
                case 8:
                    func_001C3550(obj, SOUND_S_JUMPWHOOSH_08);
                    break;
                case 9:
                    func_001C3550(obj, SOUND_S_JUMPTICK_09);
                    break;
                case 10:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_10);
                    break;
                }
                break;
            case 3:
                switch (current) {
                case 12:
                    func_001C3550(obj, SOUND_S_BALLHAND_12);
                    break;
                case 13:
                    func_001C3550(obj, SOUND_S_BALLGROUND_13);
                    break;
                case 14:
                    func_001C3550(obj, SOUND_S_FOOTSCUFF_14);
                    break;
                case 15:
                    func_001C3550(obj, SOUND_S_FOOTSCUFF_15);
                    break;
                case 16:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_16);
                    break;
                }
                break;
            case 4:
                switch (current) {
                case 50:
                    break;
                case 51:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_51);
                    break;
                case 52:
                    func_001C3550(obj, SOUND_S_PUMP_52);
                    break;
                }
                break;
            case 5:
                switch (current) {
                case 20:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_20);
                    break;
                case 14:
                    func_001C3550(obj, SOUND_S_FOOTSCUFF_14);
                    break;
                case 15:
                    func_001C3550(obj, SOUND_S_FOOTSCUFF_15);
                    break;
                case 16:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_16);
                    break;
                }
                break;
            case 6:
                switch (current) {
                case 22:
                    func_001C3550(obj, SOUND_S_SLURP_22);
                    break;
                case 23:
                    func_001C3550(obj, SOUND_S_HACKYGRND_23);
                    break;
                case 24:
                    func_001C3550(obj, SOUND_S_HACKYFOOT_24);
                    break;
                case 25:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_25);
                    break;
                case 53:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_CHAR_WOOSH_53, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 54:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_BIKE_ENGAGE, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                }
                break;
            case 7:
                switch (current) {
                case 59:
                    func_001C3550(obj, SOUND_S_ROCKBURST_59);
                    break;
                case 73:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_FOOTSTEP_73, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 74:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_HANDSTEP_74, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                }
                break;
            case 14: {
                float f = func_00222640(data);
                pitch = func_00208C60(f, soundSystem, obj->x4);
                switch (current) {
                case 66:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_ROCKBURST_66, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 67:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_ROCKBURST_67, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 68:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_ROCKBURST_68, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 69:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_ROCKBURST_69, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 70:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_ROCKBURST_69, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 71:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_EYE_OUT_70, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 72:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_EYE_CATCH_71, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 53:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_CHAR_WOOSH_53, 0x400, pitch, 0, 0);
                    obj->x8 = 0;
                    break;
                case 54:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_BIKE_ENGAGE, 0x400, pitch, 0, 0);
                    obj->x8 = 0;
                    break;
                case 55:
                    func_001C3668(obj);
                    break;
                case 56:
                    func_00208BF8(D_004BD840, &obj->x34);
                    break;
                case 57:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_B_DIRT_LAND_BG, 0x400, pitch, 0, 0);
                    obj->x8 = 0;
                    break;
                case 58:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_B_DIRT_SKID, 0x400, pitch, 0, 0);
                    obj->x8 = 0;
                    break;
                }
                break;
            }
            case 8:
                switch (current) {
                case 28:
                    func_001C3550(obj, SOUND_S_GULP_28);
                    break;
                case 29:
                    func_001C3550(obj, SOUND_S_BLENDER_29);
                    break;
                case 60:
                    func_001C3550(obj, SOUND_S_ROCKBURST_60);
                    break;
                case 30:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_FOOTSTEP_30, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 53:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_CHAR_WOOSH_53, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                case 54:
                    func_00201B68(D_004BD8B4[0], 0, SOUND_S_BIKE_ENGAGE, 0x400, 0, 0, 0);
                    obj->x8 = 0;
                    break;
                }
                break;
            case 9:
                switch (current) {
                case 34:
                    func_001C3550(obj, SOUND_S_SHOVELDIG_34);
                    break;
                case 35:
                    func_001C3550(obj, SOUND_S_SHOVELPACK_36);
                    break;
                case 37:
                    func_001C3550(obj, SOUND_S_SHOVELGRND_37);
                    break;
                case 61:
                    func_001C3550(obj, SOUND_S_ROCKBURST_61);
                    break;
                }
                break;
            case 10:
                switch (current) {
                case 39:
                    func_001C3550(obj, SOUND_S_TIREBNCE_39);
                    break;
                case 62:
                    func_001C3550(obj, SOUND_S_ROCKBURST_62);
                    break;
                case 40:
                    func_001C3550(obj, SOUND_S_TIRESPIN_40);
                    break;
                }
                break;
            case 11:
                switch (current) {
                case 63:
                    func_001C3550(obj, SOUND_S_ROCKBURST_63);
                    break;
                }
                break;
            case 12:
                switch (current) {
                case 43:
                    func_001C3550(obj, SOUND_S_HANDBAR_43);
                    break;
                case 45:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_45);
                    break;
                case 64:
                    func_001C3550(obj, SOUND_S_ROCKBURST_64);
                    break;
                }
                break;
            case 13:
                switch (current) {
                case 48:
                    func_001C3550(obj, SOUND_S_WHOOSH_48);
                    break;
                case 50:
                    func_001C3550(obj, SOUND_S_FOOTSTEP_50);
                    break;
                case 65:
                    func_001C3550(obj, SOUND_S_ROCKBURST_65);
                    break;
                }
                break;
            }
        } else if (current == 0 && event == 0) {
            obj->x8 = 1;
        }
        obj->x18 = currentFloat;
        obj->x1C = (float)event;
    }
}

/* Adjacent bank names complete the final jump table alignment. */
const char D_0028FF08[16] __attribute__((aligned(8))) = "SHELL2-L.VAG";
const char D_0028FF18[16] __attribute__((aligned(8))) = "SHELL2-R.VAG";
const char D_0028FF28[16] __attribute__((aligned(8))) = "SHELL3IL.VAG";
const char D_0028FF38[16] __attribute__((aligned(8))) = "SHELL3IR.VAG";
const char D_0028FF48[16] __attribute__((aligned(8))) = "SHELL3BL.VAG";
const char D_0028FF58[16] __attribute__((aligned(8))) = "SHELL3BR.VAG";
const char D_0028FF68[16] __attribute__((aligned(8))) = "SHELL1-L.VAG";
const char D_0028FF78[24] __attribute__((aligned(8))) = "SHELL1-R.VAG";
