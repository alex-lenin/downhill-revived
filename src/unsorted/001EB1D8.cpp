/* UNIT 0x001EB1D8 0x001F8200 .rodata=0x2969E0:0x296E10 .sbss=0x29DFFC:0x29E000 .sdata=0x29D7E0:0x29D938 */
#include "render_wait.h"
#include "animation_weight.h"
#if defined(__mips__)
#define CallShared00203B68_0 ((void (*)(void *, float, int))func_00203B68__c62)
#else
#define CallShared00203B68_0 Forward00203B68_0
#endif
#if defined(__mips__)
#define CallShared00203B68_1 func_00203B68__c16
#else
#define CallShared00203B68_1 Forward00203B68_0
#endif
#if defined(__mips__)
#define CallShared00203B68_2 func_00203B68__c16
#else
#define CallShared00203B68_2 Forward00203B68_0
#endif
#if defined(__mips__)
#define CallShared00203B68_3 func_00203B68__c16
#else
#define CallShared00203B68_3 Forward00203B68_0
#endif
#include "scalar_api.h"
#include "menu.h"
#include "animation_select.h"

#if defined(__mips__)
#define CallScalarBridge0023D4F8_28_func_0023D4F8__target func_0023D4F8__target
#else
#define CallScalarBridge0023D4F8_28_func_0023D4F8__target ScalarBridge0023D4F8_28
#endif

#if defined(__mips__)
#define CallScalarBridge0023D4F8_28_func_0023D4F8__n6 func_0023D4F8__n6
#else
#define CallScalarBridge0023D4F8_28_func_0023D4F8__n6 ScalarBridge0023D4F8_28
#endif

#if defined(__mips__)
#define CallScalarBridge0023D4F8_28_func_0023D4F8 func_0023D4F8
#else
#define CallScalarBridge0023D4F8_28_func_0023D4F8 ScalarBridge0023D4F8_28
#endif
#include "camera.h"
#include "timer.h"
#include "include_asm.h"
extern "C" {

typedef struct { int x0, x4; } Pair;
typedef struct { char *x0, *x4; } StrPair;

/* .sdata of this file, in address order (D_0029D840..D_0029D8F8 are menu strings used by
   the INCLUDE_ASM functions) */
float D_0029D7E0 = 0.85f;
float D_0029D7E4 = 0.15f;
int D_0029D7E8 = 1;
int D_0029D7EC = 1;
int D_0029D7F0 = 0;
/* max.s comes from g++'s maximum operator `>?` (MAX_EXPR), not inline asm. */
unsigned char D_0029D7F4 = 0;
unsigned int D_0029D7F8 = 0;
unsigned int D_0029D7FC = 0;
int D_0029D800 = 0;
int D_0029D804 = 0;
Pair D_0029D808 = { 0x79, 0 };
Pair D_0029D810 = { 0xA6, 0 };
Pair D_0029D818 = { 0xBB, 0 };
Pair D_0029D820 = { 0x69, 0 };
Pair D_0029D828 = { 0xC0, 0 };
Pair D_0029D830 = { 0xE4, 0 };
Pair D_0029D838 = { 0, 0 };
char D_0029D840[8] = " ";
StrPair D_0029D848 = { 0, 0 };
StrPair D_0029D850 = { D_0029D840, (char *)0x00295E90 };
StrPair D_0029D858 = { D_0029D840, (char *)0x00295E90 };
StrPair D_0029D860 = { D_0029D840, (char *)0x00295E90 };
StrPair D_0029D868 = { D_0029D840, (char *)0x00295E90 };
Pair D_0029D870 = { 0x0A8D0A8F, 0 };
Pair D_0029D878 = { 0x0AA70AAD, 0 };
char D_0029D880[8] = "OPTIONS";
char D_0029D888[8] = { ' ', 'a', ')', 0, '0', 'a', ')', 0 };
char D_0029D890[8] = "CAREER";
char D_0029D898[8] = "ARCADE";
char D_0029D8A0[8] = "NO SAVE";
char D_0029D8A8[8] = "POSTERS";
Pair D_0029D8B0 = { 0x002963C0, 0 };
char D_0029D8B8[8] = "VIBR.";
char D_0029D8C0[8] = "OFF";
char D_0029D8C8[8] = "ON";
char D_0029D8D0[8] = "EASY";
char D_0029D8D8[8] = "MEDIUM";
char D_0029D8E0[8] = "RACE";
char D_0029D8E8[8] = "pause";
char D_0029D8F0[8] = "brake";
char D_0029D8F8[8] = "TARGET";
StrPair D_0029D900 = { D_0029D840, (char *)0x002968E8 };
int D_0029D908 = 0;
int D_0029D90C = 1;
int D_0029D910 = 2;
int D_0029D914 = 3;
int D_0029D918 = 4;
int D_0029D91C = 5;
int D_0029D920 = 0;
int D_0029D924 = 0;
char D_0029D928[8] = "%s";

typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct { long x0, x8; } __attribute__((packed)) Value;
typedef struct { char pad0[0x3C]; unsigned char selectable[32]; } RiderFlag;
typedef struct { char pad0[0x290]; int selected[6]; } PlayerSlot;
typedef struct { char pad0[0x2A8]; int player[32]; } RiderSlotMap;
typedef struct { char pad0[0x4F0]; int selected; } SelectedRider;
typedef struct { char pad0[4]; unsigned char active; } Object;
typedef struct { Object *object; char pad4[0x1DC]; } ObjectRecord;
typedef struct { char pad0[0x1740]; ObjectRecord slots[32]; } ObjectSlots;
typedef struct { char pad0[0x3540]; Object *object; } SpecialSlot;
typedef struct { char pad0[0xB8]; float value; } Animation;
typedef struct { char pad0[0x1310]; Animation *animation; } AnimationOwner;

extern Value D_004C6360, D_004C6370;
extern Value *D_00275B70[], *D_00275BB0[];
extern int D_00275BF0[], D_00275C30[], D_00275C68[], D_00275C70[];
extern int D_004C6910[], D_004C6780[], D_004C69A0[], D_00296CA0[];
extern int D_0029DF68, D_0029DF70, D_0029DF7C;
extern int D_0029DED8, D_0029DEEC, D_0029DEF0;
extern int D_0029E020, D_0029E024, D_0029E028, D_0029E02C;
extern unsigned char D_0029DF84, D_0029DF85, D_0029DF88;
extern unsigned char D_0029DF89, D_0029DF8B, D_0029DF8C;
extern char **D_0029E03C, **D_0029E040, **D_0029E044, **D_0029E048;
extern char D_004B25A0[], D_004B2A90[], D_004225A0[], D_003F52D0[], D_004C7530[];
extern unsigned char D_004B25EA[];
extern int D_004B2828[], D_004B2848[];
extern char D_00296C70[], D_00296C80[], D_00296C90[];

extern float func_00203C28(Value *);
extern int func_00204230(Value *);
extern void func_00203B68(Value *, int, float);
extern void func_00203E10(Value *, float);
extern void func_002038E0(Value *, int);
extern void func_00203FE0(Value *, int);
extern void func_002039B0(Value *, int);
extern void func_00204080(Value *, int);
extern void func_001C57A8(char *, u128);
extern void func_001C58A8(char *);
extern void func_001C59B0(char *);
extern void func_001C59D8(char *);
extern void func_001C5948(char *);
extern void func_001F57D0(int, int);
extern void func_001F5398(int, int, int, int, int, char *, int);
extern void func_001F7B80(void);
extern void func_001F6878(int, int);
extern void func_001F6798(int);
extern int func_001F7AF8(int);
extern int func_00193FC8(char *, int);
extern void func_001BAF10(char *, int);
extern void func_001F4A98(void);
extern void func_001F5578(void);
extern void func_001F6338(int);
extern void func_001F50F0(int, int);
extern void func_001F5B08(int);
extern void func_001F6940(void);
extern void func_001F65E0(int);
extern void func_001F60C8(int);
extern void func_0021E428(int);
extern void func_001F95D0(char *, int);
extern void func_00190108(char *, char *);

#define RIDER D_004C6910[D_0029DF68]
#define SELECTABLE(index) (((RiderFlag *)D_004B25A0)->selectable[(index)])
#define SELECT_SOUND ((((((u128)0xA886 << 16) | 0x8A98) << 16 | 0x8AA6) << 15) | 0x5F53)
#define CANCEL_SOUND ((((((u128)0x988A << 16) | 0x869C) << 16 | 0x8286) << 15) | 0x5F53)
#define BACK_SOUND (((((u128)0x9686 << 16) | 0x8284) << 15) | 0x5F53)
#define PREVIOUS_SOUND (((u128)0x5355 << 64) | (u128)0x4F49564552505F53UL)
#define NEXT_SOUND (((((u128)0xA8B0 << 16) | 0x8A9C) << 15) | 0x5F53)

/* ---- func_001EB1D8, func_001EB370 ---- */
/* Objects at world+0x5E40.. are C++ objects; the 4th virtual says whether they are active. */
class Mode__c0 {
public:
    int x0;
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual int IsActive();
};

typedef struct Owner__c0 {
    char *world;
    int x4;
    int x8;
    int xC;
    float x10;
    float x14;
} Owner__c0;

extern float fabsf(float);
extern float func_00236088(float cur, float target, float rate, float min);
extern void func_001EB478__c0(Owner__c0 *obj) asm("func_001EB478");
extern void func_001AC578(char *);
extern void func_0012FE38(char *, Mode__c0 *);

static inline int IsPositive__c0(float value)
{
    int result = 1;
    if (!(value > 0.0f))
        result = 0;
    return result;
}

void func_001EB1D8(Owner__c0 *obj)
{
    char *world = obj->world;
    char *part = world + 0x1E40;
    float value;
    float rate;
    Mode__c0 *mode;
    int flag;

    do { value = fabsf(*(float *)(world + 0x48B4)); } while (0);
    do { rate = D_0029D7E4; } while (0);
    mode = (Mode__c0 *)(world + 0x5E40);
    obj->x10 = func_00236088(obj->x10, value - D_0029D7E0, rate, rate * 0.333f);
    obj->x14 = func_00236088(obj->x14, 1.0f, D_0029D7E4, D_0029D7E4 * 0.333f);
    func_001EB478__c0(obj);
    func_001AC578(part);
    if (mode->IsActive()) {
        func_0012FE38(world, mode);
    } else {
        obj = (Owner__c0 *)(world + 0x5FF0);
        if (((Mode__c0 *)obj)->IsActive()
            || (obj = (Owner__c0 *)(world + 0x62E0), ((Mode__c0 *)obj)->IsActive())
            || (obj = (Owner__c0 *)(world + 0x6310), ((Mode__c0 *)obj)->IsActive())) {
            func_0012FE38(world, (Mode__c0 *)obj);
        } else {
            flag = *(unsigned char *)(world + 0x13DE) ? *(int *)(world + 0x1274) : *(int *)(world + 0x11AC);
            if (!flag || !(IsPositive__c0(value = *(float *)(part + 0x1058)) & 0xFF))
                func_0012FE38(world, (Mode__c0 *)(world + 0x4ED0));
        }
    }
}

void func_001EB370(Owner__c0 *obj)
{
    obj->world[0x2EC1] = 0;
}

/* ---- merged from src/unsorted/001EB380.c ---- */
extern "C" {
typedef struct World__c1 {
    char pad0[0x4ECC];
    struct Obj__c1 *x4ECC;
} World__c1;

typedef struct Obj__c1 {
    World__c1 *world;
    char pad4[0x8 - 0x4];
    char moving;
    char pad9[0x10 - 0x9];
    float x10;
    float x14;
} Obj__c1;

extern float func_00236088__c1(float cur, float target, float rate, float min) asm("func_00236088");
extern void func_001EB478__c1(Obj__c1 *obj) asm("func_001EB478");

extern int func_001EB380__c1(Obj__c1 *obj) asm("func_001EB380");
int func_001EB380__c1(Obj__c1 *obj)
{
    if (obj->world->x4ECC != obj) {
        obj->x10 = func_00236088__c1(obj->x10, 0.0f, D_0029D7E4, D_0029D7E4 * 0.33f);
        obj->x14 = func_00236088__c1(obj->x14, 0.0f, D_0029D7E4, D_0029D7E4 * 0.333f);
        func_001EB478__c1(obj);
    }
    obj->moving = obj->x10 != 0.0f || obj->x14 != 0.0f;
    return obj->x10 != 0.0f || obj->x14 != 0.0f;
}
}
/* ---- merged from src/unsorted/001EB478.c ---- */
extern "C" {
struct Sub__c2 {
    char pad0[0xC];
    float xC;
};

struct World__c2 {
    char pad0[0x3530];
    char x3530[0x49AC - 0x3530];
    struct Sub__c2 x49AC;
};

struct Obj__c2 {
    struct World__c2 *world;
    char pad4[0x10 - 0x4];
    float x10;
    float x14;
};



extern void func_001EB478__c2(struct Obj__c2 *obj) asm("func_001EB478");
void func_001EB478__c2(struct Obj__c2 *obj)
{
    struct Sub__c2 *sub;

    func_001B3028((RiderCamera *)(obj->world->x3530), 11, obj->x10);
    sub = &obj->world->x49AC;
    sub->xC *= 1.0f - obj->x14;
}
}


typedef struct Panel {
    int x0;
    char pad4[0x1E0 - 4];
} Panel;

typedef struct State {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
    char pad28C[0x374 - 0x28C];
    char x374[0x4F0 - 0x374];
    char x4F0[0x1710 - 0x4F0];
    Panel x1710[(0xB174 - 0x1710) / 0x1E0];
    char padB0D0[0xB174 - 0x1710 - ((0xB174 - 0x1710) / 0x1E0) * 0x1E0];
    int xB174;
    int xB178;
} State;

typedef struct Fade {
    int x0;
    int x4;
    float x8;
    int xC;
} __attribute__((aligned(16))) Fade;

extern void func_0018DD08__target(int) asm("func_0018DD08");
extern int func_0018DF28__target(int, int) asm("func_0018DF28");

extern void func_001BC4F0__target(void *) asm("func_001BC4F0");
extern void func_001C5358__target(void *, int) asm("func_001C5358");
extern void func_001C5728__target(void *) asm("func_001C5728");
extern void func_001C57A8__target(void *, u128) asm("func_001C57A8");
extern void func_001C5B80__target(void *) asm("func_001C5B80");
extern void func_001C5DF0__target(void *) asm("func_001C5DF0");
extern void func_001EC398__target(char *) asm("func_001EC398");
extern void func_001ED3E0__target(void) asm("func_001ED3E0");
extern void func_001ED5B8__target(void) asm("func_001ED5B8");
extern void func_001ED900__target(void) asm("func_001ED900");
extern void func_001EDD28__target(void) asm("func_001EDD28");
extern void func_001EE2A0__target(void) asm("func_001EE2A0");
extern void func_001EE8A8__target(void) asm("func_001EE8A8");
extern void func_001EED78__target(void) asm("func_001EED78");
extern void func_001EF0E0__target(void) asm("func_001EF0E0");
extern void func_001EF5D0__target(void) asm("func_001EF5D0");
extern void func_001EFA78__target(void) asm("func_001EFA78");
extern void func_001EFE28__target(void) asm("func_001EFE28");
extern void func_001F0160__target(void) asm("func_001F0160");
extern void func_001F0A28__target(void) asm("func_001F0A28");
extern void func_001F1168__target(void) asm("func_001F1168");
extern void func_001F14A0__target(void) asm("func_001F14A0");
extern void func_001F1500__target(void) asm("func_001F1500");
extern void func_001F1968__target(void) asm("func_001F1968");
extern void func_001F1A90__target(void) asm("func_001F1A90");
extern void func_001F2098__target(void) asm("func_001F2098");
extern void func_001F24B8__target(void) asm("func_001F24B8");
extern void func_001F3B98__target(void) asm("func_001F3B98");
extern void func_001F43C0__target(void) asm("func_001F43C0");
extern void func_001F4508__target(void) asm("func_001F4508");
extern void func_001F4960__target(void) asm("func_001F4960");
extern void func_001F4DD8__target(void) asm("func_001F4DD8");
extern void func_001F4E40__target(void) asm("func_001F4E40");
extern void func_001F50F0__target(int, int) asm("func_001F50F0");
extern void func_001F5100__target(void) asm("func_001F5100");
extern void func_001F5578__target(void) asm("func_001F5578");
extern void func_001F57D0__target(int, int) asm("func_001F57D0");
extern void func_001F6AB8__target(void) asm("func_001F6AB8");
extern void func_001F79D8__target(int, int) asm("func_001F79D8");
extern void func_001F7A60__target(int) asm("func_001F7A60");
extern void func_001F7B80__target(void) asm("func_001F7B80");
extern void func_002038E0__target(void *, int) asm("func_002038E0");
extern void func_00203B68__target(void *, int, float) asm("func_00203B68");
extern void func_002065C0__target(void) asm("func_002065C0");
#include "scene_effects.h"
extern void func_0020F2A8__target(int) asm("func_0020F2A8");
extern void func_002102A8__target(void *, int, int) asm("func_002102A8");
extern void func_00210460__target(void *, int, int) asm("func_00210460");
extern void func_00215B68__target(int, int, int, int) asm("func_00215B68");
extern void func_002161B8__target(int, int, int, int) asm("func_002161B8");
extern void func_002184D0__target(int, int, int) asm("func_002184D0");
extern void func_00219EC8__target(int) asm("func_00219EC8");
extern int func_0021D638__target(int, float) asm("func_0021D638");
extern void func_0021D6F8__target(int, Fade *) asm("func_0021D6F8");
extern void func_0021D748__target(int, float) asm("func_0021D748");
extern void func_0021D7E8__target(void) asm("func_0021D7E8");
extern void func_0021E240__target(void) asm("func_0021E240");
extern void func_0021E428__target(int) asm("func_0021E428");
extern void func_0021E438__target(float) asm("func_0021E438");
extern void func_0021F0A0__target(int, int, int) asm("func_0021F0A0");
extern void func_0021F1B8__target(int, int, int, int) asm("func_0021F1B8");
extern void func_0021F210__target(int, int, int, int, int) asm("func_0021F210");
extern int func_0021F3F8__target(void) asm("func_0021F3F8");
extern void func_0021F8E0__target(void) asm("func_0021F8E0");
extern int func_002201B0__target(int) asm("func_002201B0");
extern void func_002205B8__target(int) asm("func_002205B8");
extern void func_00221F20__target(char *) asm("func_00221F20");
extern void func_0022DE40__target(void) asm("func_0022DE40");
extern void func_0022E3D0__target(void) asm("func_0022E3D0");
extern void func_0022FC68__target(void) asm("func_0022FC68");
extern void func_0022FDA0__target(void) asm("func_0022FDA0");
extern void func_0022FE60__target(void) asm("func_0022FE60");
extern void func_00230200__target(int, float, float, float, float, float) asm("func_00230200");
extern void func_00238B40__target(void) asm("func_00238B40");
extern void func_00238BD8__target(void) asm("func_00238BD8");
extern void func_00238C00__target(int *) asm("func_00238C00");
extern void func_00238D98__target(int *) asm("func_00238D98");
extern int func_00238F48__target(void) asm("func_00238F48");
extern void func_0023CB00__target(int) asm("func_0023CB00");
extern char *func_0023D040__target(int) asm("func_0023D040");

#if defined(__mips__)
extern void func_0023D4F8__target(long, long, long, int) asm("func_0023D4F8");
#else

#endif

extern int func_0023D5F0__target(int) asm("func_0023D5F0");
extern void func_0023D7C0__target(float, float, float) asm("func_0023D7C0");
extern void func_002433A8__target(int, int) asm("func_002433A8");
extern int func_00243CE8__target(int) asm("func_00243CE8");
extern void func_0025A960__target(int) asm("func_0025A960");

/* Tables of string pointers. */
extern char *D_00275710__target[] asm("D_00275710"), *D_00275728__target[] asm("D_00275728"), *D_00275798__target[] asm("D_00275798"), *D_002757D8__target[] asm("D_002757D8");
extern char *D_00275818__target[] asm("D_00275818"), *D_00275858__target[] asm("D_00275858"), *D_00275870__target[] asm("D_00275870"), *D_002758D8__target[] asm("D_002758D8");
extern char *D_002758E8__target[] asm("D_002758E8"), *D_00275908__target[] asm("D_00275908"), *D_00275928__target[] asm("D_00275928"), *D_00275938__target[] asm("D_00275938");
extern char *D_00275960__target[] asm("D_00275960"), *D_00275970__target[] asm("D_00275970"), *D_002759A0__target[] asm("D_002759A0"), *D_002759B0__target[] asm("D_002759B0");
extern char *D_002759D0__target[] asm("D_002759D0"), *D_002759E0__target[] asm("D_002759E0"), *D_002759F0__target[] asm("D_002759F0"), *D_00275A08__target[] asm("D_00275A08");
extern char *D_00275A18__target[] asm("D_00275A18"), *D_00275A30__target[] asm("D_00275A30"), *D_00275A50__target[] asm("D_00275A50"), *D_00275A60__target[] asm("D_00275A60");
extern char *D_00275A70__target[] asm("D_00275A70"), *D_00275A80__target[] asm("D_00275A80"), *D_00275A90__target[] asm("D_00275A90"), *D_00275AA0__target[] asm("D_00275AA0");
extern char *D_00275AB0__target[] asm("D_00275AB0"), *D_00275AC0__target[] asm("D_00275AC0"), *D_00275AD0__target[] asm("D_00275AD0"), *D_00275AE0__target[] asm("D_00275AE0");
extern char *D_00275AF0__target[] asm("D_00275AF0"), *D_00275B00__target[] asm("D_00275B00"), *D_00275B10__target[] asm("D_00275B10"), *D_00275B20__target[] asm("D_00275B20");
extern char *D_00275B30__target[] asm("D_00275B30"), *D_00275B40__target[] asm("D_00275B40"), *D_00275B50__target[] asm("D_00275B50"), *D_00275B60__target[] asm("D_00275B60");

extern char **D_0029E038__target asm("D_0029E038"), **D_0029E03C__target asm("D_0029E03C"), **D_0029E040__target asm("D_0029E040"), **D_0029E044__target asm("D_0029E044");
extern char **D_0029E048__target asm("D_0029E048"), **D_0029E04C__target asm("D_0029E04C"), **D_0029E050__target asm("D_0029E050"), **D_0029E054__target asm("D_0029E054");
extern char **D_0029E058__target asm("D_0029E058"), **D_0029E05C__target asm("D_0029E05C"), **D_0029E060__target asm("D_0029E060"), **D_0029E064__target asm("D_0029E064");
extern char **D_0029E068__target asm("D_0029E068"), **D_0029E06C__target asm("D_0029E06C"), **D_0029E070__target asm("D_0029E070"), **D_0029E074__target asm("D_0029E074");
extern char **D_0029E078__target asm("D_0029E078"), **D_0029E07C__target asm("D_0029E07C"), **D_0029E080__target asm("D_0029E080"), **D_0029E084__target asm("D_0029E084");
extern char **D_0029E088__target asm("D_0029E088"), **D_0029E08C__target asm("D_0029E08C"), **D_0029E090__target asm("D_0029E090"), **D_0029E094__target asm("D_0029E094");
extern char **D_0029E098__target asm("D_0029E098"), **D_0029E09C__target asm("D_0029E09C"), **D_0029E0A0__target asm("D_0029E0A0"), **D_0029E0A4__target asm("D_0029E0A4");
extern char **D_0029E0A8__target asm("D_0029E0A8"), **D_0029E0AC__target asm("D_0029E0AC"), **D_0029E0B0__target asm("D_0029E0B0"), **D_0029E0B4__target asm("D_0029E0B4");
extern char **D_0029E0B8__target asm("D_0029E0B8"), **D_0029E0BC__target asm("D_0029E0BC"), **D_0029E0C0__target asm("D_0029E0C0"), **D_0029E0C4__target asm("D_0029E0C4");
extern char **D_0029E0C8__target asm("D_0029E0C8"), **D_0029E0CC__target asm("D_0029E0CC"), **D_0029E0D0__target asm("D_0029E0D0"), **D_0029E0D4__target asm("D_0029E0D4");
extern char **D_0029E0D8__target asm("D_0029E0D8"), **D_0029E0DC__target asm("D_0029E0DC");

extern float D_00275408__target[] asm("D_00275408"), D_00275418__target[] asm("D_00275418"), D_00275428__target[] asm("D_00275428"), D_00275438__target[] asm("D_00275438"), D_00275448__target[] asm("D_00275448");

extern int D_0029DAFC__target asm("D_0029DAFC");
extern int D_0029DE9C__target asm("D_0029DE9C");
extern float D_0029DEB8__target asm("D_0029DEB8"), D_0029DEBC__target asm("D_0029DEBC"), D_0029DEC0__target asm("D_0029DEC0"), D_0029DEC4__target asm("D_0029DEC4");
extern int D_0029DEC8__target asm("D_0029DEC8"), D_0029DECC__target asm("D_0029DECC"), D_0029DED0__target asm("D_0029DED0");
extern int D_0029DED8__target asm("D_0029DED8");
extern int D_0029DF68__target asm("D_0029DF68");
extern unsigned char D_0029DFB4__target asm("D_0029DFB4");
extern unsigned int D_0029E10C__target asm("D_0029E10C");
extern int D_0029E110__target asm("D_0029E110");
extern char D_00398C40__target[] asm("D_00398C40");
extern char D_004B1F40__target[] asm("D_004B1F40");
extern State D_004B25A0__target asm("D_004B25A0");
extern char D_004B2914__target[] asm("D_004B2914");
extern char D_004B2A90__target[] asm("D_004B2A90");
extern char D_004C6370__target[] asm("D_004C6370");
extern int D_004C6770__target[] asm("D_004C6770");
extern int D_004C6910__target[] asm("D_004C6910");
extern char D_00513B00__target[] asm("D_00513B00");
extern char D_00759780__target[] asm("D_00759780");

static inline void func_inline_5350(char *base)
{
    func_001BC4F0__target(base + 0x5350);
}

int func_001EB4D0(void)
{
    char *screen;
    int i;
    int frame;
    int buf;

    D_004B25A0__target.xB174 = 0;
    D_004B25A0__target.xB178 = 0;
    screen = func_0023D040__target(0);
    D_0029DED8__target = 0;
    D_0029E10C__target = 0;
    D_0029E110__target = func_00243CE8__target(0) == 0;
    func_0022FC68__target();
    func_0022FDA0__target();
    func_00238BD8__target();
    func_001F6AB8__target();
    CallScalarBridge0023D4F8_28_func_0023D4F8__target((*(long *)&D_0029D808), (*(long *)&D_0029D810), (*(long *)&D_0029D818), 0);
    func_0020ECD0(D_0029DEC8__target, D_0029DECC__target, D_0029DED0__target, D_0029DEB8__target, D_0029DEBC__target, D_0029DEC0__target, D_0029DEC4__target);
    D_0029D7EC = 1;
    func_001EC398__target(screen);
    func_00221F20__target(screen + 0x40);
    D_004C6770__target[0] = 0;
    func_0023D7C0__target(0.0f, 0.0f, 0.0f);

    D_0029E08C__target = D_00275A08__target;
    D_0029E090__target = D_00275A18__target;
    D_0029E07C__target = D_002759B0__target;
    D_0029E05C__target = D_002758E8__target;
    D_0029E074__target = (char **)&D_0029D8B0;
    D_0029E094__target = D_00275A30__target;
    D_0029E09C__target = D_00275A60__target;
    D_0029E0A0__target = D_00275A70__target;
    D_0029E0A4__target = D_00275A80__target;
    D_0029E0A8__target = D_00275A90__target;
    D_0029E0AC__target = D_00275AA0__target;
    D_0029E098__target = D_00275A50__target;
    D_0029E0B0__target = D_00275AB0__target;
    D_0029E0B4__target = D_00275AC0__target;
    D_0029E0B8__target = D_00275AD0__target;
    D_0029E0BC__target = D_00275AE0__target;
    D_0029E0C0__target = D_00275AF0__target;
    D_0029E0C4__target = D_00275B00__target;
    D_0029E0C8__target = D_00275B10__target;
    D_0029E0CC__target = D_00275B20__target;
    D_0029E0D0__target = D_00275B30__target;
    D_0029E0D4__target = D_00275B40__target;
    D_0029E050__target = D_00275870__target;
    D_0029E04C__target = D_00275858__target;
    D_0029E0DC__target = D_00275B60__target;
    D_0029E054__target = D_002758D8__target;
    D_0029E064__target = D_00275928__target;
    D_0029E060__target = D_00275908__target;
    D_0029E038__target = D_00275710__target;
    D_0029E058__target = (char **)D_0029D888;
    D_0029E084__target = D_002759E0__target;
    D_0029E070__target = D_00275970__target;
    D_0029E078__target = D_002759A0__target;
    D_0029E03C__target = D_00275728__target;
    D_0029E040__target = D_00275798__target;
    D_0029E044__target = D_002757D8__target;
    D_0029E088__target = D_002759F0__target;
    D_0029E080__target = D_002759D0__target;
    D_0029E0D8__target = D_00275B50__target;
    D_0029E068__target = D_00275938__target;
    D_0029E06C__target = D_00275960__target;
    D_0029E048__target = D_00275818__target;

    for (i = 0; i < 3; i++) {
        func_00230200__target(i, D_00275408__target[i], D_00275418__target[i], D_00275428__target[i], D_00275438__target[i], D_00275448__target[i]);
    }

    D_0029DE9C__target = func_00238F48__target();
    D_004B25A0__target.x10C = 2;
    func_001F5100__target();
    func_00203590((struct AnimationHandle *)(void *)(D_004C6370__target), 0x2330, 0, 1);
    func_00203B68__target(D_004C6370__target, 1, 1.0f);
    func_002038E0__target(D_004C6370__target, 0);
    {
        Fade fade;
        int handle;

        func_001F50F0__target(0x2330, 1);
        *(u128 *)&fade = 0; fade.x8 = 3.0f;
        func_0021D7E8__target();
    func_0021E428__target(0);
    func_0021E438__target(12.5f);
    func_0021E240__target();
    handle = func_0021D638__target(D_004B25A0__target.x1710[D_004C6910__target[D_0029DF68__target] + 3].x0, 1.0f);
    func_0021D6F8__target(handle, &fade);
    func_0021D748__target(handle, 240.0f);
    func_001F5578__target();
    func_001F7A60__target(0x80);
    func_001F79D8__target(0, 0x78);
    }

    while (!D_0029DFB4__target) {
        if (func_0018DF28__target(0, 1) && D_0029DED8__target == 13) {
            func_001F57D0__target(0x2329, 0);
        }
        func_0018DD08__target(D_004B25A0__target.x288);
        func_0022FE60__target();
        if (D_0029E10C__target & 1) {
            buf = func_0023D5F0__target(0) + 0x10;
        } else {
            buf = func_0023D5F0__target(0) + 0x50;
        }
        func_00219EC8__target(buf);
        if (D_0029DAFC__target) {
            func_002205B8__target(0);
        }
        frame = func_0023D5F0__target(0);
        func_0025A960__target(0);
        func_002433A8__target(0, 0);
        if (D_0029E10C__target < 4) {
            func_0021F0A0__target(frame, ((unsigned char)D_0029E10C__target ^ 1) & 1, 1);
        } else {
            func_0021F1B8__target(frame, D_0029E10C__target & 1, 0, 1);
        }
        func_002433A8__target(0, 0);
        if (func_0021F3F8__target()) {
            func_0021F210__target(0, frame, D_0029E10C__target, D_0029E110__target, 1);
        } else {
            func_0021F210__target(0, frame, D_0029E10C__target, 0, 1);
        }
        func_0023CB00__target(0);
        func_0022E3D0__target();
        func_00215B68__target(0, 1, ((unsigned char)D_0029E10C__target ^ 1) & 1, D_0029E110__target);
        func_002102A8__target(D_00513B00__target, 1, ((unsigned char)D_0029E10C__target ^ 1) & 1);
        func_001F4E40__target();
        if (D_0029D800 == 1) {
            func_001F4DD8__target();
        }
        func_0021F8E0__target();
        if (D_0029DED8__target != 9) {
            func_001C5358__target(D_004B2A90__target, D_004C6910__target[D_0029DF68__target]);
        } else {
            func_001C5728__target(D_004B2A90__target);
        }
        func_001C5DF0__target(D_004B2914__target);
        func_002184D0__target(0, 0, 0);
        func_002065C0__target();
        func_00236C60((TimerPool *)(D_00759780__target));
        if (func_002201B0__target(0) || func_002201B0__target(1)) {
            D_0029DE9C__target = func_00238F48__target();
        }
        switch (D_0029DED8__target) {
        case 0:
            func_001F7B80__target();
            func_001ED3E0__target();
            func_001F57D0__target(0x2329, 1);
            if ((unsigned int)(func_00238F48__target() - D_0029DE9C__target) > 3600) {
                D_004B25A0__target.x10C = 10;
                D_0029DFB4__target = 1;
                func_001C5B80__target(D_004B25A0__target.x374);
                func_001C57A8__target(D_004B25A0__target.x4F0, ((u128)0x454D4147 << 64) | (u128)0x5F5245544E455F53LL);
            }
            break;
        case 15:
            func_001F7B80__target();
            func_001ED5B8__target();
            break;
        case 16:
            func_001F3B98__target();
            break;
        case 1:
            func_001ED900__target();
            break;
        case 24:
            func_001F7B80__target();
            func_001EDD28__target();
            break;
        case 10:
            func_001F7B80__target();
            func_001EED78__target();
            break;
        case 11:
            func_001EF0E0__target();
            break;
        case 12:
            func_001EFA78__target();
            break;
        case 17:
            func_001EFE28__target();
            break;
        case 2:
            func_001F7B80__target();
            func_001F0160__target();
            break;
        case 13:
            func_001F7B80__target();
            func_001F0A28__target();
            break;
        case 19:
            func_001F7B80__target();
            func_001F1168__target();
            break;
        case 20:
            func_001F14A0__target();
            break;
        case 32:
            func_001F1968__target();
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            func_001F24B8__target();
            break;
        case 33:
            func_001F1A90__target();
            break;
        case 34:
            func_001F2098__target();
            break;
        case 9:
            func_001F43C0__target();
            break;
        case 18:
            func_001F1500__target();
            break;
        case 21:
            func_001EF5D0__target();
            break;
        case 22:
            func_001EE2A0__target();
            break;
        case 25:
            func_001F7B80__target();
            func_001EE8A8__target();
            break;
        case 28:
            func_001F4960__target();
            break;
        case 30:
            func_001F4508__target();
            break;
        }
        WaitForRenderLegacy(D_004B1F40__target);
        func_002161B8__target(0, 1, ((unsigned char)D_0029E10C__target ^ 1) & 1, D_0029E110__target);
        func_002433A8__target(0, 0);
        func_0020F2A8__target(0);
        func_002433A8__target(0, 0);
        func_00210460__target(D_00513B00__target, 0, 1);
        func_0022DE40__target();
        D_0029E10C__target++;
        func_00238B40__target();
        func_00238C00__target(&D_0029E110__target);
        func_00238D98__target(&D_0029E110__target);
        func_inline_5350(D_00398C40__target);
    }
    return 1;
}

typedef union Qword {
    u128 q;
    struct {
        unsigned int lo;
        unsigned int hi;
        unsigned long d;
    } w;
    unsigned char b;
} Qword;

typedef struct Packet {
    Qword q0;
    Qword q1;
    Qword q2;
} Packet;


extern int D_0029DAFC__n6 asm("D_0029DAFC");
extern int D_0029DB00__n6 asm("D_0029DB00");
extern int D_0029DEE0__n6 asm("D_0029DEE0");
extern int D_0029DEE4__n6 asm("D_0029DEE4");
extern unsigned char D_0029DEE8__n6 asm("D_0029DEE8");
extern unsigned char D_0029DFB4__n6 asm("D_0029DFB4");
extern unsigned int D_0029E10C__n6 asm("D_0029E10C");
extern int D_0029E110__n6 asm("D_0029E110");
extern char D_004B25A0__n6[] asm("D_004B25A0");


#if defined(__mips__)
extern void func_0023D4F8__n6(int, int, int, int) asm("func_0023D4F8");
#else

#endif

extern void func_0023D7C0__n6(float, float, float) asm("func_0023D7C0");
extern int func_00243CE8__n6(int) asm("func_00243CE8");
extern int func_0023D5F0__n6(int) asm("func_0023D5F0");
extern void func_0025A960__n6(int) asm("func_0025A960");
extern int func_002433A8__n6(int, int) asm("func_002433A8");
extern void func_0021F0A0__n6(int, int, int) asm("func_0021F0A0");
extern void func_001B8CD8__n6(void *, int) asm("func_001B8CD8");
extern void func_00238A68__n6(int) asm("func_00238A68");
extern void func_0021F8E0__n6(void) asm("func_0021F8E0");
extern int func_00238F48__n6(void) asm("func_00238F48");
extern int func_00220618__n6(int, int) asm("func_00220618");
extern void func_00223C60__n6(unsigned char) asm("func_00223C60");
extern void func_001F4CE0__n6(void) asm("func_001F4CE0");
extern void func_0020BDA0__n6(int) asm("func_0020BDA0");
extern void func_00238B40__n6(void) asm("func_00238B40");
extern void func_0023D618__n6(int, int *, int *) asm("func_0023D618");
extern void func_002448B8__n6(int, short, short, short) asm("func_002448B8");
extern void func_00244B20__n6(int, short, short, short) asm("func_00244B20");
extern int func_00238C00__n6(int *) asm("func_00238C00");

int func_001EBFD8(int arg0)
{
    Packet pkt;
    int s0;
    int state = 2;
    int flag = 0;

    D_0029DFB4__n6 = 0;
    CallScalarBridge0023D4F8_28_func_0023D4F8__n6(0, 0, 0, 0);
    func_0023D7C0__n6(0.0f, 0.0f, 0.0f);
    D_0029DEE0__n6 = arg0;
    D_0029D7EC = 1;
    D_0029E110__n6 = !func_00243CE8__n6(0);
    state = !D_0029E110__n6 ? 3 : 2;
    s0 = func_0023D5F0__n6(0);
    func_0025A960__n6(0);
    func_002433A8__n6(0, 0);
    func_0021F0A0__n6(s0, 0, 1);
    func_002433A8__n6(0, 0);
    pkt.q0.q = 0;
    pkt.q1.q = 0;
    pkt.q2.q = 0;
    pkt.q0.w.lo = 0x70000002;
    pkt.q1.w.d = 0xE;
    pkt.q1.w.hi = 0x10000000;
    pkt.q1.w.lo = 0x8001;
    pkt.q2.w.d = 0x22;
    pkt.q2.b = state;
    D_0029E10C__n6 = 0;
    func_001B8CD8__n6(D_004B25A0__n6, 0);
    func_00238A68__n6(0x3C);
    while (D_0029DFB4__n6 == 0) {
        int x, y;
        func_0021F8E0__n6();
        if (D_0029DAFC__n6 != 0) {
            if (((unsigned int)(func_00238F48__n6() - D_0029DEE4__n6) >= 0x12D && D_0029DEE8__n6) || !D_0029DEE8__n6) {
                s0 = 0;
                if (func_00220618__n6(0x4000, 0) != 0 || func_00220618__n6(8, 0) != 0) {
                    s0 = 1;
                }
                flag = s0;
            }
            if (D_0029DAFC__n6 != 0) {
                D_0029D920 = 1;
            }
        }
        if (D_0029D920 != 0 && D_0029DB00__n6 != 0) {
            if (D_0029DEE0__n6 == 0) {
                D_0029D800 = 1;
                D_0029D920 = 0;
                D_0029DFB4__n6 = 1;
            } else {
                D_0029D920 = 0;
                D_0029D7EC = 1;
            }
        }
        s0 = func_0023D5F0__n6(0);
        pkt.q2.b = state;
        func_0025A960__n6(0);
        func_002433A8__n6(0, 0);
        *(volatile unsigned int *)0x1000A020 = 0;
        *(volatile unsigned int *)0x1000A030 = (unsigned int)&pkt & 0x7FFFFFFF;
        *(volatile unsigned int *)0x1000E010 = 4;
        *(volatile unsigned int *)0x1000A000 = 0x145;
        func_002433A8__n6(0, 0);
        func_001F4CE0__n6();
        if (D_0029DAFC__n6 != 0) {
            func_00223C60__n6(flag ^ 1);
            flag = 0;
        }
        func_0025A960__n6(0);
        func_002433A8__n6(0, 0);
        if (D_0029E10C__n6 >= 7) {
            func_0020BDA0__n6(2);
        }
        func_0021F0A0__n6(s0, 0, 1);
        func_002433A8__n6(0, 0);
        func_00238B40__n6();
        func_0023D618__n6(0, &x, &y);
        func_002448B8__n6((D_0029E10C__n6 & 1) ? s0 + 0x200 : s0 + 0x90, *(short *)&x,
                      *(short *)&y, *(short *)&D_0029E110__n6);
        {
            int dest = s0 + 0x110;
            if (D_0029E10C__n6 & 1) dest = s0 + 0x280;
            func_00244B20__n6(dest, *(short *)&x, *(short *)&y, *(short *)&D_0029E110__n6);
        }
        state = (state == 2) ? 3 : 2;
        D_0029E10C__n6++;
        func_00238C00__n6(&D_0029E110__n6);
    }
    func_002433A8__n6(0, 0);
    return 1;
}

/* ---- merged from src/unsorted/001EC398.c ---- */
extern "C" {
typedef struct Ref__c3 { int x0, x4, x8, xC; } Ref__c3;

typedef struct Pos__c3 { float x, y, z; char padC[0x24]; } Pos__c3;

typedef struct Entry__c3 { int x[10]; } Entry__c3;

typedef struct Panel__c3 { char *x0; char pad4[0x2C]; char *x30; char pad34[0x1AC]; } Panel__c3;

typedef struct Slot__c3 { char pad0[0x50]; } Slot__c3;

typedef struct Global__c3 {
    char pad0[0x4A];
    unsigned char x4A;
    char pad4B[0x288 - 0x4B];
    int x288;
    char pad28C[0x4F0 - 0x28C];
    char x4F0[0xA8];
    Slot__c3 slots[14];
    char pad9F8[0xA0C - 0x9F8];
    Entry__c3 entries[80];
    char pad168C[0x1E90 - 0x168C];
    Panel__c3 panels[14];
    char pad38D0[0xAC90 - 0x38D0];
    int counts[80];
} Global__c3;

typedef struct Screen__c3 { char pad0[0xA0]; int xA0, xA4, xA8; } Screen__c3;

typedef struct Sound__c3 { int *x0; int x4; } Sound__c3;

extern void func_001AC620__c3(Sound__c3 *, Ref__c3 *) asm("func_001AC620");
extern void func_001AC680__c3(Sound__c3 *, float) asm("func_001AC680");
extern void func_001C3488__c3(Slot__c3 *, char *, char *, Ref__c3 *, Ref__c3 *, int) asm("func_001C3488");
extern void func_001C5038__c3(char *, Ref__c3 *) asm("func_001C5038");
extern void func_001F57D0__c3(int, int) asm("func_001F57D0");
extern void func_001F5B08__c3(int) asm("func_001F5B08");
extern void func_001F60C8__c3(int) asm("func_001F60C8");
extern void func_001F65E0__c3(int) asm("func_001F65E0");
extern void func_001F6798__c3(int) asm("func_001F6798");
extern void func_001F67F0__c3(int) asm("func_001F67F0");
extern void func_002038E0__c3(Ref__c3 *, int) asm("func_002038E0");
extern void func_00203B68__c3(Ref__c3 *, int, float) asm("func_00203B68");
extern void func_00203E10__c3(Ref__c3 *, float) asm("func_00203E10");
extern void func_00207D90__c3(Pos__c3 *, Ref__c3 *) asm("func_00207D90");
extern void func_00218498__c3(int, int) asm("func_00218498");
extern void func_00254D08__c3(const char *, int, int) asm("func_00254D08");

extern Ref__c3 *D_00275B70__c3[] asm("D_00275B70");
extern Ref__c3 *D_00275BB0__c3[] asm("D_00275BB0");
extern int D_00275BF0__c3[] asm("D_00275BF0");
extern int D_00275C30__c3[] asm("D_00275C30");
extern int D_0029DF68__c3 asm("D_0029DF68");
extern Global__c3 D_004B25A0__c3 asm("D_004B25A0");
extern Sound__c3 D_004B25EC__c3[] asm("D_004B25EC");
extern Sound__c3 D_004B264C__c3[] asm("D_004B264C");
extern Pos__c3 D_004C5E50__c3 asm("D_004C5E50"), D_004C5E80__c3 asm("D_004C5E80"), D_004C5EB0__c3 asm("D_004C5EB0"), D_004C5EE0__c3 asm("D_004C5EE0"), D_004C5F10__c3 asm("D_004C5F10"), D_004C5F40__c3 asm("D_004C5F40");
extern Pos__c3 D_004C5F70__c3 asm("D_004C5F70"), D_004C5FA0__c3 asm("D_004C5FA0"), D_004C5FD0__c3 asm("D_004C5FD0"), D_004C6000__c3 asm("D_004C6000"), D_004C6030__c3 asm("D_004C6030"), D_004C6060__c3 asm("D_004C6060");
extern Pos__c3 D_004C6090__c3 asm("D_004C6090"), D_004C60C0__c3 asm("D_004C60C0"), D_004C60F0__c3 asm("D_004C60F0"), D_004C6120__c3 asm("D_004C6120"), D_004C6150__c3 asm("D_004C6150"), D_004C6180__c3 asm("D_004C6180");
extern Pos__c3 D_004C61B0__c3 asm("D_004C61B0"), D_004C61E0__c3 asm("D_004C61E0"), D_004C6210__c3 asm("D_004C6210"), D_004C6240__c3 asm("D_004C6240"), D_004C6270__c3 asm("D_004C6270"), D_004C62A0__c3 asm("D_004C62A0");
extern Pos__c3 D_004C62D0__c3 asm("D_004C62D0"), D_004C6300__c3 asm("D_004C6300"), D_004C6330__c3 asm("D_004C6330");
extern Ref__c3 D_004C6380__c3 asm("D_004C6380"), D_004C6390__c3 asm("D_004C6390"), D_004C63A0__c3 asm("D_004C63A0"), D_004C63B0__c3 asm("D_004C63B0"), D_004C63C0__c3 asm("D_004C63C0"), D_004C63D0__c3 asm("D_004C63D0");
extern Ref__c3 D_004C63E0__c3 asm("D_004C63E0"), D_004C63F0__c3 asm("D_004C63F0"), D_004C6400__c3 asm("D_004C6400"), D_004C6410__c3 asm("D_004C6410"), D_004C6420__c3 asm("D_004C6420"), D_004C6430__c3 asm("D_004C6430");
extern Ref__c3 D_004C6440__c3 asm("D_004C6440"), D_004C6450__c3 asm("D_004C6450"), D_004C6460__c3 asm("D_004C6460"), D_004C6470__c3 asm("D_004C6470"), D_004C6480__c3 asm("D_004C6480"), D_004C6490__c3 asm("D_004C6490");
extern Ref__c3 D_004C64A0__c3 asm("D_004C64A0"), D_004C64B0__c3 asm("D_004C64B0"), D_004C64C0__c3 asm("D_004C64C0"), D_004C64D0__c3 asm("D_004C64D0"), D_004C64E0__c3 asm("D_004C64E0"), D_004C64F0__c3 asm("D_004C64F0");
extern Ref__c3 D_004C6500__c3 asm("D_004C6500"), D_004C6510__c3 asm("D_004C6510"), D_004C6520__c3 asm("D_004C6520");
extern int D_004C67B8__c3[] asm("D_004C67B8");
extern int D_004C6910__c3[] asm("D_004C6910");
extern int D_004C6B5C__c3[] asm("D_004C6B5C");

static inline int soundHasData(int *p)
{
    return p[1] != 0;
}

static inline char soundIsLoaded(Sound__c3 *s)
{
    /* 64-bit: the int result is copied (not merged) into ok, as in retail */
    long ok = 0;

    if (s->x0 != 0 && s->x0[0] != 0) {
        ok = soundHasData(s->x0);
    }
    return ok;
}

static inline unsigned char animIsValid(Ref__c3 *r)
{
    return r->x0 != 0;
}

extern void func_001EC398__c3(Screen__c3 *screen) asm("func_001EC398");
void func_001EC398__c3(Screen__c3 *screen)
{
    Ref__c3 tmp;
    int i, j;
    int *loIds;
    float zero;

    zero = 0.0f;
    D_004B25A0__c3.x288 = 1;
    D_004C6B5C__c3[0] = 0;
    func_001F60C8__c3(0);
    screen->xA0 = 0;
    screen->xA4 = 0;
    screen->xA8 = 0;
    for (i = 0; i < 80; i++) {
        for (j = 1; j <= D_004B25A0__c3.counts[i]; j++) {
            func_00218498__c3(D_004B25A0__c3.entries[i].x[j], 0);
        }
    }
    zero = 0.0f;
    func_001F57D0__c3(0x232E, 1);
    func_001F57D0__c3(0x2329, 1);
    func_001F57D0__c3(0x232B, 0);
    func_001F67F0__c3(0);
    func_001F6798__c3(0);
    func_001F5B08__c3(0);

    func_00203590((struct AnimationHandle *)(void *)(&D_004C6380__c3), D_00275BF0__c3[1], 0, 1);
    func_00207D90__c3(&D_004C5E50__c3, &D_004C6380__c3);
    D_004C5E50__c3.x = zero; D_004C5E50__c3.y = zero; D_004C5E50__c3.z = 3.239f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6390__c3), D_00275C30__c3[1], 0, 0);
    func_00207D90__c3(&D_004C5E80__c3, &D_004C6390__c3);
    D_004C5E80__c3.x = zero; D_004C5E80__c3.y = zero; D_004C5E80__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C63A0__c3), D_00275BF0__c3[2], 0, 1);
    func_00207D90__c3(&D_004C5EB0__c3, &D_004C63A0__c3);
    D_004C5EB0__c3.x = zero; D_004C5EB0__c3.y = zero; D_004C5EB0__c3.z = 2.767f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C63B0__c3), D_00275C30__c3[2], 0, 0);
    func_00207D90__c3(&D_004C5EE0__c3, &D_004C63B0__c3);
    D_004C5EE0__c3.x = zero; D_004C5EE0__c3.y = zero; D_004C5EE0__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C63C0__c3), D_00275BF0__c3[3], 0, 1);
    func_00207D90__c3(&D_004C5F10__c3, &D_004C63C0__c3);
    D_004C5F10__c3.x = zero; D_004C5F10__c3.y = zero; D_004C5F10__c3.z = 3.51f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C63D0__c3), D_00275C30__c3[3], 0, 0);
    func_00207D90__c3(&D_004C5F40__c3, &D_004C63D0__c3);
    D_004C5F40__c3.x = zero; D_004C5F40__c3.y = zero; D_004C5F40__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C63E0__c3), D_00275BF0__c3[4], 0, 1);
    func_00207D90__c3(&D_004C5FD0__c3, &D_004C63E0__c3);
    D_004C5FD0__c3.x = zero; D_004C5FD0__c3.y = zero; D_004C5FD0__c3.z = 3.443f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C63F0__c3), D_00275C30__c3[4], 0, 0);
    func_00207D90__c3(&D_004C6000__c3, &D_004C63F0__c3);
    D_004C6000__c3.x = zero; D_004C6000__c3.y = zero; D_004C6000__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6400__c3), D_00275BF0__c3[5], 0, 1);
    func_00207D90__c3(&D_004C5F70__c3, &D_004C6400__c3);
    D_004C5F70__c3.x = zero; D_004C5F70__c3.y = zero; D_004C5F70__c3.z = 3.6f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6410__c3), D_00275C30__c3[5], 0, 0);
    func_00207D90__c3(&D_004C5FA0__c3, &D_004C6410__c3);
    D_004C5FA0__c3.x = zero; D_004C5FA0__c3.y = zero; D_004C5FA0__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6420__c3), D_00275BF0__c3[6], 0, 1);
    func_00207D90__c3(&D_004C6030__c3, &D_004C6420__c3);
    D_004C6030__c3.x = zero; D_004C6030__c3.y = zero; D_004C6030__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6430__c3), D_00275C30__c3[6], 0, 0);
    func_00207D90__c3(&D_004C6060__c3, &D_004C6430__c3);
    D_004C6060__c3.x = zero; D_004C6060__c3.y = zero; D_004C6060__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6440__c3), D_00275BF0__c3[7], 0, 1);
    func_00207D90__c3(&D_004C6270__c3, &D_004C6440__c3);
    D_004C6270__c3.x = zero; D_004C6270__c3.y = zero; D_004C6270__c3.z = 3.468f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6450__c3), D_00275C30__c3[7], 0, 0);
    func_00207D90__c3(&D_004C62A0__c3, &D_004C6450__c3);
    D_004C62A0__c3.x = zero; D_004C62A0__c3.y = zero; D_004C62A0__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6460__c3), D_00275BF0__c3[8], 0, 1);
    func_00207D90__c3(&D_004C6090__c3, &D_004C6460__c3);
    D_004C6090__c3.x = zero; D_004C6090__c3.y = zero; D_004C6090__c3.z = 3.15f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6470__c3), D_00275C30__c3[8], 0, 0);
    func_00207D90__c3(&D_004C60C0__c3, &D_004C6470__c3);
    D_004C60C0__c3.x = zero; D_004C60C0__c3.y = zero; D_004C60C0__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6480__c3), D_00275BF0__c3[9], 0, 1);
    func_00207D90__c3(&D_004C60F0__c3, &D_004C6480__c3);
    D_004C60F0__c3.x = zero; D_004C60F0__c3.y = zero; D_004C60F0__c3.z = 3.157f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6490__c3), D_00275C30__c3[9], 0, 0);
    func_00207D90__c3(&D_004C6120__c3, &D_004C6490__c3);
    D_004C6120__c3.x = zero; D_004C6120__c3.y = zero; D_004C6120__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C64A0__c3), D_00275BF0__c3[10], 0, 1);
    func_00207D90__c3(&D_004C6150__c3, &D_004C64A0__c3);
    D_004C6150__c3.x = zero; D_004C6150__c3.y = zero; D_004C6150__c3.z = 2.993f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C64B0__c3), D_00275C30__c3[10], 0, 0);
    func_00207D90__c3(&D_004C6180__c3, &D_004C64B0__c3);
    D_004C6180__c3.x = zero; D_004C6180__c3.y = zero; D_004C6180__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6500__c3), D_00275BF0__c3[11], 0, 1);
    func_00207D90__c3(&D_004C62D0__c3, &D_004C6500__c3);
    D_004C62D0__c3.x = zero; D_004C62D0__c3.y = zero; D_004C62D0__c3.z = 2.928f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6510__c3), D_00275C30__c3[11], 0, 0);
    func_00207D90__c3(&D_004C6300__c3, &D_004C6510__c3);
    D_004C6300__c3.x = zero; D_004C6300__c3.y = zero; D_004C6300__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C64C0__c3), D_00275BF0__c3[12], 0, 1);
    func_00207D90__c3(&D_004C61B0__c3, &D_004C64C0__c3);
    D_004C61B0__c3.x = zero; D_004C61B0__c3.y = zero; D_004C61B0__c3.z = 3.077f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C64D0__c3), D_00275C30__c3[12], 0, 0);
    func_00207D90__c3(&D_004C61E0__c3, &D_004C64D0__c3);
    D_004C61E0__c3.x = zero; D_004C61E0__c3.y = zero; D_004C61E0__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C64E0__c3), D_00275BF0__c3[13], 0, 1);
    func_00207D90__c3(&D_004C6210__c3, &D_004C64E0__c3);
    D_004C6210__c3.x = zero; D_004C6210__c3.y = zero; D_004C6210__c3.z = 3.164f;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C64F0__c3), D_00275C30__c3[13], 0, 0);
    func_00207D90__c3(&D_004C6240__c3, &D_004C64F0__c3);
    D_004C6240__c3.x = zero; D_004C6240__c3.y = zero; D_004C6240__c3.z = zero;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6520__c3), D_00275BF0__c3[14], 0, 1);
    func_00207D90__c3(&D_004C6330__c3, &D_004C6520__c3);
    D_004C6330__c3.x = zero; D_004C6330__c3.y = -1.819f; D_004C6330__c3.z = 2.458f;

    for (i = 1; i < 15; i++) {
        func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c3[i]), D_00275BF0__c3[i], 0, 0);
        if (!animIsValid(D_00275B70__c3[i])) {
            func_00254D08__c3("uiScreenInit: Animation is not valid! hi:%d,%d\n", D_00275BF0__c3[i], 0);
        }
        /* assigned in the loop and used after it: keeps the base in a register
           and C30[i] unreduced, as in retail */
        loIds = D_00275C30__c3;
        func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c3[i]), loIds[i], 0, 0);
        if (!animIsValid(D_00275BB0__c3[i])) {
            func_00254D08__c3("uiScreenInit: Animation is not valid! lo:%d,%d\n", loIds[i], 0);
        }
        tmp = *D_00275BB0__c3[i];
        func_00203E10__c3(&tmp, 1.0f);
        tmp = *D_00275BB0__c3[i];
        func_002038E0__c3(&tmp, 1);
    }
    func_001F65E0__c3(0);

    func_001C5038__c3(D_004B25A0__c3.x4F0, D_00275B70__c3[1]);
    func_001C3488__c3(&D_004B25A0__c3.slots[0], D_004B25A0__c3.panels[0].x0 + 0x10, D_004B25A0__c3.panels[0].x30 + 0x10, D_00275B70__c3[1], D_00275BB0__c3[1], 1);
    func_001C3488__c3(&D_004B25A0__c3.slots[1], D_004B25A0__c3.panels[1].x0 + 0x10, D_004B25A0__c3.panels[1].x30 + 0x10, D_00275B70__c3[2], D_00275BB0__c3[2], 2);
    func_001C3488__c3(&D_004B25A0__c3.slots[2], D_004B25A0__c3.panels[2].x0 + 0x10, D_004B25A0__c3.panels[2].x30 + 0x10, D_00275B70__c3[3], D_00275BB0__c3[3], 3);
    func_001C3488__c3(&D_004B25A0__c3.slots[3], D_004B25A0__c3.panels[3].x0 + 0x10, D_004B25A0__c3.panels[3].x30 + 0x10, D_00275B70__c3[4], D_00275BB0__c3[4], 4);
    func_001C3488__c3(&D_004B25A0__c3.slots[4], D_004B25A0__c3.panels[4].x0 + 0x10, D_004B25A0__c3.panels[4].x30 + 0x10, D_00275B70__c3[5], D_00275BB0__c3[5], 5);
    func_001C3488__c3(&D_004B25A0__c3.slots[5], D_004B25A0__c3.panels[5].x0 + 0x10, D_004B25A0__c3.panels[5].x30 + 0x10, D_00275B70__c3[6], D_00275BB0__c3[6], 6);
    func_001C3488__c3(&D_004B25A0__c3.slots[6], D_004B25A0__c3.panels[6].x0 + 0x10, D_004B25A0__c3.panels[6].x30 + 0x10, D_00275B70__c3[7], D_00275BB0__c3[7], 7);
    func_001C3488__c3(&D_004B25A0__c3.slots[7], D_004B25A0__c3.panels[7].x0 + 0x10, D_004B25A0__c3.panels[7].x30 + 0x10, D_00275B70__c3[8], D_00275BB0__c3[8], 8);
    func_001C3488__c3(&D_004B25A0__c3.slots[8], D_004B25A0__c3.panels[8].x0 + 0x10, D_004B25A0__c3.panels[8].x30 + 0x10, D_00275B70__c3[9], D_00275BB0__c3[9], 9);
    func_001C3488__c3(&D_004B25A0__c3.slots[9], D_004B25A0__c3.panels[9].x0 + 0x10, D_004B25A0__c3.panels[9].x30 + 0x10, D_00275B70__c3[10], D_00275BB0__c3[10], 10);
    func_001C3488__c3(&D_004B25A0__c3.slots[11], D_004B25A0__c3.panels[10].x0 + 0x10, D_004B25A0__c3.panels[10].x30 + 0x10, D_00275B70__c3[12], D_00275BB0__c3[12], 12);
    func_001C3488__c3(&D_004B25A0__c3.slots[12], D_004B25A0__c3.panels[11].x0 + 0x10, D_004B25A0__c3.panels[11].x30 + 0x10, D_00275B70__c3[13], D_00275BB0__c3[13], 13);
    func_001C3488__c3(&D_004B25A0__c3.slots[10], D_004B25A0__c3.panels[12].x0 + 0x10, D_004B25A0__c3.panels[12].x30 + 0x10, D_00275B70__c3[11], D_00275BB0__c3[11], 11);
    func_001C3488__c3(&D_004B25A0__c3.slots[13], D_004B25A0__c3.panels[13].x0 + 0x10, D_004B25A0__c3.panels[13].x30 + 0x10, D_00275B70__c3[14], D_00275BB0__c3[14], 14);

    if (D_004B25A0__c3.x4A == 0 || D_004C67B8__c3[0] == 1) {
        func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c3[14]), loIds[14], 0, 0);
        tmp = *D_00275BB0__c3[14];
        func_00203E10__c3(&tmp, 1.0f);
        tmp = *D_00275BB0__c3[14];
        func_002038E0__c3(&tmp, 1);
        D_004B25A0__c3.panels[13].x30[4] = 1;
    } else {
        func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c3[14]), loIds[14], 0, 5);
        tmp = *D_00275BB0__c3[14];
        func_00203E10__c3(&tmp, 1.0f);
        tmp = *D_00275BB0__c3[14];
        func_00203B68__c3(&tmp, 1, 0.995f);
        tmp = *D_00275BB0__c3[14];
        func_002038E0__c3(&tmp, 0);
        D_004B25A0__c3.panels[13].x30[4] = 1;
    }

    func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c3[D_004C6910__c3[D_0029DF68__c3]]), 0x23D4, 0, 0x32);
    func_001AC620__c3(D_004B25EC__c3, D_00275B70__c3[D_004C6910__c3[D_0029DF68__c3]]);
    if (soundIsLoaded(D_004B25EC__c3)) {
        func_001AC680__c3(D_004B25EC__c3, 0.0f);
    }
    func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c3[D_004C6910__c3[D_0029DF68__c3]]), 0x23D5, 0, 0x32);
    func_001AC620__c3(D_004B264C__c3, D_00275BB0__c3[D_004C6910__c3[D_0029DF68__c3]]);
    if (soundIsLoaded(D_004B264C__c3)) {
        func_001AC680__c3(D_004B264C__c3, 0.0f);
    }
}
}
/* ---- merged from src/unsorted/001ED3E0.c ---- */
extern "C" {
struct State__c4 { char pad[0x18]; int x18; };
extern struct State__c4 D_004C6E28__c4 asm("D_004C6E28");
extern char D_004B2A90__c4[] asm("D_004B2A90");
extern float D_0029E008__c4 asm("D_0029E008"), D_0029E00C__c4 asm("D_0029E00C"), D_0029E010__c4 asm("D_0029E010"), D_0029E014__c4 asm("D_0029E014"), D_0029E018__c4 asm("D_0029E018");
extern int D_0029DED8__c4 asm("D_0029DED8"), D_0029DF40__c4 asm("D_0029DF40"), D_0029DF44__c4 asm("D_0029DF44"), D_0029DF7C__c4 asm("D_0029DF7C");
extern unsigned char D_0029DF8B__c4 asm("D_0029DF8B");
extern void func_001F5B08__c4(int) asm("func_001F5B08");
extern void func_001F7420__c4(void *,int,int,int,int,int,char *,int) asm("func_001F7420");
extern void func_001F74F0__c4(void *) asm("func_001F74F0");
extern void func_0020F518__c4(int,int,int,int,int) asm("func_0020F518");
extern void func_0020FB80__c4(int,int,int,int,int) asm("func_0020FB80");
extern void func_0020F9F0__c4(int,int,int,int,char *) asm("func_0020F9F0");
extern void func_001F57D0__c4(int,int) asm("func_001F57D0");
extern void func_001F4A98__c4(void) asm("func_001F4A98");
extern void func_001F6940__c4(void) asm("func_001F6940");
extern void func_001F76B8__c4(void *) asm("func_001F76B8");
extern void func_001C5818__c4(void *) asm("func_001C5818");
extern void func_001ED3E0__c4(void) asm("func_001ED3E0");
void func_001ED3E0__c4(void) {
    char value = 0;
    int color;
    func_001F5B08__c4(0);
    func_001F7420__c4(&D_004C6E28__c4,15,0,0,0,0,&value,1);
    func_001F74F0__c4(&D_004C6E28__c4);
    if (D_0029E008__c4 >= 400.0f || D_0029E008__c4 <= 352.0f)
        D_0029E018__c4 = -D_0029E018__c4;
    D_0029E008__c4 += D_0029E018__c4;
    D_0029E00C__c4 += D_0029E018__c4;
    color = (int)(D_0029E018__c4 * 10.0f);
    func_0020F518__c4(0,color+116,color+116,color+136,127);
    func_0020FB80__c4(0,(int)D_0029E008__c4,(int)D_0029E00C__c4,(int)D_0029E010__c4,(int)D_0029E014__c4);
    func_0020F9F0__c4(0,0,320,199,"PRESS \255 TO START");
    func_001F57D0__c4(9001,1);
    D_0029DF7C__c4 = 0;
    func_001F4A98__c4();
    if (D_0029DF8B__c4) {
        PlayMenuSelectLegacy(&D_004C6E28__c4);
        D_0029DF40__c4 = 210;
        D_0029DED8__c4 = D_004C6E28__c4.x18;
        D_0029DF44__c4 = 0;
        func_001F6940__c4();
        func_001F76B8__c4(&D_004C6E28__c4);
    }
    func_001C5818__c4(D_004B2A90__c4);
}
}
/* func_001ED5B8: options menu with 3 rows (x1C: per-row disabled flags) */
extern "C" {
typedef unsigned char u8__cED5B8;
typedef struct Menu__cED5B8 { int x0, x4, x8, xC, x10, x14, x18; u8__cED5B8 x1C[16]; } Menu__cED5B8;
typedef struct Game__cED5B8 { char pad0[0x288]; int x288; char pad28C[0x264]; char x4F0[4]; } Game__cED5B8;
extern Menu__cED5B8 D_004C6CA8__cED5B8 asm("D_004C6CA8");
extern Game__cED5B8 D_004B25A0__cED5B8 asm("D_004B25A0");
extern char D_004C6370__cED5B8[] asm("D_004C6370");
extern int *D_0029E054__cED5B8 asm("D_0029E054");
extern int D_0029DF7C__cED5B8 asm("D_0029DF7C"), D_0029DF50__cED5B8 asm("D_0029DF50"), D_0029DED8__cED5B8 asm("D_0029DED8"), D_0029DF44__cED5B8 asm("D_0029DF44"), D_0029DF40__cED5B8 asm("D_0029DF40"), D_0029DEEC__cED5B8 asm("D_0029DEEC");
extern u8__cED5B8 D_0029DF87__cED5B8 asm("D_0029DF87"), D_0029DF86__cED5B8 asm("D_0029DF86"), D_0029DF8B__cED5B8 asm("D_0029DF8B"), D_0029DF89__cED5B8 asm("D_0029DF89");
extern void *func_00254204__cED5B8(void *, int, unsigned int) asm("func_00254204");
extern void func_001F7420__cED5B8(Menu__cED5B8 *, int, int, int, int, int, u8__cED5B8 *, int) asm("func_001F7420");
extern void func_001F74F0__cED5B8(Menu__cED5B8 *) asm("func_001F74F0");
extern void func_001F57D0__cED5B8(int, int) asm("func_001F57D0");
extern void func_001BAF10__cED5B8(Game__cED5B8 *, int) asm("func_001BAF10");
extern void func_001F5EE8__cED5B8(int, int, int, int, int, int, int) asm("func_001F5EE8");
extern void func_001F4A98__cED5B8(void) asm("func_001F4A98");
extern void func_001F7550__cED5B8(Menu__cED5B8 *) asm("func_001F7550");
extern void func_001F75C0__cED5B8(Menu__cED5B8 *) asm("func_001F75C0");
extern void func_002038E0__cED5B8(void *, int) asm("func_002038E0");
extern void func_001F50F0__cED5B8(int, int) asm("func_001F50F0");
extern void func_001F5578__cED5B8(void) asm("func_001F5578");
extern void func_001C58A8__cED5B8(void *) asm("func_001C58A8");
extern void func_001C5960__cED5B8(void *) asm("func_001C5960");
extern void func_001C5940__cED5B8(void *) asm("func_001C5940");
extern void func_001F6A08__cED5B8(int) asm("func_001F6A08");
extern void func_001F76B8__cED5B8(Menu__cED5B8 *) asm("func_001F76B8");
extern void func_001ED5B8__cED5B8(void) asm("func_001ED5B8");
void func_001ED5B8__cED5B8(void) {
    u8__cED5B8 disabled[3];
    int index, choice;
    func_00254204__cED5B8(disabled, 0, sizeof(disabled));
    func_001F7420__cED5B8(&D_004C6CA8__cED5B8, 10, 0, 0, 2, 0, disabled, 3);
    func_001F74F0__cED5B8(&D_004C6CA8__cED5B8);
    func_001F57D0__cED5B8(0x2329, 1);
    func_001BAF10__cED5B8(&D_004B25A0__cED5B8, 0);
    index = D_004C6CA8__cED5B8.x8;
    if (index <= D_004C6CA8__cED5B8.xC) {
        do {
            if (D_004C6CA8__cED5B8.x1C[index] == 1)
                func_001F5EE8__cED5B8(0, 0, 0, 0x212, index * 15 + 95, D_0029E054__cED5B8[index], 192);
            else if (index == D_004C6CA8__cED5B8.x10)
                func_001F5EE8__cED5B8(0, 1, 2, 0x212, index * 15 + 95, D_0029E054__cED5B8[index], 255);
            else
                func_001F5EE8__cED5B8(0, 0, 1, 0x212, index * 15 + 95, D_0029E054__cED5B8[index], 255);
            index++;
        } while (index <= D_004C6CA8__cED5B8.xC);
    }
    D_0029DF7C__cED5B8 = 0;
    func_001F4A98__cED5B8();
    if (D_0029DF87__cED5B8) { func_001F7550__cED5B8(&D_004C6CA8__cED5B8); return; }
    if (D_0029DF86__cED5B8) { func_001F75C0__cED5B8(&D_004C6CA8__cED5B8); return; }
    if (D_0029DF8B__cED5B8) {
        PlayMenuSelectLegacy(&D_004C6CA8__cED5B8);
        choice = D_004C6CA8__cED5B8.x10;
        if (choice == 0) {
            D_004B25A0__cED5B8.x288 = 1;
            D_0029DED8__cED5B8 = 32;
            D_0029DF50__cED5B8 = 2;
            func_00203590((struct AnimationHandle *)(void *)(D_004C6370__cED5B8), 0x2330, 0, 2);
            func_002038E0__cED5B8(D_004C6370__cED5B8, 1);
            func_001F50F0__cED5B8(0x2330, 2);
            func_001F57D0__cED5B8(0x2329, 0);
            D_0029DF44__cED5B8 = 0;
            D_0029DF40__cED5B8 = 210;
            func_001F5578__cED5B8();
            func_001C58A8__cED5B8(D_004B25A0__cED5B8.x4F0);
            func_001C5960__cED5B8(D_004B25A0__cED5B8.x4F0);
            func_001C5940__cED5B8(D_004B25A0__cED5B8.x4F0);
            func_001F6A08__cED5B8(1);
        } else if (choice == 1) {
            D_0029DED8__cED5B8 = 10;
            D_0029DF50__cED5B8 = 2;
        } else if (choice == 2) {
            D_0029DED8__cED5B8 = D_004C6CA8__cED5B8.x10;
            D_0029DF50__cED5B8 = D_004C6CA8__cED5B8.x10;
        }
        D_0029DEEC__cED5B8 = 15;
        func_001F76B8__cED5B8(&D_004C6CA8__cED5B8);
        return;
    }
    if (D_0029DF89__cED5B8) {
        PlayMenuBackLegacy(&D_004C6CA8__cED5B8);
        D_0029DEEC__cED5B8 = D_0029DED8__cED5B8;
        D_0029DED8__cED5B8 = D_004C6CA8__cED5B8.x14;
        func_001F5578__cED5B8();
        func_001F76B8__cED5B8(&D_004C6CA8__cED5B8);
    }
}
}
/* ---- merged from src/unsorted/001ED900.c ---- */
extern "C" {
typedef unsigned char u8__c5;
typedef struct Menu__c5 { int x0, x4, x8, xC, x10, x14, x18; u8__c5 x1C[6]; } Menu__c5;
typedef struct Game__c5 { char pad0[0x10c]; int x10c; int x110; char pad114[0x8f4]; int xa08; char pada0c[0xa76c]; int xb178; } Game__c5;
typedef struct World__c5 { char pad0[0x1219b]; u8__c5 x1219b; char pad1219c[2]; u8__c5 x1219e; } World__c5;
extern Game__c5 D_004B25A0__c5 asm("D_004B25A0");
extern Menu__c5 D_004C6C78__c5 asm("D_004C6C78"), D_004C67C0__c5 asm("D_004C67C0");
extern World__c5 D_004225A0__c5 asm("D_004225A0");
extern char D_003C6930__c5[] asm("D_003C6930");
extern char **D_0029E05C__c5 asm("D_0029E05C");
extern int D_0029DED4__c5 asm("D_0029DED4"), D_0029DED8__c5 asm("D_0029DED8"), D_0029DEEC__c5 asm("D_0029DEEC"), D_0029DF40__c5 asm("D_0029DF40"), D_0029DF44__c5 asm("D_0029DF44"), D_0029DF50__c5 asm("D_0029DF50"), D_0029DF7C__c5 asm("D_0029DF7C");
extern u8__c5 D_0029DF86__c5 asm("D_0029DF86"), D_0029DF87__c5 asm("D_0029DF87"), D_0029DF89__c5 asm("D_0029DF89"), D_0029DF8B__c5 asm("D_0029DF8B");
extern void *func_00254204__c5(void *, int, unsigned int) asm("func_00254204");
extern void func_001F7420__c5(Menu__c5 *, int, int, int, int, int, u8__c5 *, int) asm("func_001F7420");
extern void func_001F74F0__c5(Menu__c5 *) asm("func_001F74F0"), func_001F7550__c5(Menu__c5 *) asm("func_001F7550"), func_001F75C0__c5(Menu__c5 *) asm("func_001F75C0"), func_001F76B8__c5(Menu__c5 *) asm("func_001F76B8");
extern void func_001F5398__c5(int, int, int, int, int, char *, int) asm("func_001F5398"), func_001BAF10__c5(Game__c5 *, int) asm("func_001BAF10"), func_001F4A98__c5(void) asm("func_001F4A98"), func_001F5578__c5(void) asm("func_001F5578"), func_001F5B08__c5(int) asm("func_001F5B08");
extern void func_001F5588__c5() asm("func_001F5588");
extern void func_00183258__c5(void *, int) asm("func_00183258");
extern int func_001F76D8__c5(Menu__c5 *, int) asm("func_001F76D8");

extern void func_001ED900__c5(void) asm("func_001ED900");
void func_001ED900__c5(void) {
    u8__c5 disabled[6];
    int index;
    func_00254204__c5(disabled, 0, sizeof(disabled));
    D_004B25A0__c5.xb178 = 0;
    if (D_004225A0__c5.x1219b == 0) {
        disabled[3] = 1;
    }
    if (D_004225A0__c5.x1219e == 0) {
        disabled[4] = 1;
    }
    func_001F7420__c5(&D_004C6C78__c5, 10, 16, 0, 5, 0, disabled, 6);
    func_001F74F0__c5(&D_004C6C78__c5);
    func_001F5398__c5(0, 0, 1, 30, 10, D_0029E05C__c5[0], 255);
    func_001BAF10__c5(&D_004B25A0__c5, 0);
    for (index = D_004C6C78__c5.x8; index <= D_004C6C78__c5.xC; index++) {
        if (D_004C6C78__c5.x1C[index] == 1) {
            func_001F5398__c5(0, 0, 0, 40, 65 + index * 15, D_0029E05C__c5[index + 1], 192);
        } else if (index == D_004C6C78__c5.x10) {
            func_001F5398__c5(0, 1, 2, 40, 65 + index * 15, D_0029E05C__c5[index + 1], 255);
        } else {
            func_001F5398__c5(0, 0, 1, 40, 65 + index * 15, D_0029E05C__c5[index + 1], 255);
        }
    }
    D_0029DF7C__c5 = 0;
    func_001F4A98__c5();
    if (D_0029DF87__c5) { func_001F7550__c5(&D_004C6C78__c5); return; }
    if (D_0029DF86__c5) { func_001F75C0__c5(&D_004C6C78__c5); return; }
    if (D_0029DF8B__c5) {
        PlayMenuSelectLegacy(&D_004C6C78__c5);
        D_0029DEEC__c5 = D_0029DED8__c5;
        switch (D_004C6C78__c5.x10) {
        case 0:
            D_004B25A0__c5.x10c = 12;
            D_0029DED8__c5 = 11;
            D_004B25A0__c5.x110 = 0;
            break;
        case 1:
            D_004B25A0__c5.x110 = 1;
            D_0029DED8__c5 = 12;
            break;
        case 2:
            D_004B25A0__c5.xb178 = 1;
            D_004B25A0__c5.x10c = 4;
            D_0029DED8__c5 = 34;
            D_004B25A0__c5.x110 = 1;
            func_001F5588__c5();
            D_0029DF50__c5 = 2;
            D_0029DF40__c5 = 210;
            D_0029DF44__c5 = 0;
            func_001F5578__c5();
            func_001F5B08__c5(1);
            D_0029DED4__c5 = func_001F76D8__c5(&D_004C67C0__c5, 0);
            D_004B25A0__c5.xa08 = D_0029DEEC__c5;
            break;
        case 4:
            func_00183258__c5(D_003C6930__c5, 1);
            D_004B25A0__c5.x10c = 17;
            D_0029DED8__c5 = 34;
            D_004B25A0__c5.x110 = 0;
            func_001F5588__c5();
            D_0029DF50__c5 = 2;
            D_0029DF40__c5 = 210;
            D_0029DF44__c5 = 0;
            func_001F5578__c5();
            func_001F5B08__c5(1);
            D_0029DED4__c5 = func_001F76D8__c5(&D_004C67C0__c5, 0);
            D_004B25A0__c5.xa08 = D_0029DEEC__c5;
            break;
        case 5:
            D_004B25A0__c5.x10c = 11;
            D_0029DED8__c5 = 34;
            D_004B25A0__c5.x110 = 0;
            func_001F5588__c5();
            D_0029DF50__c5 = 2;
            D_0029DF40__c5 = 210;
            D_0029DF44__c5 = 0;
            func_001F5578__c5();
            func_001F5B08__c5(1);
            D_0029DED4__c5 = func_001F76D8__c5(&D_004C67C0__c5, 0);
            D_004B25A0__c5.xa08 = D_0029DEEC__c5;
            break;
        case 3:
            D_004B25A0__c5.x10c = 15;
            D_004B25A0__c5.x110 = 0;
            D_0029DED8__c5 = 21;
            break;
        }
        D_0029DF50__c5 = 2;
        D_0029DF40__c5 = 210;
        D_0029DF44__c5 = 0;
        func_001F5578__c5();
        func_001F5B08__c5(1);
        func_001F76B8__c5(&D_004C6C78__c5);
        return;
    }
    if (D_0029DF89__c5) {
        PlayMenuBackLegacy(&D_004C6C78__c5);
        D_0029DEEC__c5 = D_0029DED8__c5;
        D_0029DED8__c5 = D_004C6C78__c5.x14;
        func_001F5578__c5();
        func_001F76B8__c5(&D_004C6C78__c5);
    }
}
}
/* ---- merged from src/unsorted/001EDD28.c ---- */
extern "C" {
typedef unsigned char u8__c6;
typedef unsigned int u128__c6 __attribute__((mode(TI)));

typedef struct Menu__c6 { int x0, x4, x8, xC, x10, x14, x18; u8__c6 x1C[6]; } Menu__c6;
typedef struct Game__c6 {
    char pad0[0x10C];
    int x10C;
    int x110;
    int x114;
    char pad118[0x170];
    int x288;
    char pad28C[0x14];
    int x2A0;
    char pad2A4[0xAED4];
    int xb178;
} Game__c6;
typedef struct World__c6 { char pad0[0x1219B]; u8__c6 x1219B, x1219C, x1219D, x1219E; } World__c6;
typedef struct Unk3F52D0__c6 { char pad0[0x12C]; u8__c6 x12C; } Unk3F52D0__c6;

#define NAME_S_ENTER_GAME (((u128__c6)0x454D4147LL << 64) | (u128__c6)0x5F5245544E455F53LL)

extern Game__c6 D_004B25A0__c6 asm("D_004B25A0");
extern Menu__c6 D_004C6D08__c6 asm("D_004C6D08");
extern World__c6 D_004225A0__c6 asm("D_004225A0");
extern Unk3F52D0__c6 D_003F52D0__c6 asm("D_003F52D0");
extern int D_004C37A0__c6[] asm("D_004C37A0");
extern char D_004B2914__c6[] asm("D_004B2914");
extern char **D_0029E060__c6 asm("D_0029E060");
extern u8__c6 D_003F53FC__c6[] asm("D_003F53FC");
extern char D_003C6930__c6[] asm("D_003C6930");
extern int D_0026EE50__c6[] asm("D_0026EE50");
extern int D_0029DED8__c6 asm("D_0029DED8"), D_0029DEEC__c6 asm("D_0029DEEC"), D_0029DF40__c6 asm("D_0029DF40"), D_0029DF44__c6 asm("D_0029DF44"), D_0029DF50__c6 asm("D_0029DF50"), D_0029DF7C__c6 asm("D_0029DF7C");
extern u8__c6 D_0029DF86__c6 asm("D_0029DF86"), D_0029DF87__c6 asm("D_0029DF87"), D_0029DF89__c6 asm("D_0029DF89"), D_0029DF8B__c6 asm("D_0029DF8B");

extern void *func_00254204__c6(void *, int, unsigned int) asm("func_00254204");
extern void func_001F7420__c6(Menu__c6 *, int, int, int, int, int, u8__c6 *, int) asm("func_001F7420");
extern void func_001F74F0__c6(Menu__c6 *) asm("func_001F74F0"), func_001F7550__c6(Menu__c6 *) asm("func_001F7550"), func_001F75C0__c6(Menu__c6 *) asm("func_001F75C0");
extern void func_001F76B8__c6(Menu__c6 *) asm("func_001F76B8");
extern void func_001F5398__c6(int, int, int, int, int, char *, int) asm("func_001F5398");
extern void func_001F5EE8__c6(int, int, int, int, int, char *, int) asm("func_001F5EE8");
extern void func_001BAF10__c6(Game__c6 *, int) asm("func_001BAF10");
extern void func_001F4A98__c6(void) asm("func_001F4A98"), func_001F5578__c6(void) asm("func_001F5578");
extern void func_001F5B08__c6(int) asm("func_001F5B08");
extern void func_001F57D0__c6(int, int) asm("func_001F57D0");
extern void func_001E2118__c6(void *, int) asm("func_001E2118"), func_00190108__c6(void *, void *) asm("func_00190108");
extern void func_00183258__c6(void *, int) asm("func_00183258");
extern void func_001BA3B8__c6(Game__c6 *, int) asm("func_001BA3B8");
extern void func_0018B4C8__c6(void) asm("func_0018B4C8");
extern void func_001C5B80__c6(void *) asm("func_001C5B80");
extern void func_001C57A8__c6(void *, u128__c6) asm("func_001C57A8");

extern void func_001EDD28__c6(void) asm("func_001EDD28");
void func_001EDD28__c6(void)
{
    u8__c6 disabled[6];
    int index;
    int twoPlayer;

    func_00254204__c6(disabled, 0, sizeof(disabled));
    if (D_004225A0__c6.x1219C == 0) {
        disabled[5] = 1;
    }
    if (D_004225A0__c6.x1219B == 0) {
        disabled[1] = 1;
    }
    if (D_004225A0__c6.x1219E == 0) {
        disabled[2] = 1;
    }
    if (D_004225A0__c6.x1219D == 0) {
        disabled[4] = 1;
    }
    D_004B25A0__c6.xb178 = 0;
    if (D_004B25A0__c6.x288 == 2) {
        func_001F7420__c6(&D_004C6D08__c6, 11, 16, 0, 5, 0, disabled, 6);
    } else {
        func_001F7420__c6(&D_004C6D08__c6, 11, 16, 0, 2, 0, disabled, 6);
    }
    func_001F74F0__c6(&D_004C6D08__c6);
    func_001F5398__c6(0, 0, 1, 30, 10, D_0029E060__c6[0], 255);
    func_001BAF10__c6(&D_004B25A0__c6, 0);
    for (index = D_004C6D08__c6.x8; index <= D_004C6D08__c6.xC; index++) {
        if (D_004C6D08__c6.x1C[index] == 1) {
            func_001F5EE8__c6(0, 0, 0, 530, 85 + index * 15, D_0029E060__c6[index + 1], 192);
        } else if (index == D_004C6D08__c6.x10) {
            func_001F5EE8__c6(0, 1, 2, 530, 85 + index * 15, D_0029E060__c6[index + 1], 255);
        } else {
            func_001F5EE8__c6(0, 0, 1, 530, 85 + index * 15, D_0029E060__c6[index + 1], 255);
        }
    }
    D_0029DF7C__c6 = 0;
    func_001F4A98__c6();
    if (D_0029DF87__c6) {
        func_001F7550__c6(&D_004C6D08__c6);
        return;
    }
    if (D_0029DF86__c6) {
        func_001F75C0__c6(&D_004C6D08__c6);
        return;
    }
    if (D_0029DF8B__c6) {
        PlayMenuSelectLegacy(&D_004C6D08__c6);
        D_0029DEEC__c6 = D_0029DED8__c6;
        twoPlayer = D_004B25A0__c6.x288 >= 2;
        switch (D_004C6D08__c6.x10) {
        case 0:
            D_004B25A0__c6.x10C = 12;
            D_004B25A0__c6.x110 = 0;
            D_004B25A0__c6.x114 = 1;
            twoPlayer = 0;
            D_0029DED8__c6 = 11;
            break;
        case 3:
            D_004B25A0__c6.xb178 = 1;
            D_004B25A0__c6.x10C = 4;
            D_004B25A0__c6.x110 = 1;
            D_004B25A0__c6.x114 = 0;
            break;
        case 4:
            func_001E2118__c6(D_004C37A0__c6, 0);
            func_00190108__c6(&D_003F52D0__c6, D_004C37A0__c6);
            if (D_003F52D0__c6.x12C != 0) {
                return;
            }
            D_004B25A0__c6.xb178 = 1;
            D_004B25A0__c6.x110 = 1;
            D_004B25A0__c6.x114 = 0;
            break;
        case 1:
            D_004B25A0__c6.x10C = 15;
            D_004B25A0__c6.x110 = 0;
            twoPlayer = 0;
            D_0029DEEC__c6 = D_0029DED8__c6;
            D_0029DED8__c6 = 21;
            break;
        case 2:
            twoPlayer = 1;
            func_00183258__c6(D_003C6930__c6, 1);
            D_004B25A0__c6.x10C = 17;
            D_004B25A0__c6.x110 = 0;
            break;
        case 5:
            D_004B25A0__c6.x10C = 16;
            D_004B25A0__c6.x2A0 = 21;
            D_004B25A0__c6.x110 = 0;
            func_001BA3B8__c6(&D_004B25A0__c6, D_0026EE50__c6[21]);
            break;
        }
        if (twoPlayer) {
            func_0018B4C8__c6();
            if (D_003F53FC__c6[0]) {
                int tmp;

                tmp = D_0029DEEC__c6;
                D_0029DEEC__c6 = D_0029DED8__c6;
                D_0029DED8__c6 = tmp;
            } else {
                D_0029DEEC__c6 = 8;
                D_0029DED8__c6 = 9;
                D_0029DF40__c6 = 120;
                D_0029DF44__c6 = 0;
                func_001C5B80__c6(D_004B2914__c6);
                func_001C57A8__c6(D_004B2914__c6 + 0x17C, NAME_S_ENTER_GAME);
            }
        }
        if (D_004B25A0__c6.x288 == 1) {
            D_0029DF50__c6 = 2;
            D_0029DF40__c6 = 210;
            D_0029DF44__c6 = 0;
            func_001F5578__c6();
            func_001F5B08__c6(1);
        }
        func_001F76B8__c6(&D_004C6D08__c6);
        return;
    }
    if (D_0029DF89__c6) {
        PlayMenuBackLegacy(&D_004C6D08__c6);
        func_001F57D0__c6(0x2329, 1);
        D_0029DEEC__c6 = D_0029DED8__c6;
        D_0029DED8__c6 = D_004C6D08__c6.x14;
        func_001F5578__c6();
        func_001F76B8__c6(&D_004C6D08__c6);
    }
}

}
/* ---- func_001EE2A0 ---- */
extern "C" {
typedef unsigned char u8__cEE2A0;
typedef unsigned int u128__cEE2A0 __attribute__((mode(TI)));

typedef struct Menu__cEE2A0 { int x0, x4, x8, xC, x10, x14, x18; u8__cEE2A0 x1C[8]; } Menu__cEE2A0;
typedef struct Game__cEE2A0 {
    char pad0[0x10C];
    int x10C;
    int x110;
    int x114;
    char pad118[0x170];
    int x288;
    char pad28C[4];
    int x290[2];
} Game__cEE2A0;
typedef struct Unk3F52D0__cEE2A0 { char pad0[0x12C]; u8__cEE2A0 x12C; } Unk3F52D0__cEE2A0;
typedef struct Slot__cEE2A0 { int x0, x4, x8; } Slot__cEE2A0;
typedef struct Track__cEE2A0 { Slot__cEE2A0 items[4]; u8__cEE2A0 x30; } Track__cEE2A0;

extern Game__cEE2A0 D_004B25A0__cEE2A0 asm("D_004B25A0");
extern Menu__cEE2A0 D_004C6C18__cEE2A0 asm("D_004C6C18");
extern u8__cEE2A0 D_004225A0__cEE2A0[] asm("D_004225A0");
extern Unk3F52D0__cEE2A0 D_003F52D0__cEE2A0 asm("D_003F52D0");
extern int D_004C37A0__cEE2A0[] asm("D_004C37A0");
extern char **D_0029E068__cEE2A0 asm("D_0029E068");
extern char D_0029D928__cEE2A0[] asm("D_0029D928");
extern char D_004B2A90__cEE2A0[] asm("D_004B2A90");
extern int D_0029DED8__cEE2A0 asm("D_0029DED8"), D_0029DEEC__cEE2A0 asm("D_0029DEEC"), D_0029DF7C__cEE2A0 asm("D_0029DF7C");
extern u8__cEE2A0 D_0029DF86__cEE2A0 asm("D_0029DF86"), D_0029DF87__cEE2A0 asm("D_0029DF87"), D_0029DF89__cEE2A0 asm("D_0029DF89"), D_0029DF8B__cEE2A0 asm("D_0029DF8B"), D_0029DFB4__cEE2A0 asm("D_0029DFB4");
extern void *func_00254204__cEE2A0(void *, int, unsigned int) asm("func_00254204");
extern int func_00254D68__cEE2A0(char *, const char *, ...) asm("func_00254D68");
extern int func_00194C80__cEE2A0(u8__cEE2A0 *, int) asm("func_00194C80");
extern int func_001822D8__cEE2A0(int) asm("func_001822D8");
extern void func_001F7420__cEE2A0(Menu__cEE2A0 *, int, int, int, int, int, u8__cEE2A0 *, int) asm("func_001F7420");
extern void func_001F74F0__cEE2A0(Menu__cEE2A0 *) asm("func_001F74F0"), func_001F7550__cEE2A0(Menu__cEE2A0 *) asm("func_001F7550"), func_001F75C0__cEE2A0(Menu__cEE2A0 *) asm("func_001F75C0");
extern void func_001F76B8__cEE2A0(Menu__cEE2A0 *) asm("func_001F76B8");
extern void func_001F5398__cEE2A0(int, int, int, int, int, char *, int) asm("func_001F5398");
extern void func_001BAF10__cEE2A0(Game__cEE2A0 *, int) asm("func_001BAF10");
extern void func_001F4A98__cEE2A0(void) asm("func_001F4A98"), func_001F5578__cEE2A0(void) asm("func_001F5578");
extern void func_001E2118__cEE2A0(void *, int) asm("func_001E2118"), func_00190108__cEE2A0(void *, void *) asm("func_00190108");
extern void func_001B97E0__cEE2A0(Game__cEE2A0 *, int, int, int, int) asm("func_001B97E0");
extern void func_001B9AE8__cEE2A0(Game__cEE2A0 *, int, int, int, int) asm("func_001B9AE8");
extern void func_001B9890__cEE2A0(Game__cEE2A0 *, int, int) asm("func_001B9890");
extern void func_001C57A8__cEE2A0(char *, u128__cEE2A0) asm("func_001C57A8");

static __inline__ Track__cEE2A0 *track_group__cEE2A0(void) {
    return (Track__cEE2A0 *)(D_004225A0__cEE2A0 + D_004B25A0__cEE2A0.x10C * 0x50 + ((D_004B25A0__cEE2A0.x290[0] >> 5) - 1) * 0x694);
}

extern void func_001EE2A0__cEE2A0(void) asm("func_001EE2A0");
void func_001EE2A0__cEE2A0(void)
{
    u8__cEE2A0 flags[7];
    int counts[7];
    int ids[7];
    int totals[7];
    char text[64];
    int i = 0;
    int index = 0;
    int n = 0;
    int found;
    int value = 0;

    func_00254204__cEE2A0(flags, 0, sizeof(flags));
    if (D_004225A0__cEE2A0[0x1219D] == 0) {
        flags[6] = 1;
    }
    n = (D_004B25A0__cEE2A0.x290[0] >> 5) - 1;
    found = 0;
    ids[0] = 3;
    ids[1] = 2;
    ids[2] = 0;
    ids[3] = 1;
    ids[4] = 5;
    ids[5] = 4;
    ids[6] = 6;
    if (func_00194C80__cEE2A0(D_004225A0__cEE2A0 + n * 0x694, 6) > 0
        || func_00194C80__cEE2A0(D_004225A0__cEE2A0 + n * 0x694, 7) > 0
        || func_00194C80__cEE2A0(D_004225A0__cEE2A0 + n * 0x694, 8) > 0
        || func_00194C80__cEE2A0(D_004225A0__cEE2A0 + n * 0x694, 9) > 0) {
        found = 1;
    }
    if (!found) {
        flags[6] = 1;
    }
    for (i = 0; i < 7; i++) {
        counts[i] = func_00194C80__cEE2A0(D_004225A0__cEE2A0 + n * 0x694, ids[i]);
        value = func_001822D8__cEE2A0(ids[i]);
        totals[i] = value;
        if (i != 6) {
            if (counts[i] == value) {
                flags[i] = 1;
            }
            if (counts[i] == 0) {
                flags[i] = 1;
            }
        }
    }
    func_001F7420__cEE2A0(&D_004C6C18__cEE2A0, 23, 16, 0, 6, 0, flags, 7);
    func_001F74F0__cEE2A0(&D_004C6C18__cEE2A0);
    func_001F5398__cEE2A0(0, 0, 1, 30, 10, D_0029E068__cEE2A0[0], 255);
    func_001BAF10__cEE2A0(&D_004B25A0__cEE2A0, 0);
    for (index = D_004C6C18__cEE2A0.x8; index <= D_004C6C18__cEE2A0.xC; index++) {
        if (ids[index] == 6) {
            func_00254D68__cEE2A0(text, D_0029D928__cEE2A0, D_0029E068__cEE2A0[index + 1]);
        } else {
            func_00254D68__cEE2A0(text, "%s %d/%d", D_0029E068__cEE2A0[index + 1], counts[index], totals[index]);
        }
        if (D_004C6C18__cEE2A0.x1C[index] == 1) {
            func_001F5398__cEE2A0(0, 0, 0, 40, 65 + index * 15, text, 192);
        } else if (index == D_004C6C18__cEE2A0.x10) {
            func_001F5398__cEE2A0(0, 1, 2, 40, 65 + index * 15, text, 255);
        } else {
            func_001F5398__cEE2A0(0, 0, 1, 40, 65 + index * 15, text, 255);
        }
    }
    D_0029DF7C__cEE2A0 = 0;
    func_001F4A98__cEE2A0();
    if (D_0029DF87__cEE2A0) {
        func_001F7550__cEE2A0(&D_004C6C18__cEE2A0);
        return;
    }
    if (D_0029DF86__cEE2A0) {
        func_001F75C0__cEE2A0(&D_004C6C18__cEE2A0);
        return;
    }
    if (D_0029DF8B__cEE2A0 && D_004C6C18__cEE2A0.x10 >= 0) {
        D_0029DEEC__cEE2A0 = D_0029DED8__cEE2A0;
        func_001C57A8__cEE2A0(D_004B2A90__cEE2A0, ((u128__cEE2A0)0x454D4147 << 64) | (u128__cEE2A0)0x5F5245544E455F53L);
        switch (D_004C6C18__cEE2A0.x10) {
        case 0:
            D_004B25A0__cEE2A0.x110 = 1;
            D_004B25A0__cEE2A0.x10C = 3;
            break;
        case 1:
            D_004B25A0__cEE2A0.x110 = 1;
            D_004B25A0__cEE2A0.x10C = 2;
            break;
        case 2:
            D_004B25A0__cEE2A0.x110 = 1;
            D_004B25A0__cEE2A0.x10C = 0;
            break;
        case 3:
            D_004B25A0__cEE2A0.x110 = 1;
            D_004B25A0__cEE2A0.x10C = 1;
            break;
        case 4:
            D_004B25A0__cEE2A0.x110 = 1;
            D_004B25A0__cEE2A0.x10C = 5;
            break;
        case 5:
            D_004B25A0__cEE2A0.x110 = 1;
            D_004B25A0__cEE2A0.x10C = 4;
            break;
        case 6:
            D_004B25A0__cEE2A0.x110 = 1;
            func_001E2118__cEE2A0(D_004C37A0__cEE2A0, 1);
            func_00190108__cEE2A0(&D_003F52D0__cEE2A0, D_004C37A0__cEE2A0);
            if (D_003F52D0__cEE2A0.x12C != 0) {
                return;
            }
            break;
        }
        func_001B97E0__cEE2A0(&D_004B25A0__cEE2A0, 0, track_group__cEE2A0()->items[0].x0, track_group__cEE2A0()->items[0].x4, track_group__cEE2A0()->items[0].x8);
        {
            Slot__cEE2A0 *b = (Slot__cEE2A0 *)((char *)track_group__cEE2A0() + 0x18);
            func_001B9AE8__cEE2A0(&D_004B25A0__cEE2A0, D_004B25A0__cEE2A0.x290[0], 0, b->x4, b->x8);
        }
        func_001B9890__cEE2A0(&D_004B25A0__cEE2A0, 0, track_group__cEE2A0()->x30);
        func_001F76B8__cEE2A0(&D_004C6C18__cEE2A0);
        D_0029DFB4__cEE2A0 = 1;
    } else if (D_0029DF89__cEE2A0) {
        PlayMenuBackLegacy(&D_004C6C18__cEE2A0);
        D_0029DEEC__cEE2A0 = D_0029DED8__cEE2A0;
        D_0029DED8__cEE2A0 = D_004C6C18__cEE2A0.x14;
        func_001F5578__cEE2A0();
        func_001F76B8__cEE2A0(&D_004C6C18__cEE2A0);
    }
}
}
/* ---- merged from src/unsorted/001EE8A8.c ---- */
extern "C" {
typedef unsigned char u8__c7;
typedef struct Menu__c7 { int x0, x4, x8, xC, x10, x14, x18; u8__c7 disabled[4]; } Menu__c7;
typedef struct Game__c7 {
    char pad0[0x10C];
    int x10C;
    int x110;
    int x114;
    char pad118[0x170];
    int x288;
    char pad28C[4];
    int x290[2];
} Game__c7;
typedef struct World__c7 { char pad0[0x1219D]; u8__c7 x1219D; } World__c7;
typedef struct Unk3F52D0__c7 { char pad0[0x12C]; u8__c7 x12C; } Unk3F52D0__c7;
typedef struct Slot__c7 { int x0, x4, x8; } Slot__c7;

extern Game__c7 D_004B25A0__c7 asm("D_004B25A0");
extern Menu__c7 D_004C6C48__c7 asm("D_004C6C48");
extern World__c7 D_004225A0__c7 asm("D_004225A0");
extern Unk3F52D0__c7 D_003F52D0__c7 asm("D_003F52D0");
extern int D_004C37A0__c7[] asm("D_004C37A0");
extern char **D_0029E06C__c7 asm("D_0029E06C");
/* "%s", shared with func_001EE2A0__c7 */
extern char D_0029D928__c7[] asm("D_0029D928");
extern int D_0029DED8__c7 asm("D_0029DED8"), D_0029DEEC__c7 asm("D_0029DEEC"), D_0029DF7C__c7 asm("D_0029DF7C");
extern u8__c7 D_0029DF86__c7 asm("D_0029DF86"), D_0029DF87__c7 asm("D_0029DF87"), D_0029DF89__c7 asm("D_0029DF89"), D_0029DF8B__c7 asm("D_0029DF8B"), D_0029DFB4__c7 asm("D_0029DFB4");
extern void *func_00254204__c7(void *, int, unsigned int) asm("func_00254204");
extern int func_00254D68__c7(char *, const char *, ...) asm("func_00254D68");
extern int func_00194A10__c7(World__c7 *, int) asm("func_00194A10");
extern Slot__c7 *func_00194A38__c7(World__c7 *, int, int) asm("func_00194A38");
extern Slot__c7 *func_00194A60__c7(World__c7 *, int, int) asm("func_00194A60");
extern int func_00194A90__c7(World__c7 *, int, int) asm("func_00194A90");
extern void func_001F7420__c7(Menu__c7 *, int, int, int, int, int, u8__c7 *, int) asm("func_001F7420");
extern void func_001F74F0__c7(Menu__c7 *) asm("func_001F74F0"), func_001F7550__c7(Menu__c7 *) asm("func_001F7550"), func_001F75C0__c7(Menu__c7 *) asm("func_001F75C0");
extern void func_001F76B8__c7(Menu__c7 *) asm("func_001F76B8");
extern void func_001F5398__c7(int, int, int, int, int, char *, int) asm("func_001F5398");
extern void func_001F5EE8__c7(int, int, int, int, int, char *, int) asm("func_001F5EE8");
extern void func_001BAF10__c7(Game__c7 *, int) asm("func_001BAF10");
extern void func_001F4A98__c7(void) asm("func_001F4A98"), func_001F5578__c7(void) asm("func_001F5578");
extern void func_001F57D0__c7(int, int) asm("func_001F57D0");
extern void func_001E2118__c7(void *, int) asm("func_001E2118"), func_00190108__c7(void *, void *) asm("func_00190108");
extern void func_001B97E0__c7(Game__c7 *, int, int, int, int) asm("func_001B97E0");
extern void func_001B9AE8__c7(Game__c7 *, int, int, int, int) asm("func_001B9AE8");
extern void func_001B9890__c7(Game__c7 *, int, int) asm("func_001B9890");

extern void func_001EE8A8__c7(void) asm("func_001EE8A8");
void func_001EE8A8__c7(void)
{
    u8__c7 disabled[2];
    char text[64];
    int progress;
    int found;
    int index;
    int player;
    int mode;
    Slot__c7 *a;
    Slot__c7 *b;

    func_00254204__c7(disabled, 0, sizeof(disabled));
    if (D_004225A0__c7.x1219D == 0) {
        disabled[1] = 1;
    }
    if (D_004B25A0__c7.x288 >= 3) {
        disabled[1] = 1;
    }
    progress = func_00194A10__c7(&D_004225A0__c7, 4);
    found = 0;
    if (func_00194A10__c7(&D_004225A0__c7, 6) > 0 || func_00194A10__c7(&D_004225A0__c7, 7) > 0
        || func_00194A10__c7(&D_004225A0__c7, 8) > 0 || func_00194A10__c7(&D_004225A0__c7, 9) > 0) {
        found = 1;
    }
    if (!found) {
        disabled[1] = 1;
    }
    if (progress == 0 || progress == 13) {
        disabled[0] = 1;
    }
    func_001F7420__c7(&D_004C6C48__c7, 23, 16, 0, 1, 0, disabled, 2);
    func_001F74F0__c7(&D_004C6C48__c7);
    func_001F5398__c7(0, 0, 1, 30, 10, D_0029E06C__c7[0], 255);
    func_001BAF10__c7(&D_004B25A0__c7, 0);
    for (index = D_004C6C48__c7.x8; index <= D_004C6C48__c7.xC; index++) {
        if (index == 1) {
            func_00254D68__c7(text, D_0029D928__c7, D_0029E06C__c7[2]);
        } else if (progress == 0) {
            func_00254D68__c7(text, "%s (%s)", D_0029E06C__c7[index + 1], D_0029E06C__c7[3]);
        } else {
            func_00254D68__c7(text, "%s %d/13", D_0029E06C__c7[index + 1], progress + 1);
        }
        if (D_004C6C48__c7.disabled[index] == 1) {
            func_001F5EE8__c7(0, 0, 0, 530, 95 + index * 15, text, 192);
        } else if (index == D_004C6C48__c7.x10) {
            func_001F5EE8__c7(0, 1, 2, 530, 95 + index * 15, text, 255);
        } else {
            func_001F5EE8__c7(0, 0, 1, 530, 95 + index * 15, text, 255);
        }
    }
    D_0029DF7C__c7 = 0;
    func_001F4A98__c7();
    if (D_0029DF87__c7) {
        func_001F7550__c7(&D_004C6C48__c7);
        return;
    }
    if (D_0029DF86__c7) {
        func_001F75C0__c7(&D_004C6C48__c7);
        return;
    }
    if (D_0029DF8B__c7 && D_004C6C48__c7.x10 >= 0) {
        PlayMenuSelectLegacy(&D_004C6C48__c7);
        D_0029DEEC__c7 = D_0029DED8__c7;
        switch (D_004C6C48__c7.x10) {
        case 0:
            D_004B25A0__c7.x110 = 1;
            D_004B25A0__c7.x10C = 4;
            D_004B25A0__c7.x114 = 0;
            break;
        case 1:
            D_004B25A0__c7.x110 = 1;
            D_004B25A0__c7.x114 = 0;
            func_001E2118__c7(D_004C37A0__c7, 1);
            func_00190108__c7(&D_003F52D0__c7, D_004C37A0__c7);
            if (D_003F52D0__c7.x12C != 0) {
                return;
            }
            break;
        }
        mode = D_004B25A0__c7.x10C;
        for (player = 0; player < 2; player++) {
            a = func_00194A38__c7(&D_004225A0__c7, player, mode);
            b = func_00194A60__c7(&D_004225A0__c7, player, mode);
            D_004B25A0__c7.x290[player] = b->x0 * 32 + 32;
            func_001B97E0__c7(&D_004B25A0__c7, player, a->x0, a->x4, a->x8);
            func_001B9AE8__c7(&D_004B25A0__c7, D_004B25A0__c7.x290[player], player, b->x4, b->x8);
            func_001B9890__c7(&D_004B25A0__c7, player, func_00194A90__c7(&D_004225A0__c7, player, D_004B25A0__c7.x10C));
        }
        func_001F76B8__c7(&D_004C6C48__c7);
        D_0029DFB4__c7 = 1;
    } else if (D_0029DF89__c7) {
        PlayMenuBackLegacy(&D_004C6C48__c7);
        D_0029DEEC__c7 = D_0029DED8__c7;
        D_0029DED8__c7 = D_004C6C48__c7.x14;
        func_001F57D0__c7(9001, 1);
        func_001F5578__c7();
        func_001F76B8__c7(&D_004C6C48__c7);
    }
}
}
/* func_001EED78: career menu with early preview when D_0029DF98 >= 2 */
extern "C" {
typedef unsigned char u8__cEED78;
typedef unsigned int u128__cEED78 __attribute__((mode(TI)));
typedef struct Menu__cEED78 { int x0, x4, x8, xC, x10, x14, x18; } Menu__cEED78;
typedef struct Game__cEED78 { char pad0[0x288]; int x288; } Game__cEED78;
extern Menu__cEED78 D_004C6D68__cEED78[1] asm("D_004C6D68");
extern Game__cEED78 D_004B25A0__cEED78 asm("D_004B25A0");
extern char D_004B2A90__cEED78[] asm("D_004B2A90");
extern int D_0029DED8__cEED78 asm("D_0029DED8"), D_0029DEEC__cEED78 asm("D_0029DEEC"), D_0029DF40__cEED78 asm("D_0029DF40"), D_0029DF44__cEED78 asm("D_0029DF44"), D_0029DF50__cEED78 asm("D_0029DF50"), D_0029DF7C__cEED78 asm("D_0029DF7C"), D_0029DF98__cEED78 asm("D_0029DF98");
extern u8__cEED78 D_0029DF86__cEED78 asm("D_0029DF86"), D_0029DF87__cEED78 asm("D_0029DF87"), D_0029DF89__cEED78 asm("D_0029DF89"), D_0029DF8B__cEED78 asm("D_0029DF8B");
extern int *D_0029E038__cEED78 asm("D_0029E038"), *D_0029E084__cEED78 asm("D_0029E084");
extern void *func_00254204__cEED78(void *, int, unsigned int) asm("func_00254204");
extern void func_001F7420__cEED78(Menu__cEED78 *, int, int, int, int, int, u8__cEED78 *, int) asm("func_001F7420");
extern void func_001F74F0__cEED78(Menu__cEED78 *) asm("func_001F74F0"), func_001F7550__cEED78(Menu__cEED78 *) asm("func_001F7550"), func_001F75C0__cEED78(Menu__cEED78 *) asm("func_001F75C0"), func_001F76B8__cEED78(Menu__cEED78 *) asm("func_001F76B8");
extern void func_001F7B10__cEED78(int) asm("func_001F7B10"), func_001F4A98__cEED78(void) asm("func_001F4A98"), func_001F5578__cEED78(void) asm("func_001F5578");
extern void func_0020F518__cEED78(int, int, int, int, int) asm("func_0020F518"), func_0020FB80__cEED78(int, int, int, int, int) asm("func_0020FB80"), func_0020F9F0__cEED78(int, int, int, int, int) asm("func_0020F9F0");
extern void func_001BAF10__cEED78(Game__cEED78 *, int) asm("func_001BAF10"), func_001C57A8__cEED78(void *, u128__cEED78) asm("func_001C57A8");
extern void func_001F5EE8__cEED78(int, int, int, int, int, int, int) asm("func_001F5EE8");
extern void func_001EED78__cEED78(void) asm("func_001EED78");
void func_001EED78__cEED78(void) {
    u8__cEED78 flags[3];
    int i, a, b;
    int selection;
    func_00254204__cEED78(flags, 0, sizeof(flags));
    func_001F7420__cEED78(D_004C6D68__cEED78, 16, 15, 0, 2, 0, flags, 3);
    func_001F74F0__cEED78(D_004C6D68__cEED78);
    if (D_0029DF98__cEED78 >= 2) {
        func_001F7B10__cEED78(D_004C6D68__cEED78[0].x10 + 2);
        func_0020F518__cEED78(0, 50, 20, 20, 255);
        func_0020FB80__cEED78(0, 445, 266, -156, -26);
        func_0020F9F0__cEED78(0, 0, 320, 160, D_0029E038__cEED78[D_0029DF98__cEED78]);
    }
    func_001BAF10__cEED78(&D_004B25A0__cEED78, 0);
    for (i = D_004C6D68__cEED78[0].x8; i <= D_004C6D68__cEED78[0].xC; i++) {
        if (flags[i] == 1)
            func_001F5EE8__cEED78(0, 0, 0, 530, i * 15 + 95, D_0029E084__cEED78[i + 1], 192);
        else {
            a = 0;
            if (i == D_004C6D68__cEED78[0].x10) { a = 1; b = 2; } else b = 1;
            func_001F5EE8__cEED78(0, a, b, 530, i * 15 + 95, D_0029E084__cEED78[i + 1], 255);
        }
    }
    D_0029DF7C__cEED78 = 0;
    func_001F4A98__cEED78();
    if (D_0029DF87__cEED78) { func_001F7550__cEED78(D_004C6D68__cEED78); D_0029DF98__cEED78 = 0; return; }
    if (D_0029DF86__cEED78) { func_001F75C0__cEED78(D_004C6D68__cEED78); D_0029DF98__cEED78 = 0; return; }
    if (D_0029DF8B__cEED78) {
        func_001F7B10__cEED78(D_004C6D68__cEED78[0].x10 + 2);
        if (D_0029DF98__cEED78) { func_001C57A8__cEED78(D_004B2A90__cEED78, (u128__cEED78)0x4C45434E41435F53UL); return; }
        PlayMenuSelectLegacy(D_004C6D68__cEED78);
        selection = D_004C6D68__cEED78[0].x10;
        if (selection == 0) D_004B25A0__cEED78.x288 = 2;
        else if (selection == 1) D_004B25A0__cEED78.x288 = 3;
        else if (selection == 2) D_004B25A0__cEED78.x288 = 4;
        D_0029DEEC__cEED78 = D_0029DED8__cEED78;
        D_0029DED8__cEED78 = D_004C6D68__cEED78[0].x18;
        D_0029DF50__cEED78 = 2;
        D_0029DF40__cEED78 = 210;
        D_0029DF44__cEED78 = 0;
        func_001F5578__cEED78();
        func_001F76B8__cEED78(D_004C6D68__cEED78);
        return;
    }
    if (!D_0029DF89__cEED78) return;
    {
        PlayMenuBackLegacy(D_004C6D68__cEED78);
        D_004B25A0__cEED78.x288 = 1;
        D_0029DEEC__cEED78 = 10;
        D_0029DED8__cEED78 = D_004C6D68__cEED78[0].x14;
        func_001F76B8__cEED78(D_004C6D68__cEED78);
        D_0029DF98__cEED78 = 0;
    }
}
}
/* ---- merged from src/unsorted/001EF0E0.c ---- */
extern "C" {
typedef unsigned char u8__c8;
typedef unsigned int u128__c8 __attribute__((mode(TI)));
typedef struct Menu__c8 { int x0,x4,x8,xC,x10,x14,x18; } Menu__c8;
typedef struct Game__c8 {
    char pad0[0x10c]; int x10c; int x110; char pad114[0x174]; int x288;
    char pad28c[0x77c]; int xa08; char pada0c[0xa768]; int xb174;
} Game__c8;
extern Game__c8 D_004B25A0__c8 asm("D_004B25A0");
extern Menu__c8 D_004C6DF8__c8 asm("D_004C6DF8"), D_004C67C0__c8 asm("D_004C67C0");
extern char D_004B2A90__c8[] asm("D_004B2A90");
typedef struct Unk3F52D0__c8 { char pad0[0x12c]; u8__c8 x12c; } Unk3F52D0__c8;
extern Unk3F52D0__c8 D_003F52D0__c8 asm("D_003F52D0");
extern int D_004C3778__c8[] asm("D_004C3778");
extern char **D_0029E088__c8 asm("D_0029E088");
extern int D_0029DED4__c8 asm("D_0029DED4"),D_0029DED8__c8 asm("D_0029DED8"),D_0029DEEC__c8 asm("D_0029DEEC"),D_0029DF40__c8 asm("D_0029DF40"),D_0029DF44__c8 asm("D_0029DF44"),D_0029DF50__c8 asm("D_0029DF50"),D_0029DF7C__c8 asm("D_0029DF7C");
extern u8__c8 D_0029DF86__c8 asm("D_0029DF86"),D_0029DF87__c8 asm("D_0029DF87"),D_0029DF89__c8 asm("D_0029DF89"),D_0029DF8B__c8 asm("D_0029DF8B");
extern void func_00254204__c8(void *,int,int) asm("func_00254204");
extern void func_001F7420__c8(Menu__c8 *,int,int,int,int,int,u8__c8 *,int) asm("func_001F7420");
extern void func_001F74F0__c8(Menu__c8 *) asm("func_001F74F0"),func_001F7550__c8(Menu__c8 *) asm("func_001F7550"),func_001F75C0__c8(Menu__c8 *) asm("func_001F75C0"),func_001F76B8__c8(Menu__c8 *) asm("func_001F76B8");
extern void func_001F7B80__c8(void) asm("func_001F7B80");
extern void func_001F5398__c8(int,int,int,int,int,char *,int) asm("func_001F5398"),func_001F5EE8__c8(int,int,int,int,int,char *,int) asm("func_001F5EE8");
extern void func_001BAF10__c8(Game__c8 *,int) asm("func_001BAF10"),func_001F4A98__c8(void) asm("func_001F4A98"),func_001F5578__c8(void) asm("func_001F5578"),func_001F5B08__c8(int) asm("func_001F5B08");
extern void func_001F5588__c8(void) asm("func_001F5588"),func_0018B4C8__c8(void) asm("func_0018B4C8");
extern void func_001C57A8__c8(char *, u128__c8) asm("func_001C57A8");
extern int func_001F76D8__c8(Menu__c8 *,int) asm("func_001F76D8");

extern void func_001EF0E0__c8(void) asm("func_001EF0E0");
void func_001EF0E0__c8(void) {
    u8__c8 disabled[4];
    int index, row, j, x, tmp;
    int selected = 1, style = 2;
    int mode;
    Game__c8 *game;
    Menu__c8 *menu;
    char **names;

    func_00254204__c8(disabled,0,4);
    D_004B25A0__c8.xb174 = 0;
    mode = D_004B25A0__c8.x288;
    if (mode == 1) {
        disabled[1] = 1;
        func_001F7420__c8(&D_004C6DF8__c8,0x22,1,0,3,0,disabled,4);
    } else if (mode == 2) {
        disabled[3] = 1;
        func_001F7420__c8(&D_004C6DF8__c8,0x22,0x18,0,2,0,disabled,4);
        func_001F7B80__c8();
    } else if (mode >= 3) {
        disabled[1] = 1;
        disabled[3] = 1;
        func_001F7420__c8(&D_004C6DF8__c8,0x22,0x18,0,2,0,disabled,4);
        func_001F7B80__c8();
    }
    func_001F74F0__c8(&D_004C6DF8__c8);
    func_001F5398__c8(0,0,1,30,10,D_0029E088__c8[0],255);
    func_001BAF10__c8(&D_004B25A0__c8,0);
    x = 40;
    if (D_004B25A0__c8.x288 >= 2) x = 530;
    for (index = D_004C6DF8__c8.x8; index <= D_004C6DF8__c8.xC; index++) {
        if (disabled[index] == 0) {
            j = 0;
            row = index;
            names = D_0029E088__c8;
            while (j <= index) {
                if (disabled[j] == 1) row--;
                j++;
            }
            menu = &D_004C6DF8__c8;
            if (index == menu->x10) { selected = 1; style = 2; }
            else { selected = 0; style = 1; }
            game = &D_004B25A0__c8;
            if (game->x288 >= 2)
                func_001F5EE8__c8(0,selected,style,x,85+row*15,names[index+1],255);
            else
                func_001F5398__c8(0,selected,style,x,85+row*15,names[index+1],255);
        }
    }
    D_0029DF7C__c8 = 0;
    func_001F4A98__c8();
    if (D_0029DF87__c8) { func_001F7550__c8(&D_004C6DF8__c8); return; }
    if (D_0029DF86__c8) { func_001F75C0__c8(&D_004C6DF8__c8); return; }
    if (D_0029DF8B__c8) {
        PlayMenuSelectLegacy(&D_004C6DF8__c8);
        D_0029DEEC__c8 = D_0029DED8__c8;
        D_0029DED8__c8 = D_004C6DF8__c8.x18;
        switch (D_004C6DF8__c8.x10) {
        case 0:
            D_004B25A0__c8.x10c = 12;
            break;
        case 1:
            D_004B25A0__c8.xb174 = 1;
            D_004B25A0__c8.x10c = 12;
            break;
        case 2:
            D_004B25A0__c8.x10c = 13;
            D_004B25A0__c8.x110 = 0;
            break;
        case 3:
            D_004B25A0__c8.x10c = 18;
            D_004C3778__c8[0] = 2;
            D_0029DED8__c8 = 30;
            func_001F76B8__c8(&D_004C6DF8__c8);
            return;
        }
        if (D_004B25A0__c8.x288 == 1) {
            func_001F5588__c8();
            D_0029DF50__c8 = 2;
            D_0029DF40__c8 = 210;
            D_0029DF44__c8 = 0;
            func_001F5578__c8();
            func_001F5B08__c8(1);
            D_0029DED4__c8 = func_001F76D8__c8(&D_004C67C0__c8,0);
            D_004B25A0__c8.xa08 = D_0029DEEC__c8;
        } else {
            func_0018B4C8__c8();
            if (D_003F52D0__c8.x12c) {
                tmp = D_0029DEEC__c8;
                D_0029DEEC__c8 = D_0029DED8__c8;
                D_0029DED8__c8 = tmp;
            } else {
                D_0029DEEC__c8 = 8;
                D_0029DED8__c8 = 9;
                D_0029DF40__c8 = 120;
                D_0029DF44__c8 = 0;
            }
        }
        func_001F76B8__c8(&D_004C6DF8__c8);
        return;
    }
    if (D_0029DF89__c8) {
        D_0029DEEC__c8 = D_0029DED8__c8;
        if (D_004B25A0__c8.x288 == 1) D_0029DED8__c8 = 1;
        else if (D_004B25A0__c8.x288 >= 2) D_0029DED8__c8 = 24;
        func_001F5578__c8();
        func_001C57A8__c8(D_004B2A90__c8, (u128__c8)0x4B4341425F53L);
        func_001F76B8__c8(&D_004C6DF8__c8);
    }
}
}
/* ---- merged candidate for func_001EF5D0 ---- */
extern "C" {
typedef unsigned char u8__web6;
typedef struct Menu__web6 { int x0,x4,x8,xC,x10,x14,x18; } Menu__web6;
typedef struct Game__web6 {
    char pad0[0x10C]; int x10C; int x110; char pad114[0x174]; int x288;
    char pad28C[4]; int x290; char pad294[0xC]; int x2A0; char pad2A4[0xC0];
    int x364; char pad368[0x6A0]; int xA08;
} Game__web6;
extern Game__web6 D_004B25A0__web6 asm("D_004B25A0");
extern Menu__web6 D_004C6CD8__web6 asm("D_004C6CD8"), D_004C67C0__web6 asm("D_004C67C0");
extern char **D_0029E064__web6 asm("D_0029E064");
extern int D_0026EEAC__web6[] asm("D_0026EEAC");
extern int D_0029DED4__web6 asm("D_0029DED4"), D_0029DED8__web6 asm("D_0029DED8"), D_0029DEEC__web6 asm("D_0029DEEC");
extern int D_0029DF40__web6 asm("D_0029DF40"), D_0029DF44__web6 asm("D_0029DF44"), D_0029DF50__web6 asm("D_0029DF50"), D_0029DF7C__web6 asm("D_0029DF7C");
extern u8__web6 D_0029DF86__web6 asm("D_0029DF86"), D_0029DF87__web6 asm("D_0029DF87"), D_0029DF89__web6 asm("D_0029DF89"), D_0029DF8B__web6 asm("D_0029DF8B"), D_0029DFB4__web6 asm("D_0029DFB4");
extern u8__web6 D_003F53FC__web6[] asm("D_003F53FC");
extern void *func_00254204__web6(void *, int, unsigned int) asm("func_00254204");
extern void func_001F7420__web6(Menu__web6 *, int, int, int, int, int, u8__web6 *, int) asm("func_001F7420");
extern void func_001F74F0__web6(Menu__web6 *) asm("func_001F74F0");
extern void func_001F7550__web6(Menu__web6 *) asm("func_001F7550");
extern void func_001F75C0__web6(Menu__web6 *) asm("func_001F75C0");
extern void func_001F76B8__web6(Menu__web6 *) asm("func_001F76B8");
extern void func_001F7B80__web6(void) asm("func_001F7B80");
extern void func_001F5398__web6(int, int, int, int, int, char *, int) asm("func_001F5398");
extern void func_001F5EE8__web6(int, int, int, int, int, char *, int) asm("func_001F5EE8");
extern void func_001BAF10__web6(Game__web6 *, int) asm("func_001BAF10");
extern void func_001F4A98__web6(void) asm("func_001F4A98");
extern void func_001F5578__web6(void) asm("func_001F5578");
extern void func_001F5B08__web6(int) asm("func_001F5B08");
extern void func_001F5588__web6(void) asm("func_001F5588");
extern void func_0018B4C8__web6(void) asm("func_0018B4C8");
extern void func_001B9AE8__web6(Game__web6 *, int, int, int, int) asm("func_001B9AE8");
extern void func_001BA3B8__web6(Game__web6 *, int) asm("func_001BA3B8");
extern int func_001F76D8__web6(Menu__web6 *, int) asm("func_001F76D8");
extern void func_001EF5D0__web6(void) asm("func_001EF5D0");
void func_001EF5D0__web6(void)
{
    u8__web6 disabled[2];
    Game__web6 *game = &D_004B25A0__web6;
    int index;
    int selected;
    int style;
    int tmp;
    int mode;

    func_00254204__web6(disabled, 0, sizeof(disabled));
    func_001F7420__web6(&D_004C6CD8__web6, 0x22, 1, 0, 1, 0, disabled, 2);
    func_001F74F0__web6(&D_004C6CD8__web6);
    func_001F5398__web6(0, 0, 1, 30, 10, D_0029E064__web6[0], 255);
    func_001BAF10__web6(game, 0);

    for (index = D_004C6CD8__web6.x8; index <= D_004C6CD8__web6.xC; index++) {
        Game__web6 *loop_game = &D_004B25A0__web6;
        int loop_mode = loop_game->x288;
        if (loop_mode == 1) {
            if (disabled[index] == 1) {
                func_001F5398__web6(0, 0, 0, 40, 95 + index * 15,
                               D_0029E064__web6[index + 1], 192);
            } else {
                selected = 0;
                if (index == D_004C6CD8__web6.x10) {
                    selected = 1;
                    style = 2;
                } else {
                    style = 1;
                }
                func_001F5398__web6(0, selected, style, 40, 95 + index * 15,
                               D_0029E064__web6[index + 1], 255);
            }
        } else if (loop_mode >= 2) {
            func_001F7B80__web6();
            if (disabled[index] == 1) {
                func_001F5EE8__web6(0, 0, 0, 530, 95 + index * 15,
                               D_0029E064__web6[index + 1], 192);
            } else {
                selected = 0;
                if (index == D_004C6CD8__web6.x10) {
                    selected = 1;
                    style = 2;
                } else {
                    style = 1;
                }
                func_001F5EE8__web6(0, selected, style, 530, 95 + index * 15,
                               D_0029E064__web6[index + 1], 255);
            }
        }
    }

    D_0029DF7C__web6 = 0;
    func_001F4A98__web6();
    if (D_0029DF87__web6) {
        func_001F7550__web6(&D_004C6CD8__web6);
        return;
    }
    if (D_0029DF86__web6) {
        func_001F75C0__web6(&D_004C6CD8__web6);
        return;
    }
    if (D_0029DF8B__web6) {
        PlayMenuSelectLegacy(&D_004C6CD8__web6);
        D_0029DEEC__web6 = D_0029DED8__web6;
        if (D_004C6CD8__web6.x10 == 0) {
            D_004B25A0__web6.x10C = 15;
            D_004B25A0__web6.x110 = 0;
            D_004B25A0__web6.x364 = 1;
            func_001B9AE8__web6(&D_004B25A0__web6, D_004B25A0__web6.x290, 0, 0, 0);
        } else if (D_004C6CD8__web6.x10 == 1) {
            D_004B25A0__web6.x110 = 0;
            D_004B25A0__web6.x10C = 15;
            D_004B25A0__web6.x364 = 0;
            func_001B9AE8__web6(&D_004B25A0__web6, D_004B25A0__web6.x290, 0, 0, 0);
        }

        if (D_004B25A0__web6.x288 == 1) {
            D_0029DED8__web6 = 0x22;
            func_001F5588__web6();
            D_0029DF50__web6 = 2;
            D_0029DF40__web6 = 0xD2;
            D_0029DF44__web6 = 0;
            func_001F5578__web6();
            func_001F5B08__web6(1);
            D_0029DED4__web6 = func_001F76D8__web6(&D_004C67C0__web6, 0);
            D_004B25A0__web6.xA08 = D_0029DEEC__web6;
            D_004B25A0__web6.x2A0 = 0x17;
            func_001BA3B8__web6(&D_004B25A0__web6, D_0026EEAC__web6[0]);
        } else {
            func_0018B4C8__web6();
            if (D_003F53FC__web6[0]) {
                tmp = D_0029DEEC__web6;
                D_0029DEEC__web6 = D_0029DED8__web6;
                D_0029DED8__web6 = tmp;
            } else {
                func_001F76B8__web6(&D_004C6CD8__web6);
                D_004B25A0__web6.x2A0 = 0x17;
                func_001BA3B8__web6(&D_004B25A0__web6, D_0026EEAC__web6[0]);
                D_0029DFB4__web6 = 1;
                D_0029DF40__web6 = 0x78;
                D_0029DF44__web6 = 0;
            }
        }
        func_001F76B8__web6(&D_004C6CD8__web6);
        return;
    }
    if (D_0029DF89__web6) {
        PlayMenuBackLegacy(&D_004C6CD8__web6);
        mode = D_004B25A0__web6.x288;
        D_0029DEEC__web6 = D_0029DED8__web6;
        if (mode == 1) {
            D_0029DED8__web6 = 1;
        } else if (mode >= 2) {
            D_0029DED8__web6 = 0x18;
        }
        func_001F76B8__web6(&D_004C6CD8__web6);
    }
}

}

/* ---- merged from src/unsorted/001EFA78.c ---- */
extern "C" {
typedef unsigned char u8__c9;
typedef struct Menu__c9 { int x0,x4,x8,xC,x10,x14,x18; } Menu__c9;
typedef struct Game__c9 { char pad0[0x10c]; int x10c; char pad110[0x8f8]; int xa08; char pada0c[0xa76c]; int xb178; } Game__c9;
typedef struct World__c9 { char pad0[0x1219d]; u8__c9 x1219d; } World__c9;
typedef struct Unk3F52D0__c9 { char pad0[0x12c]; u8__c9 x12c; } Unk3F52D0__c9;
extern Game__c9 D_004B25A0__c9 asm("D_004B25A0");
extern Menu__c9 D_004C6B88__c9 asm("D_004C6B88"),D_004C67C0__c9 asm("D_004C67C0");
extern World__c9 D_004225A0__c9 asm("D_004225A0");
extern Unk3F52D0__c9 D_003F52D0__c9 asm("D_003F52D0");
extern int D_004C37A0__c9[] asm("D_004C37A0");
extern char **D_0029E08C__c9 asm("D_0029E08C");
extern int D_0029DED4__c9 asm("D_0029DED4"),D_0029DED8__c9 asm("D_0029DED8"),D_0029DEEC__c9 asm("D_0029DEEC"),D_0029DF40__c9 asm("D_0029DF40"),D_0029DF44__c9 asm("D_0029DF44"),D_0029DF50__c9 asm("D_0029DF50"),D_0029DF7C__c9 asm("D_0029DF7C");
extern u8__c9 D_0029DF86__c9 asm("D_0029DF86"),D_0029DF87__c9 asm("D_0029DF87"),D_0029DF89__c9 asm("D_0029DF89"),D_0029DF8B__c9 asm("D_0029DF8B");
extern void *func_00254204__c9(void *,int,unsigned int) asm("func_00254204");
extern void func_001F7420__c9(Menu__c9 *,int,int,int,int,int,u8__c9 *,int) asm("func_001F7420");
extern void func_001F74F0__c9(Menu__c9 *) asm("func_001F74F0"),func_001F7550__c9(Menu__c9 *) asm("func_001F7550"),func_001F75C0__c9(Menu__c9 *) asm("func_001F75C0"),func_001F76B8__c9(Menu__c9 *) asm("func_001F76B8");
extern void func_001F5398__c9(int,int,int,int,int,char *,int) asm("func_001F5398"),func_001BAF10__c9(Game__c9 *,int) asm("func_001BAF10"),func_001F4A98__c9(void) asm("func_001F4A98"),func_001F5578__c9(void) asm("func_001F5578"),func_001F5B08__c9(int) asm("func_001F5B08");
extern void func_001F5588__c9() asm("func_001F5588");
extern void func_001E2118__c9(void *,int) asm("func_001E2118"),func_00190108__c9(void *,void *) asm("func_00190108");
extern int func_001F76D8__c9(Menu__c9 *,int) asm("func_001F76D8");
extern void func_001EFA78__c9(void) asm("func_001EFA78");
void func_001EFA78__c9(void) {
    u8__c9 disabled[3];
    int index, style;
    unsigned int selection = 1;
    func_00254204__c9(disabled,0,sizeof(disabled));
    if (D_004225A0__c9.x1219d == 0) {
        disabled[2] = 1;
    }
    D_004B25A0__c9.xb178 = 0;
    func_001F7420__c9(&D_004C6B88__c9,14,1,0,2,0,disabled,3);
    func_001F74F0__c9(&D_004C6B88__c9);
    func_001F5398__c9(0,0,1,30,10,D_0029E08C__c9[0],255);
    func_001BAF10__c9(&D_004B25A0__c9,0);
    index = D_004C6B88__c9.x8;
    if (index <= D_004C6B88__c9.xC) {
        do {
        if (disabled[index] == 1) {
            func_001F5398__c9(0,0,0,40,85+index*15,D_0029E08C__c9[index+1],192);
        } else {
            selection = 0;
            if (index == D_004C6B88__c9.x10) { selection = 1; style = 2; }
            else style = 1;
            func_001F5398__c9(0,selection,style,40,85+index*15,D_0029E08C__c9[index+1],255);
        }
        index++;
        } while (index <= D_004C6B88__c9.xC);
    }
    D_0029DF7C__c9 = 0;
    func_001F4A98__c9();
    if (D_0029DF87__c9) { func_001F7550__c9(&D_004C6B88__c9); return; }
    if (D_0029DF86__c9) { func_001F75C0__c9(&D_004C6B88__c9); return; }
    if (D_0029DF8B__c9) {
        PlayMenuSelectLegacy(&D_004C6B88__c9);
        D_0029DEEC__c9 = D_0029DED8__c9;
        selection = D_004C6B88__c9.x10;
        if (selection == 0) {
            D_004B25A0__c9.x10c = 3;
            D_004B25A0__c9.xb178 = 1;
            D_0029DED8__c9 = 34;
            func_001F5588__c9();
            D_0029DF50__c9 = 2;
            D_0029DF40__c9 = 210;
            D_0029DF44__c9 = 0;
            func_001F5578__c9();
            func_001F5B08__c9(1);
            D_0029DED4__c9 = func_001F76D8__c9(&D_004C67C0__c9,0);
            D_004B25A0__c9.xa08 = D_0029DEEC__c9;
        } else if (selection == 1) {
            D_0029DED8__c9 = 17;
        } else if (selection == 2) {
            func_001E2118__c9(&D_004C37A0__c9,0);
            func_00190108__c9(&D_003F52D0__c9,&D_004C37A0__c9);
            D_004B25A0__c9.xb178 = 1;
            if (D_003F52D0__c9.x12c == 0) {
                D_0029DED8__c9 = 34;
                func_001F5588__c9();
                D_0029DF50__c9 = selection;
                D_0029DF40__c9 = 210;
                D_0029DF44__c9 = 0;
                func_001F5578__c9();
                func_001F5B08__c9(1);
                D_0029DED4__c9 = func_001F76D8__c9(&D_004C67C0__c9,0);
                D_004B25A0__c9.xa08 = D_0029DEEC__c9;
            }
        }
        func_001F76B8__c9(&D_004C6B88__c9);
        return;
    }
    if (D_0029DF89__c9) {
        PlayMenuBackLegacy(&D_004C6B88__c9);
        D_0029DEEC__c9 = D_0029DED8__c9;
        D_0029DED8__c9 = D_004C6B88__c9.x14;
        func_001F5578__c9();
        func_001F76B8__c9(&D_004C6B88__c9);
    }
}
}
/* ---- merged from src/unsorted/001EFE28.c ---- */
extern "C" {
typedef unsigned char u8__c10;
typedef struct Menu__c10 { int x0,x4,x8,xC,x10,x14,x18; } Menu__c10;
typedef struct Game__c10 { char pad0[0x10c]; int x10c; char pad110[0x8f8]; int xa08; char pada0c[0xa76c]; int xb178; } Game__c10;
extern Game__c10 D_004B25A0__c10 asm("D_004B25A0");
extern Menu__c10 D_004C6B88__c10 asm("D_004C6B88"),D_004C67C0__c10 asm("D_004C67C0");
extern char **D_0029E090__c10 asm("D_0029E090");
extern int D_0029DED4__c10 asm("D_0029DED4"),D_0029DED8__c10 asm("D_0029DED8"),D_0029DEEC__c10 asm("D_0029DEEC"),D_0029DF40__c10 asm("D_0029DF40"),D_0029DF44__c10 asm("D_0029DF44"),D_0029DF50__c10 asm("D_0029DF50"),D_0029DF7C__c10 asm("D_0029DF7C");
extern u8__c10 D_0029DF86__c10 asm("D_0029DF86"),D_0029DF87__c10 asm("D_0029DF87"),D_0029DF89__c10 asm("D_0029DF89"),D_0029DF8B__c10 asm("D_0029DF8B");
extern void func_00254204__c10(void *,int,int) asm("func_00254204");
extern void func_001F7420__c10(Menu__c10 *,int,int,int,int,int,u8__c10 *,int) asm("func_001F7420");
extern void func_001F74F0__c10(Menu__c10 *) asm("func_001F74F0"),func_001F7550__c10(Menu__c10 *) asm("func_001F7550"),func_001F75C0__c10(Menu__c10 *) asm("func_001F75C0"),func_001F76B8__c10(Menu__c10 *) asm("func_001F76B8");
extern void func_001F5398__c10(int,int,int,int,int,char *,int) asm("func_001F5398"),func_001BAF10__c10(Game__c10 *,int) asm("func_001BAF10"),func_001F4A98__c10(void) asm("func_001F4A98"),func_001F5588__c10(int) asm("func_001F5588"),func_001F5578__c10(void) asm("func_001F5578"),func_001F5B08__c10(int) asm("func_001F5B08");
extern int func_001F76D8__c10(Menu__c10 *,int) asm("func_001F76D8");
extern void func_001EFE28__c10(void) asm("func_001EFE28");
void func_001EFE28__c10(void) {
    u8__c10 disabled[4];
    int index, row, selected, style;
    int disabled_row;
    func_00254204__c10(disabled,0,4);
    D_004B25A0__c10.xb178 = 0;
    func_001F7420__c10(&D_004C6B88__c10,14,12,0,3,0,disabled,4);
    func_001F74F0__c10(&D_004C6B88__c10);
    func_001F5398__c10(0,0,1,30,10,D_0029E090__c10[0],255);
    func_001BAF10__c10(&D_004B25A0__c10,0);
    index = D_004C6B88__c10.x8;
    if (index <= D_004C6B88__c10.xC) {
        
        do {
        if (disabled[index] == 1) {
            func_001F5398__c10(0,0,0,40,85+index*15,D_0029E090__c10[index+1],192);
        } else {
            selected = 0;
            if (index == D_004C6B88__c10.x10) { selected = 1; style = 2; }
            else style = 1;
            func_001F5398__c10(0,selected,style,40,85+index*15,D_0029E090__c10[index+1],255);
        }
        index++;
        } while (index <= D_004C6B88__c10.xC);
    }
    D_0029DF7C__c10 = 0;
    func_001F4A98__c10();
    if (D_0029DF87__c10) { func_001F7550__c10(&D_004C6B88__c10); return; }
    if (D_0029DF86__c10) { func_001F75C0__c10(&D_004C6B88__c10); return; }
    if (D_0029DF8B__c10) {
        PlayMenuSelectLegacy(&D_004C6B88__c10);
        D_0029DEEC__c10 = D_0029DED8__c10;
        D_0029DED8__c10 = D_004C6B88__c10.x18;
        if (D_004C6B88__c10.x10 == 0) { D_004B25A0__c10.x10c = 2; D_0029DED8__c10 = 14; }
        else if (D_004C6B88__c10.x10 == 1) { D_004B25A0__c10.x10c = 0; D_0029DED8__c10 = 14; }
        else if (D_004C6B88__c10.x10 == 2) { D_004B25A0__c10.x10c = 1; D_0029DED8__c10 = 14; }
        else if (D_004C6B88__c10.x10 == 3) { D_004B25A0__c10.x10c = 5; D_0029DED8__c10 = 14; }
        D_004B25A0__c10.xb178 = 1;
        D_0029DED8__c10 = 34;
        func_001F5588__c10(34);
        D_0029DF50__c10 = 2;
        D_0029DF40__c10 = 210;
        D_0029DF44__c10 = 0;
        func_001F5578__c10();
        func_001F5B08__c10(1);
        D_0029DED4__c10 = func_001F76D8__c10(&D_004C67C0__c10,0);
        D_004B25A0__c10.xa08 = D_0029DEEC__c10;
        func_001F76B8__c10(&D_004C6B88__c10);
        return;
    }
    if (D_0029DF89__c10) {
        PlayMenuBackLegacy(&D_004C6B88__c10);
        D_0029DEEC__c10 = D_0029DED8__c10;
        D_0029DED8__c10 = D_004C6B88__c10.x14;
        func_001F5578__c10();
        func_001F76B8__c10(&D_004C6B88__c10);
    }
}
}
typedef struct MenuM32 { int x0,x4,x8,xC,x10,x14,x18; } MenuM32;
typedef struct ObjM32 { char pad[4]; unsigned char x4; } ObjM32;
typedef struct GameM32 {
    char pad0[0x10C]; int x10C,x110;
    char pad114[0x290-0x114]; int x290;
    char pad294[0x2A0-0x294]; int x2A0;
    char pad2A4[0x374-0x2A4]; char x374[0x17C]; char x4F0[0x2070-0x4F0]; ObjM32 *x2070;
} GameM32;
typedef struct RecordM32 { unsigned char x0,x1; unsigned short x2; char pad4[12]; unsigned char x10,x11; char pad12[0x12]; int x24,x28,x2C,x30,x34,x38,x3C; } RecordM32;
extern MenuM32 D_004C6D98_M asm("D_004C6D98");
extern GameM32 D_004B25A0_M asm("D_004B25A0");
extern struct { char pad[0x12198]; unsigned char x12198; char pad12199[6]; unsigned char x1219F; } D_004225A0_M asm("D_004225A0");
extern struct { char pad[0x12C]; unsigned char x12C; } D_003F52D0_M asm("D_003F52D0");
extern struct { char pad[0x10]; RecordM32 *x10; } D_0043F9F0_M asm("D_0043F9F0");
extern char D_00389D90_M[] asm("D_00389D90"), D_00398C40_M[] asm("D_00398C40");
extern int D_0026EE50_M[] asm("D_0026EE50");
extern char **D_0029E070_M asm("D_0029E070");
extern int D_0029DED8_M asm("D_0029DED8"),D_0029DEEC_M asm("D_0029DEEC"),D_0029DF40_M asm("D_0029DF40"),D_0029DF44_M asm("D_0029DF44"),D_0029DF50_M asm("D_0029DF50"),D_0029DF7C_M asm("D_0029DF7C");
extern unsigned char D_0029DEE8_M asm("D_0029DEE8"),D_0029DF86_M asm("D_0029DF86"),D_0029DF87_M asm("D_0029DF87"),D_0029DF89_M asm("D_0029DF89"),D_0029DF8B_M asm("D_0029DF8B");
extern void *func_00254204_M(void *,int,unsigned int) asm("func_00254204");
extern void func_001F7420_M(void *,int,int,int,int,int,unsigned char *,int) asm("func_001F7420");
extern void func_001F5EE8_M(int,int,int,int,int,char *,int) asm("func_001F5EE8");
extern void func_001F74F0_M(void *) asm("func_001F74F0");
extern void func_001F7550_M(void *) asm("func_001F7550");
extern void func_001F75C0_M(void *) asm("func_001F75C0");
extern void func_001F76B8_M(void *) asm("func_001F76B8");
extern void func_001BAF10_M(void *,int) asm("func_001BAF10");
extern void func_001F4A98_M(void) asm("func_001F4A98");
extern void func_001F1090_M(void) asm("func_001F1090");
extern void func_001F9118_M(void) asm("func_001F9118");
extern void func_001F57D0_M(int,int) asm("func_001F57D0");
extern void func_0015E6E0_M(void *) asm("func_0015E6E0");
extern void func_00190108_M(void *,void *) asm("func_00190108");
extern void func_00190188_M(void *) asm("func_00190188");
extern void func_00187F48_M(void) asm("func_00187F48");
extern void func_00187BA8_M(void) asm("func_00187BA8");
extern void func_00187AB0_M(void) asm("func_00187AB0");
extern void func_00187D60_M(void) asm("func_00187D60");
extern void func_001BB670_M(void *) asm("func_001BB670");
extern void func_001F66B0_M(void) asm("func_001F66B0");
extern void func_001F7DE0_M(void) asm("func_001F7DE0");
extern void func_001BA3B8_M(void *,int) asm("func_001BA3B8");
extern void func_001B9AE8_M(void *,int,int,int,int) asm("func_001B9AE8");
extern void func_001B97E0_M(void *,int,int,int,int) asm("func_001B97E0");
extern void func_001B9890_M(void *,int,int) asm("func_001B9890");
extern void func_0016C298_M(void *,int) asm("func_0016C298");
extern void func_0016C2A0_M(void *,int) asm("func_0016C2A0");
extern void func_001C5B80_M(void *) asm("func_001C5B80");
extern void func_001C57A8_M(void *,u128) asm("func_001C57A8");
extern void func_001F5578_M(void) asm("func_001F5578");
extern void func_001F60C8_M(int) asm("func_001F60C8");
void func_001F0160(void) {
    unsigned char disabled[9];
    int index, selected = 1, style;
    int row, disabled_row;
    int track, rider;
    RecordM32 *record;
    func_00254204_M(disabled,0,9);
    if (D_004225A0_M.x12198 == 0) disabled[5] = 1;
    if (D_004225A0_M.x1219F == 0) disabled[4] = 1;
    func_001F7420_M(&D_004C6D98_M,13,15,0,8,0,disabled,9);
    func_001F74F0_M(&D_004C6D98_M);
    func_001BAF10_M(&D_004B25A0_M,0);
    index = D_004C6D98_M.x8;
    while (index <= D_004C6D98_M.xC) {
        if (disabled[index] == 1) {
            func_001F5EE8_M(1,0,0,530,90 + index * 10,D_0029E070_M[index+1],192);
        } else {
            if (index == D_004C6D98_M.x10) { selected = 1; style = 2; }
            else { selected = 0; style = 1; }
            func_001F5EE8_M(1,selected,style,530,90 + index * 10,D_0029E070_M[index+1],255);
        }
        index++;
    }
    D_0029DF7C_M = 0;
    func_001F4A98_M();
    if (D_0029DF87_M) { func_001F7550_M(&D_004C6D98_M); return; }
    if (D_0029DF86_M) { func_001F75C0_M(&D_004C6D98_M); return; }
    if (D_0029DF8B_M) {
        PlayMenuSelectLegacy(&D_004C6D98_M);
        D_0029DEEC_M = D_0029DED8_M;
        D_0029DED8_M = D_004C6D98_M.x18;
        switch (D_004C6D98_M.x10) {
        case 0:
            D_0029DED8_M = 13;
            func_001F1090_M();
            D_004B25A0_M.x2070->x4 = 0;
            break;
        case 1: D_0029DED8_M = 19; break;
        case 2:
            func_001F57D0_M(9001,0);
            func_0015E6E0_M(D_00389D90_M);
            func_00190108_M(&D_003F52D0_M,D_00389D90_M);
            D_0029DEEC_M = D_0029DED8_M;
            D_0029DED8_M = 2;
            func_001F57D0_M(9001,1);
            break;
        case 3:
            D_0029DEE8_M = 0;
            func_001F9118_M();
            D_0029DEEC_M = D_0029DED8_M;
            D_0029DED8_M = 2;
            break;
        case 4:
            func_00187F48_M();
            func_00190188_M(&D_003F52D0_M);
            D_0029DEEC_M = D_0029DED8_M;
            D_0029DED8_M = 2;
            break;
        case 5: D_0029DED8_M = 20; break;
        case 6:
            func_00187BA8_M();
            func_00190188_M(&D_003F52D0_M);
            if (!D_003F52D0_M.x12C) {
                func_001BB670_M(&D_004B25A0_M);
                func_001F66B0_M();
                func_001F7DE0_M();
                func_001F1090_M();
            }
            D_0029DEEC_M = D_0029DED8_M;
            D_0029DED8_M = 2;
            break;
        case 7:
            func_00187AB0_M();
            func_00190188_M(&D_003F52D0_M);
            D_0029DEEC_M = D_0029DED8_M;
            D_0029DED8_M = 2;
            break;
        case 8:
            func_00187D60_M();
            func_00190188_M(&D_003F52D0_M);
            if (D_003F52D0_M.x12C) {
                D_0029DEEC_M = D_0029DED8_M;
                D_0029DED8_M = 2;
            } else {
                track = D_0043F9F0_M.x10->x0;
                D_004B25A0_M.x2A0 = track;
                func_001BA3B8_M(&D_004B25A0_M,D_0026EE50_M[track]);
                record = D_0043F9F0_M.x10;
                rider = record->x2;
                D_004B25A0_M.x290 = rider;
                D_0029DEEC_M = D_0029DED8_M;
                D_0029DED8_M = 9;
                D_0029DF40_M = 120;
                D_0029DF44_M = 0;
                func_001B9AE8_M(&D_004B25A0_M,rider,0,record->x34,record->x38);
                func_001B97E0_M(&D_004B25A0_M,0,record->x24,record->x28,record->x2C);
                func_001B9890_M(&D_004B25A0_M,0,record->x3C);
                func_0016C298_M(D_00398C40_M,record->x10);
                func_0016C2A0_M(D_00398C40_M,record->x11);
                D_004B25A0_M.x10C = 19;
                D_004B25A0_M.x110 = 0;
                func_001C5B80_M(D_004B25A0_M.x374);
                func_001C57A8_M(D_004B25A0_M.x4F0,((u128)0x454D4147 << 64) | (u128)0x5F5245544E455F53UL);
            }
            break;
        }
        D_0029DF50_M = 2;
        D_0029DF40_M = 210;
        D_0029DF44_M = 0;
        func_001F5578_M();
        func_001F76B8_M(&D_004C6D98_M);
        return;
    }
    if (D_0029DF89_M) {
        PlayMenuBackLegacy(&D_004C6D98_M);
        D_0029DEEC_M = D_0029DED8_M;
        D_0029DED8_M = D_004C6D98_M.x14;
        func_001F5578_M();
        func_001F60C8_M(0);
        D_004B25A0_M.x2070->x4 = 1;
        func_001F76B8_M(&D_004C6D98_M);
    }
}

extern "C" {
typedef struct { char bytes[48]; } LocalItems__w1;
extern LocalItems__w1 D_00296B98__w1 asm("D_00296B98");
extern char D_004C8508__w1[] asm("D_004C8508");
extern char D_004225A0__w1[] asm("D_004225A0");
extern void func_001F9C78__w1(void *, void *) asm("func_001F9C78");
void func_001F0680(void)
{
    void *list = D_004C8508__w1;
    LocalItems__w1 items = D_00296B98__w1;
    int i;
    for (i = 0; i < 4; ++i)
        func_001F9C78__w1(list, items.bytes + i * 12);
    for (i = 22; i >= 0; --i)
        D_004225A0__w1[0x12190 + i] = 1;
}
}
extern "C" {
typedef unsigned int u128__n61 __attribute__((mode(TI)));
typedef struct { int x0,x4,x8; } Entry__n61;
typedef struct { char pad[0x118]; int x118; char pad2[0x3d4]; char x4F0[1]; } State__n61;
typedef struct { char pad[0x2114]; int x2114; char pad2[0x68]; int x2180; int x2184; int x2188; } Flags__n61;
extern struct { char x[0x13000]; } D_004225A0__n61 asm("D_004225A0");
extern State__n61 D_004B25A0__n61 asm("D_004B25A0");
extern Flags__n61 D_004325A0__n61 asm("D_004325A0");
extern char D_00296BC8__n61[] asm("D_00296BC8");
extern Entry__n61 D_0026DEA0__n61[] asm("D_0026DEA0");
extern struct { char x[4096]; } D_004C8508__n61 asm("D_004C8508");
extern int D_0029C3D4__n61 asm("D_0029C3D4");
extern int D_0029DED8__n61 asm("D_0029DED8");
extern void func_001F6618__n61(void) asm("func_001F6618");
extern void func_001BB670__n61(void *) asm("func_001BB670");
extern void func_001F66B0__n61(void) asm("func_001F66B0");
extern void func_001F7DE0__n61(void) asm("func_001F7DE0");
extern void func_001C57A8__n61(void *,u128__n61) asm("func_001C57A8");
extern void func_001F9E08__n61(void *) asm("func_001F9E08");
extern void func_001F5398__n61(int,int,int,int,int,void *,int) asm("func_001F5398");
extern Entry__n61 *func_001F9B98__n61(void *) asm("func_001F9B98");
extern int func_001F9BF0__n61(void *) asm("func_001F9BF0");
extern void func_001F9C78__n61(void *,void *) asm("func_001F9C78");
void func_001F0780(void) {
    int i, j, found, k, l, m, n;
    Entry__n61 *entry;
    void *list;

    func_001F6618__n61();
    func_001BB670__n61(&D_004B25A0__n61);
    func_001F66B0__n61();
    if (D_004B25A0__n61.x118) {
        func_001F7DE0__n61();
    }
    D_0029DED8__n61 = 2;
    func_001C57A8__n61(D_004B25A0__n61.x4F0, (u128__n61)0x5443454C45535F53UL);
    func_001F9E08__n61(&D_004C8508__n61);
    func_001F5398__n61(0, 1, 2, 0xA5, 0xA0, D_00296BC8__n61, 0xFF);

    k = 0;
    do {
        D_004325A0__n61.x2180 |= 1 << k;
        k++;
    } while (k < 16);
    for (l = 0; l < 18; l++) {
        D_004325A0__n61.x2188 |= 1 << l;
    }
    for (m = 22; m >= 0; m--) {
        D_004225A0__n61.x[0x12190 + m] = 1;
    }
    for (n = 0; n < 26; n++) {
        if (n != 23 && n != 21) {
            D_004325A0__n61.x2114 |= 1 << n;
        }
    }

    list = &D_004C8508__n61;
    for (i = 0; i < D_0029C3D4__n61; i++) {
        found = 0;
        for (j = 0; j < func_001F9BF0__n61(list); j++) {
            entry = func_001F9B98__n61(list) + j;
            if (D_0026DEA0__n61[i].x0 == entry->x0 &&
                D_0026DEA0__n61[i].x4 == entry->x4 &&
                D_0026DEA0__n61[i].x8 == entry->x8) {
                found = 1;
            }
        }
        if (!found) {
            func_001F9C78__n61(list, D_0026DEA0__n61 + i);
        }
    }
}

}
extern "C" {
typedef struct MenuN42 { int x0,x4,x8,xC,x10,x14,x18; } MenuN42;
typedef struct GameN42 { char pad[0x11A6C]; unsigned char x11A6C[4],x11A70[4]; } GameN42;
extern MenuN42 D_004C6BE8__n42 asm("D_004C6BE8");
extern GameN42 D_004225A0__n42 asm("D_004225A0");
extern int D_004C6B10__n42[4] asm("D_004C6B10");
extern int D_004C6B20__n42[4] asm("D_004C6B20");
extern int D_004C6B30__n42[4] asm("D_004C6B30");
extern char **D_0029E074__n42 asm("D_0029E074");
extern char **D_0029E078__n42 asm("D_0029E078");
extern char **D_0029E07C__n42 asm("D_0029E07C");
extern char D_004B25A0__n42[] asm("D_004B25A0");
extern char D_004B2A90__n42[] asm("D_004B2A90");
extern unsigned char D_0029DF84__n42 asm("D_0029DF84");
extern unsigned char D_0029DF85__n42 asm("D_0029DF85");
extern unsigned char D_0029DF86__n42 asm("D_0029DF86");
extern unsigned char D_0029DF87__n42 asm("D_0029DF87");
extern unsigned char D_0029DF89__n42 asm("D_0029DF89");
extern unsigned char D_0029DF8B__n42 asm("D_0029DF8B");
extern int D_0029DF7C__n42 asm("D_0029DF7C");
extern int D_0029DED8__n42 asm("D_0029DED8");
extern int D_0029DEEC__n42 asm("D_0029DEEC");
extern void func_00254204__n42(void *,int,int) asm("func_00254204");
extern void func_001F7420__n42(MenuN42 *,int,int,int,int,int,unsigned char *,int) asm("func_001F7420");
extern void func_001F74F0__n42(MenuN42 *) asm("func_001F74F0");
extern void func_001F7550__n42(MenuN42 *) asm("func_001F7550");
extern void func_001F75C0__n42(MenuN42 *) asm("func_001F75C0");
extern void func_001F76B8__n42(MenuN42 *) asm("func_001F76B8");
extern void func_001F5398__n42(int,int,int,int,int,char *,int) asm("func_001F5398");
extern void func_001F5EE8__n42(int,int,int,int,int,char *,int) asm("func_001F5EE8");
extern void func_001BAF10__n42(void *,int) asm("func_001BAF10");
extern void func_001F5828__n42(int) asm("func_001F5828");
extern void func_001F57D0__n42(int,int) asm("func_001F57D0");
extern void func_001F4A98__n42(void) asm("func_001F4A98");
extern int func_001F5320__n42(int,int,int,int,unsigned char *) asm("func_001F5320");
extern void func_001C57A8__n42(void *,u128) asm("func_001C57A8");
extern void func_00220198__n42(int,int) asm("func_00220198");
extern void func_001F5578__n42(void) asm("func_001F5578");
extern void func_001F1090__n42(void) asm("func_001F1090");
extern void func_001F60C8__n42(int) asm("func_001F60C8");
extern void func_001F0A28__n42(void) asm("func_001F0A28");
void func_001F0A28__n42(void) {
    unsigned char disabled[4], flags[6];
    int i, selected, style, alpha;
    func_00254204__n42(disabled,0,4);
    func_00254204__n42(flags,0,6);
    func_001F7420__n42(&D_004C6BE8__n42,2,2,0,3,0,disabled,4);
    func_001F74F0__n42(&D_004C6BE8__n42);
    func_001F5398__n42(0,0,1,30,9,D_0029E074__n42[0],255);
    func_001BAF10__n42(D_004B25A0__n42,0);
    for (i=0; i<4; i++) D_004C6B10__n42[i]=D_004C6B20__n42[i]*2+D_004C6B30__n42[i];
    for (i=D_004C6BE8__n42.x8; i<=D_004C6BE8__n42.xC; i++) {
        if (disabled[i-1]==1) {
            func_001F5398__n42(1,0,0,45,25+i*9,D_0029E078__n42[i],192);
            func_001F5398__n42(1,0,0,180,25+i*9,D_0029E07C__n42[0],192);
            func_001F5EE8__n42(1,0,0,422,25+i*9,D_0029E07C__n42[D_004C6B20__n42[i]+1],192);
            func_001F5398__n42(1,0,0,430,25+i*9,D_0029E07C__n42[5],192);
            func_001F5EE8__n42(1,0,0,570,25+i*9,D_0029E07C__n42[D_004C6B30__n42[i]+6],192);
        } else {
            if (i==D_004C6BE8__n42.x10) { selected=1; style=2; alpha=255; }
            else { selected=0; style=1; alpha=192; }
            func_001F5398__n42(1,selected,style,45,25+i*9,D_0029E078__n42[i],alpha);
            func_001F5398__n42(1,selected,style,180,25+i*9,D_0029E07C__n42[0],alpha);
            func_001F5EE8__n42(1,selected,style,422,25+i*9,D_0029E07C__n42[D_004C6B20__n42[i]+1],alpha);
            func_001F5398__n42(1,selected,style,430,25+i*9,D_0029E07C__n42[5],alpha);
            func_001F5EE8__n42(1,selected,style,570,25+i*9,D_0029E07C__n42[D_004C6B30__n42[i]+6],alpha);
        }
    }
    func_001F5828__n42(D_004C6B10__n42[D_004C6BE8__n42.x10]/2);
    func_001F57D0__n42(9001,2);
    D_0029DF7C__n42=0;
    func_001F4A98__n42();
    if (D_0029DF87__n42) { func_001F7550__n42(&D_004C6BE8__n42); return; }
    if (D_0029DF86__n42) { func_001F75C0__n42(&D_004C6BE8__n42); return; }
    if (D_0029DF84__n42) {
        int value=func_001F5320__n42(-1,0,5,D_004C6B10__n42[D_004C6BE8__n42.x10],flags);
        D_004C6B10__n42[D_004C6BE8__n42.x10]=value;
        D_004C6B20__n42[D_004C6BE8__n42.x10]=value/2;
        D_004C6B30__n42[D_004C6BE8__n42.x10]=value%2;
        func_001C57A8__n42(D_004B2A90__n42,((u128)0x5355 << 64) | (u128)0x4F49564552505F53L);
        return;
    }
    if (D_0029DF85__n42) {
        int value=func_001F5320__n42(1,0,5,D_004C6B10__n42[D_004C6BE8__n42.x10],flags);
        D_004C6B10__n42[D_004C6BE8__n42.x10]=value;
        D_004C6B20__n42[D_004C6BE8__n42.x10]=value/2;
        D_004C6B30__n42[D_004C6BE8__n42.x10]=value%2;
        func_001C57A8__n42(D_004B2A90__n42,(u128)0x5458454E5F53L);
        return;
    }
    if (D_0029DF8B__n42) {
        for (i=0;i<4;i++) {
            int enabled;
            D_004225A0__n42.x11A6C[i]=D_004C6B10__n42[i]/2;
            enabled = D_004C6B10__n42[i]%2;
            D_004225A0__n42.x11A70[i]=enabled;
            enabled = D_004225A0__n42.x11A70[i];
            func_00220198__n42(i,enabled!=0);
        }
        D_0029DEEC__n42=13;
        PlayMenuSelectLegacy(&D_004C6BE8__n42);
        D_0029DED8__n42=D_004C6BE8__n42.x18;
        func_001F57D0__n42(9001,1);
        func_001F5578__n42();
        func_001F76B8__n42(&D_004C6BE8__n42);
        return;
    }
    if (D_0029DF89__n42) {
        D_0029DEEC__n42=13;
        PlayMenuBackLegacy(&D_004C6BE8__n42);
        D_0029DED8__n42=D_004C6BE8__n42.x14;
        func_001F57D0__n42(9001,1);
        func_001F5578__n42();
        func_001F1090__n42();
        func_001F60C8__n42(0);
        func_001F76B8__n42(&D_004C6BE8__n42);
    }
}
}

/* ---- merged from src/unsorted/001F1090.c ---- */
extern "C" {
typedef struct Game__c11 {
    char p0[0x11a6c];
    unsigned char x11A6C[4];
    unsigned char x11A70[4];
} Game__c11;

extern Game__c11 D_004225A0__c11 asm("D_004225A0");
extern int D_004C6B10__c11[4] asm("D_004C6B10");
extern int D_004C6B20__c11[4] asm("D_004C6B20");
extern int D_004C6B30__c11[4] asm("D_004C6B30");
extern void func_00220198__c11(int, int) asm("func_00220198");

extern void func_001F1090__c11(void) asm("func_001F1090");
void func_001F1090__c11(void) {
    int i;

    for (i = 0; i < 4; i++) {
        D_004C6B20__c11[i] = D_004225A0__c11.x11A6C[i];
        D_004C6B30__c11[i] = D_004225A0__c11.x11A70[i];
        func_00220198__c11(i, D_004C6B30__c11[i] != 0);
    }
    for (i = 0; i < 4; i++) {
        D_004C6B10__c11[i] = D_004C6B20__c11[i] * 2 + D_004C6B30__c11[i];
    }
}
}
/* ---- merged from src/unsorted/001F1168.c ---- */
extern "C" {
typedef unsigned char u8__c12;
typedef struct Menu__c12 { int x0,x4,x8,xC,x10,x14,x18; } Menu__c12;
typedef struct Game__c12 { char pad0[0x10c]; int x10c; char pad110[0x8f8]; int xa08; char pada0c[0xa76c]; int xb178; } Game__c12;
extern Game__c12 D_004B25A0__c12 asm("D_004B25A0");
extern Menu__c12 D_004C6DC8__c12 asm("D_004C6DC8"),D_004C67C0__c12 asm("D_004C67C0");
extern char **D_0029E080__c12 asm("D_0029E080");
extern int D_0029DED4__c12 asm("D_0029DED4"),D_0029DED8__c12 asm("D_0029DED8"),D_0029DEEC__c12 asm("D_0029DEEC"),D_0029DF40__c12 asm("D_0029DF40"),D_0029DF44__c12 asm("D_0029DF44"),D_0029DF50__c12 asm("D_0029DF50"),D_0029DF7C__c12 asm("D_0029DF7C");
extern u8__c12 D_0029DF86__c12 asm("D_0029DF86"),D_0029DF87__c12 asm("D_0029DF87"),D_0029DF89__c12 asm("D_0029DF89"),D_0029DF8B__c12 asm("D_0029DF8B");
extern void func_00254204__c12(void *,int,int) asm("func_00254204");
extern void func_001F7420__c12(Menu__c12 *,int,int,int,int,int,u8__c12 *,int) asm("func_001F7420");
extern void func_001F74F0__c12(Menu__c12 *) asm("func_001F74F0"),func_001F7550__c12(Menu__c12 *) asm("func_001F7550"),func_001F75C0__c12(Menu__c12 *) asm("func_001F75C0"),func_001F76B8__c12(Menu__c12 *) asm("func_001F76B8");
extern void func_001F5EE8__c12(int,int,int,int,int,char *,int) asm("func_001F5EE8"),func_001BAF10__c12(Game__c12 *,int) asm("func_001BAF10"),func_001F4A98__c12(void) asm("func_001F4A98"),func_001F5588__c12(int) asm("func_001F5588"),func_001F5578__c12(void) asm("func_001F5578"),func_001F5B08__c12(int) asm("func_001F5B08");
extern int func_001F76D8__c12(Menu__c12 *,int) asm("func_001F76D8");
typedef struct Settings__c12 { char pad[0x14]; u8__c12 x14; } Settings__c12;
extern char D_004225A0__c12[] asm("D_004225A0"),D_00398C40__c12[] asm("D_00398C40");
extern void func_0016C298__c12(void *,int) asm("func_0016C298"),func_0016C2A0__c12(void *,int) asm("func_0016C2A0");
static __inline__ Settings__c12 *settings(char *world) { return (Settings__c12 *)(world + 0x11a60); }
extern void func_001F1168__c12(void) asm("func_001F1168");
void func_001F1168__c12(void) {
    u8__c12 disabled[3];
    int index, row, selected, style, selection;
    int disabled_row;
    func_00254204__c12(disabled,0,3);
    func_001F7420__c12(&D_004C6DC8__c12,2,2,0,2,settings(D_004225A0__c12)->x14,disabled,3);
    func_001F74F0__c12(&D_004C6DC8__c12);
    func_001BAF10__c12(&D_004B25A0__c12,0);
    index = D_004C6DC8__c12.x8;
    if (index <= D_004C6DC8__c12.xC) {
        
        do {
        if (disabled[index] == 1) {
            func_001F5EE8__c12(0,0,0,530,95+index*15,D_0029E080__c12[index+1],192);
        } else {
            selected = 0;
            if (index == D_004C6DC8__c12.x10) { selected = 1; style = 2; }
            else style = 1;
            func_001F5EE8__c12(0,selected,style,530,95+index*15,D_0029E080__c12[index+1],255);
        }
        index++;
        } while (index <= D_004C6DC8__c12.xC);
    }
    D_0029DF7C__c12 = 0;
    func_001F4A98__c12();
    if (D_0029DF87__c12) { func_001F7550__c12(&D_004C6DC8__c12); return; }
    if (D_0029DF86__c12) { func_001F75C0__c12(&D_004C6DC8__c12); return; }
    if (D_0029DF8B__c12) {
        PlayMenuSelectLegacy(&D_004C6DC8__c12);
        D_0029DEEC__c12 = D_0029DED8__c12;
        D_0029DED8__c12 = D_004C6DC8__c12.x18;
        selection = D_004C6DC8__c12.x10;
        switch (selection) {
        case 0:
            settings(D_004225A0__c12)->x14 = 0;
            func_0016C298__c12(D_00398C40__c12,0);
            func_0016C2A0__c12(D_00398C40__c12,50);
            break;
        case 1:
            settings(D_004225A0__c12)->x14 = selection;
            func_0016C298__c12(D_00398C40__c12,1);
            func_0016C2A0__c12(D_00398C40__c12,50);
            break;
        case 2:
            settings(D_004225A0__c12)->x14 = selection;
            func_0016C298__c12(D_00398C40__c12,2);
            func_0016C2A0__c12(D_00398C40__c12,50);
            break;
        }
        D_0029DED8__c12 = 2;
        D_0029DF50__c12 = 2;
        D_0029DF40__c12 = 210;
        D_0029DF44__c12 = 0;
        func_001F5578__c12();
        func_001F76B8__c12(&D_004C6DC8__c12);
        return;
    }
    if (D_0029DF89__c12) {
        PlayMenuBackLegacy(&D_004C6DC8__c12);
        D_0029DEEC__c12 = D_0029DED8__c12;
        D_0029DED8__c12 = D_004C6DC8__c12.x14;
        func_001F5578__c12();
        func_001F76B8__c12(&D_004C6DC8__c12);
    }
}
}
/* ---- merged from src/unsorted/001F14A0.c ---- */
extern "C" {
typedef struct State__c13 { char pad[0x10C]; int x10C, x110; } State__c13;
extern State__c13 D_004B25A0__c13 asm("D_004B25A0");
extern char D_003F52D0__c13[] asm("D_003F52D0"), D_00436940__c13[] asm("D_00436940");
extern int D_0029DED8__c13 asm("D_0029DED8");
extern unsigned char D_0029DFB4__c13 asm("D_0029DFB4");
extern void func_00190108__c13(void *, void *) asm("func_00190108");
extern void func_001F14A0__c13(void) asm("func_001F14A0");
void func_001F14A0__c13(void) {
    D_004B25A0__c13.x110 = 0;
    func_00190108__c13(D_003F52D0__c13, D_00436940__c13);
    if (D_004B25A0__c13.x10C == 14) D_0029DFB4__c13 = 1;
    else D_0029DED8__c13 = 2;
}
}
/* ---- merged from src/unsorted/001F1500.c ---- */
extern "C" {
typedef unsigned int u128__c14 __attribute__((mode(TI)));

typedef struct Block16__c14 {
    char x0[16];
} Block16__c14;

typedef struct Global__c14 {
    char pad0[0x10C];
    int x10C;
    char pad110[0x374 - 0x110];
    char x374[0x4F0 - 0x374];
    char x4F0[0xA08 - 0x4F0];
    int xA08;
} Global__c14;

#define NAME_S_ENTER_GAME (((u128__c14)0x454D4147LL << 64) | (u128__c14)0x5F5245544E455F53LL)


extern int D_0029DF68__c14 asm("D_0029DF68");
extern int D_0029DED8__c14 asm("D_0029DED8");
extern int D_0029DEEC__c14 asm("D_0029DEEC");
extern int D_0029DF40__c14 asm("D_0029DF40");
extern int D_0029DF44__c14 asm("D_0029DF44");
extern float D_0029DF78__c14 asm("D_0029DF78");
extern unsigned char D_003F53FC__c14[] asm("D_003F53FC");
extern Global__c14 D_004B25A0__c14 asm("D_004B25A0");
extern char D_004C6370__c14[] asm("D_004C6370");
extern int D_004C6910__c14[] asm("D_004C6910");
extern Block16__c14 *D_00275B70__c14[] asm("D_00275B70");
extern Block16__c14 *D_00275BB0__c14[] asm("D_00275BB0");
extern int D_00275C28__c14[] asm("D_00275C28");
extern int D_00275C30__c14[] asm("D_00275C30");
extern int D_00275C70__c14[] asm("D_00275C70");
extern int D_00275CB0__c14[] asm("D_00275CB0");

extern float func_00203C28__c14(void *) asm("func_00203C28");
extern void func_00203FE0__c14(void *, int) asm("func_00203FE0");
extern void func_002038E0__c14(void *, int) asm("func_002038E0");
extern void func_001F79D8__c14(int, int) asm("func_001F79D8");
extern void func_001F67F0__c14(int) asm("func_001F67F0");
extern void func_00139BF8__c14(int) asm("func_00139BF8");
extern void func_001F66B0__c14(void) asm("func_001F66B0");
extern void func_001F7DE0__c14(void) asm("func_001F7DE0");
extern void func_001C5B80__c14(void *) asm("func_001C5B80");
extern void func_001C57A8__c14(void *, u128__c14) asm("func_001C57A8");

extern void func_001F1500__c14(void) asm("func_001F1500");
void func_001F1500__c14(void)
{
    int fading = 0;
    int idx;
    Block16__c14 tmp;

    func_00203590((struct AnimationHandle *)(void *)(D_004C6370__c14), 0x2330, 0, D_00275CB0__c14[D_004C6910__c14[D_0029DF68__c14]]);
    if (D_0029DF78__c14 < func_00203C28__c14(D_004C6370__c14) && func_00203C28__c14(D_004C6370__c14) < 1.0f) {
        fading = 1;
    }
    if ((unsigned char)fading) {
        func_001F79D8__c14(0x80, 0x10);
    } else if (func_00203C28__c14(D_004C6370__c14) == 1.0f) {
        func_001F67F0__c14(1);
        func_00203590((struct AnimationHandle *)(void *)(D_004C6370__c14), 0x2330, 0, 1);
        func_00203FE0__c14(D_004C6370__c14, 1);
        func_002038E0__c14(D_004C6370__c14, 0);
        func_00139BF8__c14(0);
        func_001F66B0__c14();
        func_001F7DE0__c14();
        if (D_004B25A0__c14.x10C == 15 || D_004B25A0__c14.x10C == 17) {
            func_001C5B80__c14(D_004B25A0__c14.x374);
            func_001C57A8__c14(D_004B25A0__c14.x4F0, NAME_S_ENTER_GAME);
        }
        if (D_004B25A0__c14.x10C == 15 || D_004B25A0__c14.x10C == 17) {
            func_001C5B80__c14(D_004B25A0__c14.x374);
            func_001C57A8__c14(D_004B25A0__c14.x4F0, NAME_S_ENTER_GAME);
        }
        if (D_003F53FC__c14[0]) {
            D_0029DEEC__c14 = D_0029DED8__c14;
            D_0029DED8__c14 = D_004B25A0__c14.xA08;
            idx = D_004C6910__c14[D_0029DF68__c14];
            if (idx == 14) {
                func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c14[14]), D_00275C28__c14[0], 0, D_0029D914);
                tmp = *D_00275B70__c14[D_004C6910__c14[D_0029DF68__c14]];
                func_00203FE0__c14(&tmp, 1);
                tmp = *D_00275B70__c14[D_004C6910__c14[D_0029DF68__c14]];
                func_002038E0__c14(&tmp, 0);
            } else {
                func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c14[idx]), D_00275C30__c14[idx], 0, D_0029D914);
                tmp = *D_00275BB0__c14[D_004C6910__c14[D_0029DF68__c14]];
                func_00203FE0__c14(&tmp, 1);
                tmp = *D_00275BB0__c14[D_004C6910__c14[D_0029DF68__c14]];
                func_002038E0__c14(&tmp, 0);
            }
            func_00203590((struct AnimationHandle *)(void *)(D_004C6370__c14), 0x2330, 0, D_00275C70__c14[D_004C6910__c14[D_0029DF68__c14]]);
            func_00203FE0__c14(D_004C6370__c14, 1);
            func_002038E0__c14(D_004C6370__c14, 0);
        } else {
            D_0029DEEC__c14 = 8;
            D_0029DED8__c14 = 9;
            D_0029DF40__c14 = 120;
            D_0029DF44__c14 = 0;
        }
    }
}
}
/* ---- merged from src/unsorted/001F1968.c ---- */
extern "C" {
typedef unsigned int u128__c15 __attribute__((mode(TI)));
extern char D_004C6370__c15[] asm("D_004C6370"), D_004B2A90__c15[] asm("D_004B2A90");
extern int D_0029DED8__c15 asm("D_0029DED8"), D_0029DEEC__c15 asm("D_0029DEEC");
extern unsigned char D_0029DF8B__c15 asm("D_0029DF8B");
extern float func_00203C28__c15(void *) asm("func_00203C28");
extern void func_00203B68__c15(void *,int,float) asm("func_00203B68");
extern void func_001F4A98__c15(void) asm("func_001F4A98");
extern void func_001C57A8__c15(void *,u128__c15) asm("func_001C57A8");
extern void func_001F1968__c15(void) asm("func_001F1968");
void func_001F1968__c15(void) {
 int flag;
 float threshold = 0.95f;
 func_00203590((struct AnimationHandle *)(void *)(D_004C6370__c15),0x2330,0,2);
 if(func_00203C28__c15(D_004C6370__c15) >= 1.0f) {
 int previous = D_0029DED8__c15;
 D_0029DED8__c15 = 3;
 D_0029DEEC__c15 = previous;
 return;
 }
 flag = func_00203C28__c15(D_004C6370__c15) < threshold;
 if(flag & 0xFF) {
 func_001F4A98__c15();
 if(D_0029DF8B__c15) {
 func_00203B68__c15(D_004C6370__c15,1,threshold);
 func_001C57A8__c15(D_004B2A90__c15,(((((u128__c15)0xA886 << 16)|0x8A98)<<16|0x8AA6)<<15)|0x5F53);
 }
 } else if(func_00203C28__c15(D_004C6370__c15) > threshold) {
 func_00203C28__c15(D_004C6370__c15);
 }
}
}
INCLUDE_RODATA("build/rodata", D_00296B98);
INCLUDE_RODATA("build/rodata", D_00296BC8);
/* ---- merged from nearmiss-best func_001F1A90 (byte-exact C) ---- */
extern "C" {
typedef unsigned int u128__c62 __attribute__((mode(TI)));
typedef struct Value__c62 { int x[4]; } Value__c62;

extern int D_0029DF68__c62 asm("D_0029DF68");
extern int D_0029DED8__c62 asm("D_0029DED8");
extern int D_0029DF40__c62 asm("D_0029DF40");
extern int D_0029DF44__c62 asm("D_0029DF44");
extern int D_0029DF48__c62 asm("D_0029DF48");
extern int D_0029DF64__c62 asm("D_0029DF64");
extern int D_0029DF70__c62 asm("D_0029DF70");
extern int D_0029DF74__c62 asm("D_0029DF74");
extern int D_0029D908__c62 asm("D_0029D908");
extern int D_0029D914__c62 asm("D_0029D914");
extern int D_0029E01C__c62 asm("D_0029E01C");
extern int D_0029E024__c62 asm("D_0029E024");
extern unsigned char D_0029DF8B__c62 asm("D_0029DF8B");
extern char D_004B2A90__c62[] asm("D_004B2A90");
extern char D_004C6370__c62[] asm("D_004C6370");
extern int D_004C6910__c62[] asm("D_004C6910");
extern Value__c62 *D_00275B70__c62[] asm("D_00275B70");
extern Value__c62 *D_00275BB0__c62[] asm("D_00275BB0");
extern int D_00275BF0__c62[] asm("D_00275BF0");
extern int D_00275C30__c62[] asm("D_00275C30");
extern int D_00275C70__c62[] asm("D_00275C70");

extern float func_00203C28__c62(void *) asm("func_00203C28");
extern void func_00203B68__c62(void *, int, float) asm("func_00203B68");
extern void func_002038E0__c62(Value__c62 *, int) asm("func_002038E0");
extern void func_001F5B08__c62(int) asm("func_001F5B08");
extern void func_001F5D08__c62(int, int, int, int, int, void *, int) asm("func_001F5D08");
extern void func_001F4A98__c62(void) asm("func_001F4A98");
extern void func_001C52D0__c62(char *) asm("func_001C52D0");
extern void func_001C57A8__c62(void *, u128__c62) asm("func_001C57A8");
extern void func_001C59D8__c62(char *) asm("func_001C59D8");
extern void func_001C5988__c62(char *) asm("func_001C5988");
extern void func_001C5950__c62(char *) asm("func_001C5950");
extern void func_001F5578__c62(void) asm("func_001F5578");
extern void func_001F6940__c62(void) asm("func_001F6940");
extern void func_001F65A8__c62(int) asm("func_001F65A8");
extern void func_0021E428__c62(int) asm("func_0021E428");

extern void func_001F1A90__c62(void) asm("func_001F1A90");
void func_001F1A90__c62(void)
{
    int idx;
    float limit;
    Value__c62 value;

    D_0029DF70__c62++;
    func_00203590((struct AnimationHandle *)(void *)(D_004C6370__c62), 0x2330, 0, D_00275C70__c62[D_004C6910__c62[D_0029DF68__c62]]);
    if (func_00203C28__c62(D_004C6370__c62) >= 0.5f) {
        func_001F5B08__c62(1);
    }
    if (D_0029E01C__c62 == 0) {
        if (func_00203C28__c62(D_004C6370__c62) >= 1.0f) {
            func_001F5B08__c62(1);
            idx = D_004C6910__c62[D_0029DF68__c62];
            if (idx != 0xE) {
                func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c62[idx]), D_00275BF0__c62[idx], 0, D_0029D908__c62);
                value = *D_00275B70__c62[D_004C6910__c62[D_0029DF68__c62]];
                func_002038E0__c62(&value, 1);
            }
            D_0029DF48__c62 = 0;
            D_0029E01C__c62 = 1;
            return;
        }
        func_001F5D08__c62(0, 1, 2, 0x140, 0xC3, (void *)"RIDER SELECTED", 0xFF);
        func_00203590((struct AnimationHandle *)(void *)(D_004C6370__c62), 0x2330, 0, D_00275C70__c62[D_004C6910__c62[D_0029DF68__c62]]);
        if (D_0029DF48__c62 == 0) {
            func_001F4A98__c62();
            if (D_0029DF8B__c62) {
                func_001C52D0__c62(D_004B2A90__c62);
                D_0029DF74__c62 = 1;
            }
            if (D_0029DF70__c62 >= 0x1F && D_0029DF74__c62 == 1) {
                float table[15] = {
                    0.98f, 0.98f, 0.98f, 0.98f, 0.98f, 0.98f, 0.98f,
                    0.98f, 0.98f, 0.98f, 0.98f, 0.98f, 0.98f, 0.98f, 0.99f,
                };

                D_0029DF48__c62 = D_0029DF74__c62;
                D_0029DF74__c62 = 0;
                limit = table[D_004C6910__c62[D_0029DF68__c62]];
                func_00203B68__c62(D_004C6370__c62, 1, limit);
                idx = D_004C6910__c62[D_0029DF68__c62];
                if (idx != 0xE) {
                    func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c62[idx]), D_00275C30__c62[idx], 0, D_0029D914__c62);
                    value = *D_00275BB0__c62[D_004C6910__c62[D_0029DF68__c62]];
                    CallShared00203B68_0(&value, limit, 1);
                }
                func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c62[D_004C6910__c62[D_0029DF68__c62]]), D_00275BF0__c62[D_004C6910__c62[D_0029DF68__c62]], 0, D_0029D914__c62);
                value = *D_00275B70__c62[D_004C6910__c62[D_0029DF68__c62]];
                func_00203B68__c62(&value, 1, limit);
                func_001C57A8__c62(D_004B2A90__c62, (((((u128__c62)0xA886 << 16) | 0x8A98) << 16 | 0x8AA6) << 15) | 0x5F53);
                func_001C59D8__c62(D_004B2A90__c62);
                func_001F5B08__c62(1);
            }
        }
    } else if (D_0029E01C__c62 == 1) {
        D_0029DF64__c62 = 0;
        D_0029DED8__c62 = 0x10;
        D_0029E01C__c62 = 0;
        D_0029DF48__c62 = 0;
        func_001F6940__c62();
        limit = 1.0f;
        func_001F65A8__c62(0);
        func_001F5B08__c62(1);
        func_00203590((struct AnimationHandle *)(void *)(D_004C6370__c62), 0x2330, 0, D_00275C70__c62[D_004C6910__c62[D_0029DF68__c62]]);
        if (func_00203C28__c62(D_004C6370__c62) < limit) {
            func_00203B68__c62(D_004C6370__c62, 1, limit);
            func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c62[D_004C6910__c62[D_0029DF68__c62]]), D_00275C30__c62[D_004C6910__c62[D_0029DF68__c62]], 0, D_0029D914__c62);
            value = *D_00275BB0__c62[D_004C6910__c62[D_0029DF68__c62]];
            func_00203B68__c62(&value, 1, limit);
        }
        func_001C5988__c62(D_004B2A90__c62);
        func_001C5950__c62(D_004B2A90__c62);
        D_0029DF44__c62 = 0;
        D_0029DF40__c62 = 0x10E;
        func_001F5578__c62();
        D_0029DF70__c62 = 0;
        func_001F6940__c62();
        D_0029E024__c62 = 0;
        func_0021E428__c62(0);
    }
}
}
/* ---- merged from src/unsorted/001F2098.c ---- */
extern "C" {
typedef unsigned int u128__c16 __attribute__((mode(TI)));
typedef struct { long x0, x8; } __attribute__((packed)) Value__c16;

extern int D_0029DF68__c16 asm("D_0029DF68");
extern int D_0029DF70__c16 asm("D_0029DF70");
extern float D_0029DF78__c16 asm("D_0029DF78");
extern int D_0029E01C__c16 asm("D_0029E01C");
extern int D_0029DF48__c16 asm("D_0029DF48");
extern int D_0029DF74__c16 asm("D_0029DF74");
extern unsigned char D_0029DF8B__c16 asm("D_0029DF8B");
extern int D_0029D918__c16 asm("D_0029D918");
extern int D_0029DED8__c16 asm("D_0029DED8");
extern int D_0029DF64__c16 asm("D_0029DF64");
extern int D_0029DF44__c16 asm("D_0029DF44");
extern int D_0029DF40__c16 asm("D_0029DF40");
extern int D_004C6910__c16[] asm("D_004C6910");
extern int D_00275CB0__c16[] asm("D_00275CB0");
extern Value__c16 D_004C6370__c16 asm("D_004C6370");
extern Value__c16 *D_00275B70__c16[] asm("D_00275B70");
extern Value__c16 *D_00275BB0__c16[] asm("D_00275BB0");
extern int D_00275C28__c16[] asm("D_00275C28");
extern int D_00275C30__c16[] asm("D_00275C30");
extern char D_004B2A90__c16[] asm("D_004B2A90");

extern float func_00203C28__c16(Value__c16 *) asm("func_00203C28");
extern void func_00203B68__c16(Value__c16 *, float, int) asm("func_00203B68");
extern void func_001F5B08__c16(int) asm("func_001F5B08");
extern void func_001F4A98__c16(void) asm("func_001F4A98");
extern void func_001C57A8__c16(char *, u128__c16) asm("func_001C57A8");
extern void func_001C59D8__c16(char *) asm("func_001C59D8");
extern void func_001C5988__c16(char *) asm("func_001C5988");
extern void func_001C5950__c16(char *) asm("func_001C5950");
extern void func_001F5578__c16(void) asm("func_001F5578");

extern void func_001F2098__c16(void) asm("func_001F2098");
void func_001F2098__c16(void)
{
    int flag;
    float limit;
    int idx;

    D_0029DF70__c16++;
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6370__c16), 0x2330, 0, D_00275CB0__c16[D_004C6910__c16[D_0029DF68__c16]]);
    flag = func_00203C28__c16(&D_004C6370__c16) >= D_0029DF78__c16;
    if (flag & 0xFF) {
        D_0029E01C__c16 = 1;
    }
    if (D_0029E01C__c16 == 0) {
        if (D_0029DF70__c16 > 20 && func_00203C28__c16(&D_004C6370__c16) >= 1.0f) {
            func_001F5B08__c16(1);
            D_0029DF48__c16 = 0;
            D_0029E01C__c16 = 1;
            return;
        }
        if (D_0029DF48__c16 == 0) {
            func_001F4A98__c16();
            if (D_0029DF8B__c16) {
                D_0029DF74__c16 = 1;
            }
            {
            float table[15] = {
                0.65f, 0.65f, 0.7f, 0.55f, 0.55f, 0.55f, 0.55f, 0.55f,
                0.55f, 0.65f, 0.55f, 0.55f, 0.6f, 0.55f, 0.45f,
            };
            Value__c16 value;

            limit = table[D_004C6910__c16[D_0029DF68__c16]];
            if (D_0029DF74__c16 == 1) {
                flag = func_00203C28__c16(&D_004C6370__c16) <= limit;
                if (flag & 0xFF) {
                    D_0029DF74__c16 = 0;
                    D_0029DF48__c16 = 1;
                    CallShared00203B68_1(&D_004C6370__c16, limit, 1);
                    idx = D_004C6910__c16[D_0029DF68__c16];
                    if (idx != 14) {
                        func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c16[idx]), D_00275C30__c16[idx], 0, D_0029D918__c16);
                        value = *D_00275BB0__c16[D_004C6910__c16[D_0029DF68__c16]];
                        CallShared00203B68_2(&value, limit, 1);
                    } else {
                        func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c16[14]), D_00275C28__c16[0], 0, D_0029D918__c16);
                        value = *D_00275B70__c16[D_004C6910__c16[D_0029DF68__c16]];
                        CallShared00203B68_3(&value, limit, 1);
                    }
                    func_001C57A8__c16(D_004B2A90__c16, (((((u128__c16)0xA886 << 16) | 0x8A98) << 16 | 0x8AA6) << 15) | 0x5F53);
                    func_001C59D8__c16(D_004B2A90__c16);
                    func_001F5B08__c16(1);
                }
            }
            }
        }
    } else if (D_0029E01C__c16 == 1) {
        D_0029DED8__c16 = 0x12;
        D_0029DF64__c16 = 0;
        D_0029E01C__c16 = 0;
        D_0029DF48__c16 = 0;
        func_001F5B08__c16(1);
        func_001C5988__c16(D_004B2A90__c16);
        func_001C5950__c16(D_004B2A90__c16);
        D_0029DF44__c16 = 0;
        D_0029DF40__c16 = 0x10E;
        func_001F5578__c16();
        D_0029DF70__c16 = 0;
    }
}
}
void func_001F24B8(void)
{
    int zero = 0;
    int offset20 = 0x20, offset40 = 0x40, offset60 = 0x60, offset80 = 0x80;
    int offsetA0 = 0xA0, offsetC0 = 0xC0, offsetE0 = 0xE0, offset100 = 0x100;
    int offset120 = 0x120, offset140 = 0x140, offset160 = 0x160, offset180 = 0x180;
    int offset1A0 = 0x1A0, offset200 = 0x200;
    int slot[15] = {
        zero, offset20, offset40, offset60, offset80, offsetA0, offsetC0, offset160,
        offset120, offsetE0, offset100, offset1A0, offset140, offset180, offset200
    };
    Value value, value2, value3;
    int ready = 0;
    bool labelVisible, transitionReady, entryReady, previousReady, nextReady;
    long pendingReady;
    int pendingRider, labelRider, confirmedRider, activeRider, i;

    D_0029DF68 = 0;
    D_0029DF7C = 0;
    SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, 2);
    if (func_00203C28(&D_004C6370) >= 0.95f) {
        func_0021E428(1);
    }
    func_001C58A8(D_004B2A90);
    func_001C59B0(D_004B2A90);
    SelectAnimationResourceLegacy(D_00275B70[2], D_00275BF0[2], 0, 2);
    value = *D_00275B70[2];
    if (func_00204230(&value)) {
        labelVisible = ((value2 = *D_00275B70[2]), func_00203C28(&value2)) >= 0.31085044f &&
               ((value3 = *D_00275B70[2]), func_00203C28(&value3)) <= 0.92082113f;
        if (labelVisible & 0xFF) {
            func_001F57D0(0x235F, 1);
        } else {
            func_001F57D0(0x235F, 0);
        }
    }
    func_001F7B80();
    func_001F57D0(0x232B, 0);
    func_001F57D0(0x233D, 0);

    if (D_0029E028 == 0x2330 && (unsigned int)(D_0029E02C - 3) < 15) {
        SelectAnimationResourceLegacy(&D_004C6360, 0x2330, 0, D_0029E02C);
        pendingReady = 0;
        if (func_00203C28(&D_004C6360) >= 0.5f) {
            if (func_00203C28(&D_004C6360) <= 0.6f) {
                int pending = D_0029E024 != 0;
                pendingReady = pending;
            }
        }
        if (pendingReady & 0xFF) {
            func_001F6878(D_0029E020, RIDER);
            D_0029E024 = 0;
        }
        transitionReady = func_00203C28(&D_004C6360) >= 1.0f ||
               func_00203C28(&D_004C6360) <= 0.0f;
        if (transitionReady & 0xFF) {
            ready = 1;
            pendingRider = RIDER;
            if (D_004C6780[pendingRider] == 1 && pendingRider >= 7) {
                D_004C6780[pendingRider] = 0;
                SelectAnimationResourceLegacy(D_00275B70[pendingRider], D_00275BF0[pendingRider], 0, D_0029D91C);
                value = *D_00275B70[RIDER];
                func_00203E10(&value, 1.0f);
                value = *D_00275B70[RIDER];
                func_002038E0(&value, 1);
                D_0029DED8 = 0x1C;
                D_004C69A0[RIDER] = D_0029D91C;
                if (RIDER == 14) {
                    SelectAnimationResourceLegacy(D_00275BB0[14], D_00275C68[0], 0, D_0029D91C);
                    value = *D_00275BB0[RIDER];
                    func_00203E10(&value, 1.0f);
                    value = *D_00275BB0[RIDER];
                    func_002038E0(&value, 1);
                }
                return;
            }
            if (SELECTABLE(RIDER) == 0) {
                func_001F57D0(0x233D, 1);
                labelRider = RIDER;
                if (labelRider == 7 || labelRider == 8 || labelRider == 14) {
                    func_001F5398(0, 1, 2, 0x55, 0xA7, D_00296C70, 0xFF);
                } else {
                    func_001F5398(0, 1, 2, 0x55, 0xA7, D_00296C80, 0xFF);
                }
                if (D_0029DEF0 > 0) {
                    func_001F5398(5, 0, 0, 0x34, 0x34, D_0029E048[RIDER], 0xFF);
                    func_001F6798(1);
                } else {
                    func_001F6798(0);
                }
            } else {
                func_001F57D0(0x232B, RIDER);
                if (D_0029DEF0 > 0) {
                    if (D_0029DEF0 == 1) {
                        func_001F5398(5, 0, 0, 0x34, 0x23, D_0029E040[RIDER], 0xFF);
                    } else if (D_0029DEF0 == 2) {
                        func_001F5398(5, 0, 0, 0x34, 0x23, D_0029E044[RIDER], 0xFF);
                    }
                    if (func_00193FC8(D_004225A0, func_001F7AF8(RIDER))) {
                        func_001F5398(5, 0, 0, 0x34, 0x5A, D_0029E048[1], 0xFF);
                    } else {
                        func_001F5398(5, 0, 0, 0x34, 0x6A, D_0029E048[0], 0xFF);
                    }
                    func_001F6798(1);
                } else {
                    func_001F6798(0);
                }
            }
        }
        func_001F5398(0, 0, 1, 0x1E, 0xA, D_0029E03C[0], 0xFF);
        if (D_0029DEF0 == 0) {
            func_001BAF10(D_004B25A0, 1);
        } else {
            func_001BAF10(D_004B25A0, 2);
        }
    } else if (D_0029E02C == 2) {
        SelectAnimationResourceLegacy(&D_004C6360, D_0029E028, 0, 2);
        entryReady = func_00203C28(&D_004C6360) == 1.0f ||
               func_00203C28(&D_004C6360) == 0.0f;
        if (entryReady & 0xFF) {
            ready = 1;
            if (D_0029DEF0 > 0) {
                if (D_0029DEF0 == 1) {
                    func_001F5398(5, 0, 0, 0x34, 0x23, D_0029E040[RIDER], 0xFF);
                } else if (D_0029DEF0 == 2) {
                    func_001F5398(5, 0, 0, 0x34, 0x23, D_0029E044[RIDER], 0xFF);
                }
                if (func_00193FC8(D_004225A0, func_001F7AF8(RIDER))) {
                    func_001F5398(5, 0, 0, 0x34, 0x5A, D_0029E048[1], 0xFF);
                } else {
                    func_001F5398(5, 0, 0, 0x34, 0x6A, D_0029E048[0], 0xFF);
                }
                func_001F6798(1);
            } else {
                func_001F6798(0);
            }
            func_001F57D0(0x232B, RIDER);
            func_001F5398(0, 0, 1, 0x1E, 0xA, D_0029E03C[D_0029DF68], 0xFF);
            if (D_0029DEF0 == 0) {
                func_001BAF10(D_004B25A0, 1);
            } else {
                func_001BAF10(D_004B25A0, 2);
            }
        }
    }

    func_001F4A98();
    if (D_0029DF88 && ready) {
        if (SELECTABLE(RIDER) == 0) {
            if (D_0029DEF0 != 0) {
                D_0029DEF0 = 0;
            } else {
                D_0029DEF0 = 1;
            }
        } else {
            if (D_0029DEF0 == 2) {
                D_0029DEF0 = 0;
            } else {
                D_0029DEF0++;
            }
        }
        func_001C57A8(D_004B2A90, SELECT_SOUND);
        func_001F5578();
        return;
    }
    if (D_0029DF8B && ready) {
        func_001F5578();
        func_001F6798(0);
        SelectAnimationResourceLegacy(&D_004C6370, D_0029E028, 0, D_0029E02C);
        if (D_0029E028 == 0x2330 && D_0029E02C == 2 &&
            func_00203C28(&D_004C6370) <= 0.9f) {
            func_00203B68(&D_004C6370, 1, 0.9f);
            func_001C57A8(D_004B2A90, SELECT_SOUND);
            func_001C59D8(D_004B2A90);
            return;
        }
        if (D_0029E028 == 0x2330 && D_0029E02C == 2 &&
            func_00203C28(&D_004C6370) != 1.0f) {
            func_00203FE0(&D_004C6370, 1);
            func_002038E0(&D_004C6370, 0);
            return;
        }
        confirmedRider = RIDER;
        if (SELECTABLE(confirmedRider) == 0) {
            func_001F5398(1, 1, 0, 0x6E, 0xAF, D_00296C90, 0xFF);
            func_001C57A8(D_004B25A0 + 0x4F0, CANCEL_SOUND);
            return;
        }
        D_0029DF70 = 0;
        ((PlayerSlot *)D_004B25A0)->selected[D_0029DF68] = slot[confirmedRider];
        ((RiderSlotMap *)D_004B25A0)->player[slot[confirmedRider] / 32 - 1] = D_0029DF68;
        func_001F6338(confirmedRider);
        SelectAnimationResourceLegacy(D_00275B70[RIDER], D_00275BF0[RIDER], 0, D_0029D914);
        value = *D_00275B70[RIDER];
        func_00203E10(&value, 1.0f);
        ((SelectedRider *)D_004B25A0)->selected = RIDER;
        if (RIDER == 2) {
            func_001F57D0(0x235F, 0);
        }
        func_00202E38((struct AnimationHandle *)(void *)(D_00275B70[RIDER]), 1, 7, 500.1f);
        activeRider = RIDER;
        if (activeRider < 11) {
            ((ObjectSlots *)D_004B25A0)->slots[activeRider + 3].object->active = 1;
        } else if (activeRider == 11) {
            ((SpecialSlot *)D_004B25A0)->object->active = 1;
        } else if (activeRider == 12 || activeRider == 13) {
            ((ObjectSlots *)D_004B25A0)->slots[activeRider + 2].object->active = 1;
        }
        if (RIDER != 14) {
            SelectAnimationResourceLegacy(D_00275BB0[RIDER], D_00275C30[RIDER], 0, D_0029D914);
            value = *D_00275BB0[RIDER];
            func_00203E10(&value, 1.0f);
            func_00202E38((struct AnimationHandle *)(void *)(D_00275BB0[RIDER]), 1, 7, 500.1f);
        }
        SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, D_00275C70[RIDER]);
        func_00203E10(&D_004C6370, 1.0f);
        func_00202E38((struct AnimationHandle *)(void *)(&D_004C6370), 1, 7, 500.1f);
        func_001F50F0(0x2330, D_00275C70[RIDER]);
        D_0029DEEC = D_0029DED8;
        D_0029DED8 = 0x21;
        func_001F57D0(0x232B, 0);
        func_001C57A8(D_004B2A90, SELECT_SOUND);
        func_001C5948(D_004B2A90);
        return;
    }
    if (D_0029DF89 && ready) {
        func_001F6798(0);
        func_001F5B08(0);
        func_001F6940();
        D_0029E024 = 0;
        func_0021E428(0);
        func_001F65E0(0);
        func_001F5578();
        func_001F60C8(0);
        if (D_004B2828[0] == 1) {
            D_0029DED8 = 15;
        } else if (D_004B2828[0] >= 2) {
            D_0029DED8 = 10;
        }
        for (i = 0x20; i <= 0xE0; i += 0x20) {
            D_004B2848[i / 32 - 1] = -1;
        }
        D_0029DEEC = 3;
        RIDER = 1;
        SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, 1);
        func_00203FE0(&D_004C6370, 1);
        func_002038E0(&D_004C6370, 0);
        func_001F50F0(0x2330, 1);
        func_001F57D0(0x232B, 0);
        func_001C57A8(D_004B2A90, BACK_SOUND);
        func_001C59D8(D_004B2A90);
        ((AnimationOwner *)D_004B2A90)->animation->value = 1.0f;
        func_001F57D0(0x233D, 0);
        return;
    }
    if (D_0029DF84 && ready) {
        int previousOldindex, previousAnimation;
        func_001F6798(0);
        previousOldindex = RIDER;
        D_0029E020 = previousOldindex;
        RIDER--;
        previousAnimation = previousOldindex + 1;
        if (RIDER <= 0) {
            RIDER = 14;
        }
        if (previousOldindex == 14 && RIDER == 13) {
            previousAnimation = 16;
        } else if (RIDER == 14) {
            if (D_004B25EA[0] == 0) {
                previousAnimation = 15;
                RIDER = 13;
            } else {
                previousAnimation = 17;
            }
        }
        SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, 18);
        if (RIDER == 11 &&
            ((previousReady = func_00203C28(&D_004C6370) != 0.0f &&
                     func_00203C28(&D_004C6370) != 1.0f) & 0xFF)) {
            SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, previousAnimation);
            func_00203E10(&D_004C6370, 1.375f);
            func_00204080(&D_004C6370, 0);
            func_00202E38((struct AnimationHandle *)(void *)(&D_004C6370), 1, 1, 1000.2f);
        } else {
            SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, previousAnimation);
            func_00203E10(&D_004C6370, 1.375f);
            func_002039B0(&D_004C6370, 1);
        }
        func_001F60C8(RIDER);
        func_001F50F0(0x2330, previousAnimation);
        func_001C57A8(D_004B2A90, PREVIOUS_SOUND);
        D_0029E024 = 1;
        return;
    }
    if (D_0029DF85 && ready) {
        int nextOldindex, nextAnimation;
        func_001F6798(0);
        nextOldindex = RIDER;
        D_0029E020 = nextOldindex;
        RIDER++;
        nextAnimation = nextOldindex + 2;
        if (RIDER == 14) {
            if (D_004B25EA[0] == 0) {
                nextAnimation = 15;
                RIDER = 1;
            } else {
                nextAnimation = 16;
            }
        } else if (RIDER >= 15) {
            nextAnimation = 17;
            RIDER = 1;
        }
        SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, 18);
        if (RIDER == 13 &&
            ((nextReady = func_00203C28(&D_004C6370) != 0.0f &&
                     func_00203C28(&D_004C6370) != 1.0f) & 0xFF)) {
            SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, nextAnimation);
            func_00203E10(&D_004C6370, 1.375f);
            func_00204080(&D_004C6370, 1);
            func_00202E38((struct AnimationHandle *)(void *)(&D_004C6370), 1, 1, 1000.2f);
        } else {
            SelectAnimationResourceLegacy(&D_004C6370, 0x2330, 0, nextAnimation);
            func_00203E10(&D_004C6370, 1.375f);
            func_002038E0(&D_004C6370, 1);
        }
        func_001F60C8(RIDER);
        func_001F50F0(0x2330, nextAnimation);
        func_001C57A8(D_004B2A90, NEXT_SOUND);
        D_0029E024 = 1;
        return;
    }
    if (D_0029DF8C && D_0029DEF0 > 0 &&
        func_00193FC8(D_004225A0, func_001F7AF8(RIDER)) && ready) {
        if (D_00296CA0[RIDER] != 0) {
            func_001F95D0(D_004C7530, D_00296CA0[RIDER]);
            func_00190108(D_003F52D0, D_004C7530);
        }
    }
}

typedef struct { int x0, x4, x8, xC, x10; } Menu__n27;
typedef struct { char pad0[0x288]; int x288, x28C, x290; char pad294[0x14]; int x2A8[7]; } Game__n27;
typedef struct { int ids[6]; } __attribute__((packed)) Ids__n27;
typedef struct { char pad0[0x1219D]; unsigned char x1219D; } World__n27;
extern World__n27 world__n27 asm("D_004225A0");
extern void func_001BAF10__n27(Game__n27 *,int) asm("func_001BAF10");
extern Game__n27 game__n27 asm("D_004B25A0");
extern Menu__n27 menu__n27 asm("D_004C6D38");
extern Ids__n27 ids__n27 asm("D_00296CE0");
extern char **labels__n27 asm("D_0029E058");
extern int func_00194C80__n27(char *,int) asm("func_00194C80");
extern int func_00194A10__n27(char *,int) asm("func_00194A10");
extern int func_001822D8__n27(int) asm("func_001822D8");
extern void func_00254204__n27(void *,int,int) asm("func_00254204");
extern void func_001F7420__n27(Menu__n27 *,int,int,int,int,int,unsigned char *,int) asm("func_001F7420");
extern void func_001F74F0__n27(Menu__n27 *) asm("func_001F74F0");
extern void func_001F7550__n27(Menu__n27 *) asm("func_001F7550");
extern void func_001F75C0__n27(Menu__n27 *) asm("func_001F75C0");
extern void func_001F76B8__n27(Menu__n27 *) asm("func_001F76B8");
extern void func_001F5EE8__n27(int,int,int,int,int,char *,int) asm("func_001F5EE8");
extern void func_001F5100__n27(void) asm("func_001F5100");
extern void func_001F65A8__n27(int) asm("func_001F65A8");
extern void func_001F6A08__n27(int) asm("func_001F6A08");
extern unsigned char D_0029DF86, D_0029DF87;
extern int D_0029DF98, D_0029E01C, D_0029DF40, D_0029DF44;
void func_001F3B98(void)
{
    unsigned char disabled[2];
    int found = 0;
    int n, i, selected=1, color=2;
    func_00254204__n27(disabled,0,2);
    n = (game__n27.x290 >> 5) - 1;
    if (game__n27.x288 == 1) {
        if (world__n27.x1219D) {
            int any = 0;
            if(func_00194C80__n27((char *)&world__n27+n*0x694,6)>0 ||
                    func_00194C80__n27((char *)&world__n27+n*0x694,7)>0 ||
                    func_00194C80__n27((char *)&world__n27+n*0x694,8)>0 ||
                    func_00194C80__n27((char *)&world__n27+n*0x694,9)>0) any=1;
            found=any;
        }
        Ids__n27 ids = ids__n27;
        int totals[6];
        int counts[6];
        int j;
        for(j=0;j<6;j++) {
            counts[j] = func_00194C80__n27((char *)&world__n27+n*0x694,ids.ids[j]);
            totals[j] = func_001822D8__n27(ids.ids[j]);
            if(counts[j]!=totals[j] && counts[j]!=0) found=1;
        }
    } else if(game__n27.x288<3) {
        if(world__n27.x1219D) {
            int any = 0;
            if(func_00194A10__n27((char *)&world__n27,6)>0 ||
                    func_00194A10__n27((char *)&world__n27,7)>0 ||
                    func_00194A10__n27((char *)&world__n27,8)>0 ||
                    func_00194A10__n27((char *)&world__n27,9)>0) any=1;
            found=any;
        }
        int count = func_00194A10__n27((char *)&world__n27,4);
        if(count && count!=13) found=1;
    }
    if(!found) disabled[1]=1;
    if(game__n27.x288<3) {
        func_001F7420__n27(&menu__n27,1,15,0,1,0,disabled,2);
    } else {
        func_001F7420__n27(&menu__n27,1,15,0,0,0,disabled,2);
        disabled[1]=1;
        func_001F7B80();
    }
    func_001F74F0__n27(&menu__n27);
    func_001BAF10__n27(&game__n27,0);
    if(game__n27.x288==1) {
        for(i=menu__n27.x8;i<=menu__n27.xC;i++) {
            if(disabled[i]==1) {
                func_001F5398(0,0,0,40,85+i*15,labels__n27[i],192);
            } else {
                if(i==menu__n27.x10) {selected=1;color=2;} else {selected=0;color=1;}
                func_001F5398(0,selected,color,40,85+i*15,labels__n27[i],255);
            }
        }
    } else if(game__n27.x288>=2) {
        for(i=menu__n27.x8;i<=menu__n27.xC;i++) {
            if(disabled[i]==1) {
                func_001F5EE8__n27(0,0,0,530,95+i*15,labels__n27[i],192);
            } else {
                if(i==menu__n27.x10) {selected=1;color=2;} else {selected=0;color=1;}
                func_001F5EE8__n27(0,selected,color,530,95+i*15,labels__n27[i],255);
            }
        }
    }
    D_0029DF7C=0;
    func_001F4A98();
    if(D_0029DF87) {func_001F7550__n27(&menu__n27);return;}
    if(D_0029DF86) {func_001F75C0__n27(&menu__n27);return;}
    if(D_0029DF8B) {
        PlayMenuSelectLegacy(&menu__n27);
        D_0029DEEC=D_0029DED8;
        if(menu__n27.x10==0) {
            if(game__n27.x288==1) D_0029DED8=1;
            else if(game__n27.x288>=2) D_0029DED8=24;
        } else if(menu__n27.x10==1) {
            if(game__n27.x288==1) D_0029DED8=22;
            else if(game__n27.x288>=2) D_0029DED8=25;
        }
        func_001F57D0(0x2329,0);
        func_001F76B8__n27(&menu__n27);
        return;
    }
    if(D_0029DF89) {
        PlayMenuBackLegacy(&menu__n27);
        D_0029DEEC=D_0029DED8;
        if(game__n27.x288>=2) {
            game__n27.x288=1;
            D_0029DED8=10;
            D_0029DF98=0;
        } else {
            D_0029DED8=3;
            for(i=32;i<=224;i+=32) game__n27.x2A8[i/32-1]=-1;
            SelectAnimationResourceLegacy(&D_004C6370,0x2330,0,D_004C6910[0]+1);
            if(D_004C6910[0]==14) SelectAnimationResourceLegacy(&D_004C6370,0x2330,0,16);
            func_00203FE0(&D_004C6370,1);
            func_002038E0(&D_004C6370,0);
            func_001F50F0(0x2330,D_004C6910[0]+1);
            func_001F5100__n27();
            func_001F65A8__n27(1);
            func_001F65E0(0);
            func_001F60C8(RIDER);
            if(D_004C6910[0]!=14) {
                SelectAnimationResourceLegacy(D_00275BB0[RIDER],D_00275C30[RIDER],0,0);
                Value tmp=*D_00275BB0[RIDER];
                func_00203E10(&tmp,1.0f);
                tmp=*D_00275BB0[RIDER];
                func_002038E0(&tmp,1);
            }
            D_0029E01C=0;
            D_0029DF40=210;
            D_0029DF44=0;
        }
        if(game__n27.x288==1) func_001F6A08__n27(RIDER);
        func_001F76B8__n27(&menu__n27);
    }
}

/* ---- merged from src/unsorted/001F43C0.c ---- */
extern "C" {
typedef struct Flagged__c17 {
    char pad0[4];
    char x4;
} Flagged__c17;

extern int D_0029DF40__c17 asm("D_0029DF40");
extern int D_0029DF44__c17 asm("D_0029DF44");
extern char D_0029DEE9__c17 asm("D_0029DEE9");
extern char D_0029DFB4__c17 asm("D_0029DFB4");
extern Flagged__c17 *D_0029C468__c17 asm("D_0029C468");
extern Flagged__c17 *D_0029DD34__c17 asm("D_0029DD34");
extern int *D_0029E094__c17 asm("D_0029E094");
typedef struct Game__c17 {
    char pad0[0x10C];
    int mode;
} Game__c17;

extern Game__c17 D_004B25A0__c17 asm("D_004B25A0");
extern char D_004C5DB0__c17[] asm("D_004C5DB0");

extern void func_00224518__c17(char *) asm("func_00224518");
extern void func_002245C8__c17(void) asm("func_002245C8");
extern void func_001F5D08__c17(int, int, int, int, int, int, int) asm("func_001F5D08");

extern void func_001F43C0__c17(void) asm("func_001F43C0");
void func_001F43C0__c17(void) {
    int index;

    if (D_0029DF44__c17 == D_0029DF40__c17 - 5) {
        func_00224518__c17(D_004C5DB0__c17);
        func_002245C8__c17();
        D_0029DEE9__c17 = 0;
    }
    if (D_0029C468__c17 != 0) {
        D_0029C468__c17->x4 = 1;
    }
    if (D_0029DD34__c17 != 0) {
        D_0029DD34__c17->x4 = 1;
    }
    index = D_0029DF44__c17;
    if (index < D_0029DF40__c17) {
        D_0029DF44__c17 = ++index;
        index = 0;
        if (D_004B25A0__c17.mode == 15) {
            index = 1;
        } else if (D_004B25A0__c17.mode == 16) {
            index = 2;
        } else if (D_004B25A0__c17.mode == 17) {
            index = 3;
        } else if (D_004B25A0__c17.mode == 19) {
            index = 4;
        } else if (D_004B25A0__c17.mode == 13) {
            index = 5;
        } else if (D_004B25A0__c17.mode == 5) {
            index = 6;
        } else if (D_004B25A0__c17.mode == 11) {
            index = 7;
        }
        func_001F5D08__c17(0, 1, 2, 0x140, 100, D_0029E094__c17[index], 0xFF);
    } else {
        D_0029DFB4__c17 = 1;
    }
}
}
/* ---- merged from src/unsorted/001F4508.c ---- */
extern "C" {
typedef unsigned char u8__c18;
typedef struct Menu__c18 { int x0,x4,x8,xC,x10,x14,x18; } Menu__c18;
typedef struct Game__c18 { char pad0[0x288]; int x288; char pad28c[0x77c]; int xa08; } Game__c18;
extern Game__c18 D_004B25A0__c18 asm("D_004B25A0");
extern Menu__c18 D_004C6E58__c18 asm("D_004C6E58"),D_004C67C0__c18 asm("D_004C67C0");
extern char **D_0029E0D8__c18 asm("D_0029E0D8");
extern int D_0029DED4__c18 asm("D_0029DED4"),D_0029DED8__c18 asm("D_0029DED8"),D_0029DEEC__c18 asm("D_0029DEEC"),D_0029DF40__c18 asm("D_0029DF40"),D_0029DF44__c18 asm("D_0029DF44"),D_0029DF50__c18 asm("D_0029DF50");
extern int D_004C3778__c18[] asm("D_004C3778");
extern u8__c18 D_0029DF86__c18 asm("D_0029DF86"),D_0029DF87__c18 asm("D_0029DF87"),D_0029DF89__c18 asm("D_0029DF89"),D_0029DF8B__c18 asm("D_0029DF8B");
extern u8__c18 D_003F53FC__c18[] asm("D_003F53FC");
extern void *func_00254204__c18(void *,int,int) asm("func_00254204");
extern void func_001F7420__c18(Menu__c18 *,int,int,int,int,int,u8__c18 *,int) asm("func_001F7420");
extern void func_001F74F0__c18(Menu__c18 *) asm("func_001F74F0"),func_001F7550__c18(Menu__c18 *) asm("func_001F7550"),func_001F75C0__c18(Menu__c18 *) asm("func_001F75C0"),func_001F76B8__c18(Menu__c18 *) asm("func_001F76B8");
extern void func_001F5398__c18(int,int,int,int,int,char *,int) asm("func_001F5398"),func_001F5EE8__c18(int,int,int,int,int,char *,int) asm("func_001F5EE8");
extern void func_001BAF10__c18(Game__c18 *,int) asm("func_001BAF10"),func_001F4920__c18(void) asm("func_001F4920"),func_001F4A98__c18(void) asm("func_001F4A98"),func_001F5588__c18() asm("func_001F5588"),func_001F5578__c18(void) asm("func_001F5578"),func_001F5B08__c18(int) asm("func_001F5B08"),func_0018B4C8__c18() asm("func_0018B4C8");
extern int func_001F76D8__c18(Menu__c18 *,int) asm("func_001F76D8");
extern void func_001F4508__c18(void) asm("func_001F4508");
void func_001F4508__c18(void) {
    u8__c18 disabled[3];
    int index, selected, style, selection, x, tmp;
    func_00254204__c18(disabled,0,3);
    func_001F7420__c18(&D_004C6E58__c18,34,11,0,2,0,disabled,3);
    func_001F74F0__c18(&D_004C6E58__c18);
    func_001F5398__c18(0,0,1,30,10,D_0029E0D8__c18[0],255);
    func_001BAF10__c18(&D_004B25A0__c18,0);
    x = D_004B25A0__c18.x288 >= 2 ? 530 : 40;
    index = D_004C6E58__c18.x8;
    if (index <= D_004C6E58__c18.xC) {
        do {
        if (disabled[index] == 1) {
            if (D_004B25A0__c18.x288 >= 2)
                func_001F5EE8__c18(0,0,0,x,95+index*15,D_0029E0D8__c18[index+1],192);
            else
                func_001F5398__c18(0,0,0,x,95+index*15,D_0029E0D8__c18[index+1],192);
        } else {
            selected = 0;
            if (index == D_004C6E58__c18.x10) { selected = 1; style = 2; }
            else style = 1;
            if (D_004B25A0__c18.x288 >= 2)
                func_001F5EE8__c18(0,selected,style,x,95+index*15,D_0029E0D8__c18[index+1],255);
            else
                func_001F5398__c18(0,selected,style,x,95+index*15,D_0029E0D8__c18[index+1],255);
        }
        index++;
        } while (index <= D_004C6E58__c18.xC);
    }
    func_001F4920__c18();
    func_001F4A98__c18();
    if (D_0029DF87__c18) { func_001F7550__c18(&D_004C6E58__c18); return; }
    if (D_0029DF86__c18) { func_001F75C0__c18(&D_004C6E58__c18); return; }
    if (D_0029DF8B__c18) {
        PlayMenuSelectLegacy(&D_004C6E58__c18);
        D_0029DEEC__c18 = 30;
        D_0029DED8__c18 = D_004C6E58__c18.x18;
        selection = D_004C6E58__c18.x10;
        switch (selection) {
        case 0:
            D_004C3778__c18[0] = 0;
            break;
        case 1:
            D_004C3778__c18[0] = selection;
            break;
        case 2:
            D_004C3778__c18[0] = selection;
            break;
        }
        if (D_004B25A0__c18.x288 == 1) {
            func_001F5588__c18();
            D_0029DF50__c18 = 2;
            D_0029DF40__c18 = 210;
            D_0029DF44__c18 = 0;
            func_001F5578__c18();
            func_001F5B08__c18(1);
            D_0029DED4__c18 = func_001F76D8__c18(&D_004C67C0__c18,0);
            D_004B25A0__c18.xa08 = D_0029DEEC__c18;
            func_001F76B8__c18(&D_004C6E58__c18);
        } else {
            func_0018B4C8__c18();
            if (D_003F53FC__c18[0]) {
                tmp = D_0029DEEC__c18;
                D_0029DEEC__c18 = D_0029DED8__c18;
                D_0029DED8__c18 = tmp;
            } else {
                D_0029DEEC__c18 = 8;
                D_0029DED8__c18 = 9;
                D_0029DF40__c18 = 120;
                D_0029DF44__c18 = 0;
            }
        }
        func_001F76B8__c18(&D_004C6E58__c18);
        return;
    }
    if (D_0029DF89__c18) {
        PlayMenuBackLegacy(&D_004C6E58__c18);
        D_0029DEEC__c18 = D_0029DED8__c18;
        D_0029DED8__c18 = D_004C6E58__c18.x14;
        func_001F76B8__c18(&D_004C6E58__c18);
    }
}
}
/* ---- merged from src/unsorted/001F4920.c ---- */
extern "C" {
extern int D_0029DF68__c19 asm("D_0029DF68");
extern int D_0029DF7C__c19 asm("D_0029DF7C");
extern void func_001F4920__c19(void) asm("func_001F4920");
void func_001F4920__c19(void) {
    int value = D_0029DF68__c19;
    if (value == 0) {
        D_0029DF7C__c19 = 0;
        return;
    }
    if (value == 1) goto store;
    if (value == 2) goto store;
    if (value != 3) return;
store:
    D_0029DF7C__c19 = value;
}
}
/* ---- merged from src/unsorted/001F4960.c ---- */
extern "C" {
typedef struct Value__c20 {
    int x[4];
} Value__c20;

extern int D_0029DF68__c20 asm("D_0029DF68");
extern int D_0029DED8__c20 asm("D_0029DED8");
extern int D_004C6910__c20[] asm("D_004C6910");
extern Value__c20 *D_00275B70__c20[] asm("D_00275B70");
extern char D_00296CF8__c20[] asm("D_00296CF8");
extern char D_00296D10__c20[] asm("D_00296D10");

extern void func_001F5D08__c20(int, int, int, int, int, char *, int) asm("func_001F5D08");
extern void func_001F57D0__c20(int, int) asm("func_001F57D0");
extern float func_00203C28__c20(Value__c20 *) asm("func_00203C28");

extern void func_001F4960__c20(void) asm("func_001F4960");
void func_001F4960__c20(void) {
    int type;
    Value__c20 value;

    type = D_004C6910__c20[D_0029DF68__c20];
    if (type == 7 || type == 8 || type == 14) {
        func_001F5D08__c20(0, 1, 2, 0xA5, 0xA5, D_00296CF8__c20, 0xFF);
    } else {
        func_001F5D08__c20(0, 1, 2, 0xA5, 0xA5, D_00296D10__c20, 0xFF);
    }
    func_001F57D0__c20(0x233D, 1);
    value = *D_00275B70__c20[D_004C6910__c20[D_0029DF68__c20]];
    if (func_00203C28__c20(&value) >= 1.0f) {
        D_0029DED8__c20 = 3;
        func_001F57D0__c20(0x233D, 0);
    }
}
}
/* ---- merged from src/unsorted/001F4A98.c ---- */
extern "C" {
extern unsigned int D_0029DF7C__c21 asm("D_0029DF7C");
extern int D_0029DF80__c21 asm("D_0029DF80"),D_0029DF90__c21 asm("D_0029DF90");
extern unsigned char D_0029DF84__c21 asm("D_0029DF84"),D_0029DF85__c21 asm("D_0029DF85"),D_0029DF86__c21 asm("D_0029DF86"),D_0029DF87__c21 asm("D_0029DF87"),D_0029DF88__c21 asm("D_0029DF88"),D_0029DF89__c21 asm("D_0029DF89"),D_0029DF8A__c21 asm("D_0029DF8A"),D_0029DF8B__c21 asm("D_0029DF8B"),D_0029DF8C__c21 asm("D_0029DF8C"),D_0029DF94__c21 asm("D_0029DF94"),D_0029DF95__c21 asm("D_0029DF95"),D_0029DF96__c21 asm("D_0029DF96"),D_0029DF97__c21 asm("D_0029DF97");
extern int func_002205A0__c21(int) asm("func_002205A0"),func_00220618__c21(int,int) asm("func_00220618");
extern void func_001F4A98__c21(void) asm("func_001F4A98");
void func_001F4A98__c21(void) {
 int first=-1,last=-1,state;
 D_0029DF80__c21=-1;D_0029DF90__c21=0;
 D_0029DF87__c21=0;
 D_0029DF86__c21=0;
 D_0029DF84__c21=0;
 D_0029DF85__c21=0;
 D_0029DF88__c21=0;
 D_0029DF89__c21=0;
 D_0029DF8A__c21=0;
 D_0029DF8B__c21=0;
 D_0029DF8C__c21=0;
 if(D_0029DF7C__c21<4U) {last=D_0029DF7C__c21;first=last;} else if(D_0029DF7C__c21-4U<3U) {last=D_0029DF7C__c21-3;first=0;}
 D_0029DF80__c21=first;
 while(D_0029DF80__c21<=last) {
  state=func_002205A0__c21(D_0029DF80__c21);
  if(func_00220618__c21(0x40,state)) {D_0029DF90__c21=D_0029DF87__c21=1;}
  else if(func_00220618__c21(0x10,state)) {D_0029DF90__c21=D_0029DF86__c21=1;}
  else if(func_00220618__c21(0x4000,state)) {D_0029DF90__c21=D_0029DF8B__c21=1;}
  else if(func_00220618__c21(0x80,state)) {D_0029DF90__c21=D_0029DF84__c21=1;}
  else if(func_00220618__c21(0x20,state)) {D_0029DF90__c21=D_0029DF85__c21=1;}
  else if(func_00220618__c21(0x8000,state)) {D_0029DF90__c21=D_0029DF88__c21=1;}
  else if(func_00220618__c21(0x1000,state)) {D_0029DF90__c21=D_0029DF89__c21=1;}
  else if(func_00220618__c21(0x2000,state)) {D_0029DF90__c21=D_0029DF8A__c21=1;}
  else if(func_00220618__c21(0x400,state)) {D_0029DF90__c21=D_0029DF94__c21=1;}
  else if(func_00220618__c21(0x800,state)) {D_0029DF90__c21=D_0029DF95__c21=1;}
  else if(func_00220618__c21(0x100,state)) {D_0029DF90__c21=D_0029DF96__c21=1;}
  else if(func_00220618__c21(0x200,state)) {D_0029DF90__c21=D_0029DF97__c21=1;}
  else if(func_00220618__c21(0x1,state)) {D_0029DF90__c21=D_0029DF8C__c21=1;}
  D_0029DF80__c21++;
 }
}
}
INCLUDE_RODATA("build/rodata", D_00296C70);
INCLUDE_RODATA("build/rodata", D_00296C80);
INCLUDE_RODATA("build/rodata", D_00296C90);
INCLUDE_RODATA("build/rodata", D_00296CA0);
INCLUDE_RODATA("build/rodata", D_00296CE0);
INCLUDE_RODATA("build/rodata", D_00296CF8);
INCLUDE_RODATA("build/rodata", D_00296D10);
/* ---- merged from src/unsorted/001F4CE0.c ---- */
extern "C" {
extern int D_0029DEE0__c22 asm("D_0029DEE0");
extern int D_0029DEE4__c22 asm("D_0029DEE4");
extern char D_0029DEE8__c22 asm("D_0029DEE8");
extern char D_00275488__c22[] asm("D_00275488");
extern char D_002754A0__c22[] asm("D_002754A0");
extern char D_002754B8__c22[] asm("D_002754B8");
extern char D_002754D0__c22[] asm("D_002754D0");
extern char D_002754E8__c22[] asm("D_002754E8");
extern int func_00223BB0__c22(char *name, int flag) asm("func_00223BB0");
extern int func_00238F48__c22(void) asm("func_00238F48");

extern void func_001F4CE0__c22(void) asm("func_001F4CE0");
void func_001F4CE0__c22(void)
{
    if (D_0029D7EC == 0) {
        return;
    }
    D_0029DEE8__c22 = 0;
    switch (D_0029DEE0__c22) {
    case 1:
        func_00223BB0__c22(D_00275488__c22, 0);
        D_0029DEE0__c22 = 2;
        D_0029DEE4__c22 = func_00238F48__c22();
        D_0029DEE8__c22 = 1;
        break;
    case 2:
        func_00223BB0__c22(D_002754A0__c22, 0);
        D_0029DEE0__c22 = 3;
        D_0029DEE4__c22 = func_00238F48__c22();
        D_0029DEE8__c22 = 1;
        break;
    case 3:
        func_00223BB0__c22(D_002754B8__c22, 1);
        D_0029DEE0__c22 = 5;
        D_0029DEE4__c22 = func_00238F48__c22();
        break;
    case 4:
        func_00223BB0__c22(D_002754D0__c22, 0);
        D_0029DEE0__c22 = 0;
        D_0029DEE4__c22 = func_00238F48__c22();
        break;
    case 5:
        func_00223BB0__c22(D_002754E8__c22, 1);
        D_0029DEE0__c22 = 0;
        D_0029DEE4__c22 = func_00238F48__c22();
        break;
    }
    D_0029D7EC = 0;
}

/* Not used here: read by func_001F60C8__c22/func_001F6338__c22/func_001F7DE0__c22. It follows this
   function's jump table directly in retail .rodata (no 16-byte padding in between),
   so the original file defined both. */
extern const int D_00296D48[15] = { 0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 14, 15, 17 };
}
/* ---- merged from src/unsorted/001F4DD8.c ---- */
extern "C" {
struct Table__c23 {
    char pad0[0xA0C];
    int xA0C[1048][10];
    char padADCC[0xADD0 - 0xADCC];
    int xADD0[1];
};

extern struct Table__c23 D_004B25A0__c23 asm("D_004B25A0");
extern int D_0029DEDC__c23 asm("D_0029DEDC");
extern int D_0029DED8__c23 asm("D_0029DED8");
extern void func_00218498__c23(int, int) asm("func_00218498");

extern void func_001F4DD8__c23(void) asm("func_001F4DD8");
void func_001F4DD8__c23(void)
{
    int idx = D_0029DEDC__c23;

    D_0029DED8__c23 = idx;
    func_00218498__c23(D_004B25A0__c23.xA0C[idx][D_004B25A0__c23.xADD0[idx]], 1);
    D_0029D800 = 0;
}
}
/* ---- merged from src/unsorted/001F4E40.c ---- */
extern "C" {
struct Context__c24 {
char pad_0[7824];char *x1E90;
char pad_1E94[44];char *x1EC0;
char pad_1EC4[428];char *x2070;
char pad_2074[44];char *x20A0;
char pad_20A4[428];char *x2250;
char pad_2254[44];char *x2280;
char pad_2284[428];char *x2430;
char pad_2434[44];char *x2460;
char pad_2464[428];char *x2610;
char pad_2614[44];char *x2640;
char pad_2644[428];char *x27F0;
char pad_27F4[44];char *x2820;
char pad_2824[428];char *x29D0;
char pad_29D4[44];char *x2A00;
char pad_2A04[428];char *x2BB0;
char pad_2BB4[44];char *x2BE0;
char pad_2BE4[428];char *x2D90;
char pad_2D94[44];char *x2DC0;
char pad_2DC4[428];char *x2F70;
char pad_2F74[44];char *x2FA0;
char pad_2FA4[428];char *x3150;
char pad_3154[44];char *x3180;
char pad_3184[428];char *x3330;
char pad_3334[44];char *x3360;
char pad_3364[428];char *x3510;
char pad_3514[44];char *x3540;
char pad_3544[428];char *x36F0;
};
extern struct Context__c24 D_004B25A0__c24 asm("D_004B25A0");
extern void func_00207E68__c24(void *,void *,void *) asm("func_00207E68");
extern char D_004C5E50__c24[] asm("D_004C5E50");
extern char D_004C5E80__c24[] asm("D_004C5E80");
extern char D_004C5EB0__c24[] asm("D_004C5EB0");
extern char D_004C5EE0__c24[] asm("D_004C5EE0");
extern char D_004C5F10__c24[] asm("D_004C5F10");
extern char D_004C5F40__c24[] asm("D_004C5F40");
extern char D_004C5FD0__c24[] asm("D_004C5FD0");
extern char D_004C6000__c24[] asm("D_004C6000");
extern char D_004C5F70__c24[] asm("D_004C5F70");
extern char D_004C5FA0__c24[] asm("D_004C5FA0");
extern char D_004C6030__c24[] asm("D_004C6030");
extern char D_004C6060__c24[] asm("D_004C6060");
extern char D_004C6270__c24[] asm("D_004C6270");
extern char D_004C62A0__c24[] asm("D_004C62A0");
extern char D_004C6090__c24[] asm("D_004C6090");
extern char D_004C60C0__c24[] asm("D_004C60C0");
extern char D_004C60F0__c24[] asm("D_004C60F0");
extern char D_004C6120__c24[] asm("D_004C6120");
extern char D_004C6150__c24[] asm("D_004C6150");
extern char D_004C6180__c24[] asm("D_004C6180");
extern char D_004C61B0__c24[] asm("D_004C61B0");
extern char D_004C61E0__c24[] asm("D_004C61E0");
extern char D_004C6210__c24[] asm("D_004C6210");
extern char D_004C6240__c24[] asm("D_004C6240");
extern char D_004C62D0__c24[] asm("D_004C62D0");
extern char D_004C6300__c24[] asm("D_004C6300");
extern char D_004C6330__c24[] asm("D_004C6330");
extern void func_001F4E40__c24(void) asm("func_001F4E40");
void func_001F4E40__c24(void) {
func_00207E68__c24(D_004C5E50__c24,D_004B25A0__c24.x1E90+0x40,D_004B25A0__c24.x1E90+0x10);
func_00207E68__c24(D_004C5E80__c24,D_004B25A0__c24.x1EC0+0x40,D_004B25A0__c24.x1EC0+0x10);
func_00207E68__c24(D_004C5EB0__c24,D_004B25A0__c24.x2070+0x40,D_004B25A0__c24.x2070+0x10);
func_00207E68__c24(D_004C5EE0__c24,D_004B25A0__c24.x20A0+0x40,D_004B25A0__c24.x20A0+0x10);
func_00207E68__c24(D_004C5F10__c24,D_004B25A0__c24.x2250+0x40,D_004B25A0__c24.x2250+0x10);
func_00207E68__c24(D_004C5F40__c24,D_004B25A0__c24.x2280+0x40,D_004B25A0__c24.x2280+0x10);
func_00207E68__c24(D_004C5FD0__c24,D_004B25A0__c24.x2430+0x40,D_004B25A0__c24.x2430+0x10);
func_00207E68__c24(D_004C6000__c24,D_004B25A0__c24.x2460+0x40,D_004B25A0__c24.x2460+0x10);
func_00207E68__c24(D_004C5F70__c24,D_004B25A0__c24.x2610+0x40,D_004B25A0__c24.x2610+0x10);
func_00207E68__c24(D_004C5FA0__c24,D_004B25A0__c24.x2640+0x40,D_004B25A0__c24.x2640+0x10);
func_00207E68__c24(D_004C6030__c24,D_004B25A0__c24.x27F0+0x40,D_004B25A0__c24.x27F0+0x10);
func_00207E68__c24(D_004C6060__c24,D_004B25A0__c24.x2820+0x40,D_004B25A0__c24.x2820+0x10);
func_00207E68__c24(D_004C6270__c24,D_004B25A0__c24.x29D0+0x40,D_004B25A0__c24.x29D0+0x10);
func_00207E68__c24(D_004C62A0__c24,D_004B25A0__c24.x2A00+0x40,D_004B25A0__c24.x2A00+0x10);
func_00207E68__c24(D_004C6090__c24,D_004B25A0__c24.x2BB0+0x40,D_004B25A0__c24.x2BB0+0x10);
func_00207E68__c24(D_004C60C0__c24,D_004B25A0__c24.x2BE0+0x40,D_004B25A0__c24.x2BE0+0x10);
func_00207E68__c24(D_004C60F0__c24,D_004B25A0__c24.x2D90+0x40,D_004B25A0__c24.x2D90+0x10);
func_00207E68__c24(D_004C6120__c24,D_004B25A0__c24.x2DC0+0x40,D_004B25A0__c24.x2DC0+0x10);
func_00207E68__c24(D_004C6150__c24,D_004B25A0__c24.x2F70+0x40,D_004B25A0__c24.x2F70+0x10);
func_00207E68__c24(D_004C6180__c24,D_004B25A0__c24.x2FA0+0x40,D_004B25A0__c24.x2FA0+0x10);
func_00207E68__c24(D_004C61B0__c24,D_004B25A0__c24.x3150+0x40,D_004B25A0__c24.x3150+0x10);
func_00207E68__c24(D_004C61E0__c24,D_004B25A0__c24.x3180+0x40,D_004B25A0__c24.x3180+0x10);
func_00207E68__c24(D_004C6210__c24,D_004B25A0__c24.x3330+0x40,D_004B25A0__c24.x3330+0x10);
func_00207E68__c24(D_004C6240__c24,D_004B25A0__c24.x3360+0x40,D_004B25A0__c24.x3360+0x10);
func_00207E68__c24(D_004C62D0__c24,D_004B25A0__c24.x3510+0x40,D_004B25A0__c24.x3510+0x10);
func_00207E68__c24(D_004C6300__c24,D_004B25A0__c24.x3540+0x40,D_004B25A0__c24.x3540+0x10);
func_00207E68__c24(D_004C6330__c24,D_004B25A0__c24.x36F0+0x40,D_004B25A0__c24.x36F0+0x10);
}
}
/* ---- merged from src/unsorted/001F50F0.c ---- */
extern "C" {
extern int D_0029E028__c25 asm("D_0029E028");
extern int D_0029E02C__c25 asm("D_0029E02C");

extern void func_001F50F0__c25(int a, int b) asm("func_001F50F0");
void func_001F50F0__c25(int a, int b)
{
    D_0029E028__c25 = a;
    D_0029E02C__c25 = b;
}
}
/* ---- merged from src/unsorted/001F5100.c ---- */
extern "C" {
typedef struct { int x[4]; } Value__c5100;
extern Value__c5100 *D_00275B70__c5100[] asm("D_00275B70");
extern int D_00275BF0__c5100[] asm("D_00275BF0");
extern int D_004C6780__c5100[] asm("D_004C6780");
typedef struct { char pad0[0x3C]; unsigned char selectable[32]; } RiderFlag__c5100;
extern char D_004B25A0__c5100[] asm("D_004B25A0");
extern void func_00203E10__c5100(Value__c5100 *, float) asm("func_00203E10");
extern void func_002038E0__c5100(Value__c5100 *, int) asm("func_002038E0");
extern void func_00203CB8__c5100(Value__c5100 *) asm("func_00203CB8");
extern void func_00203A80__c5100(Value__c5100 *, int, float) asm("func_00203A80");
extern void func_001F57D0__c5100(int, int) asm("func_001F57D0");
extern void func_001F5100__c5100(void) asm("func_001F5100");
void func_001F5100__c5100(void)
{
    int i, j;
    Value__c5100 v;
    for (i = 1; i < 15; i++) {
        if (((RiderFlag__c5100 *)D_004B25A0__c5100)->selectable[i] == 1 && !D_004C6780__c5100[i]) {
            func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c5100[i]), D_00275BF0__c5100[i], 0, D_0029D90C);
            v = *D_00275B70__c5100[i];
            func_00203E10__c5100(&v, 1.0f);
            v = *D_00275B70__c5100[i];
            func_002038E0__c5100(&v, 1);
        } else {
            for (j = 0; j < 6; j++) {
                func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c5100[i]), D_00275BF0__c5100[i], 0, j);
                v = *D_00275B70__c5100[i];
                func_00203CB8__c5100(&v);
            }
            func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c5100[i]), D_00275BF0__c5100[i], 0, D_0029D91C);
            v = *D_00275B70__c5100[i];
            func_00203A80__c5100(&v, 1, 1.0f);
        }
        if (i == 2)
            func_001F57D0__c5100(0x235F, 0);
    }
}
}
/* ---- merged from src/unsorted/001F5320.c ---- */
extern "C" {
extern int func_001F5320__c26(int step, int lo, int hi, int index, unsigned char *flags) asm("func_001F5320");
int func_001F5320__c26(int step, int lo, int hi, int index, unsigned char *flags) {
    int count, range, blocked;
    long lower;

    index += step;
    if (index > hi) index = lo; else index = (index < lo) ? hi : index;
    range = hi - lo;
    count = 0;
    blocked = 1;
    lower = lo;
    while (flags[index] == blocked) {
        index += step;
        if (index > hi) index = lo; else index = (index < lower) ? hi : index;
        count++;
        if (count > range) { index = -1; break; }
    }
    return index;
}
}
/* ---- merged from src/unsorted/001F5398.c ---- */
extern "C" {
struct Style__c27 { int colors[2][3][3]; int x48,x4C; float x50,x54; };
extern struct Style__c27 D_004C6560__c27[] asm("D_004C6560");
extern int D_0029DFF4__c27 asm("D_0029DFF4"),D_0029DFF8__c27 asm("D_0029DFF8"),D_0029DFFC__c27 asm("D_0029DFFC");
extern void func_0020F518__c27(int,int,int,int,int) asm("func_0020F518");
extern void func_0020FB80__c27(int,int,int,int,int) asm("func_0020FB80");
extern void func_0020F9A8__c27(int,int,int,int,void*) asm("func_0020F9A8");
extern void func_001F5398__c27(int style,int group,int color,int x,int y,void *text,int target) asm("func_001F5398");
void func_001F5398__c27(int style,int group,int color,int x,int y,void *text,int target) {
 int alpha=255;
 if(target==255) alpha=D_0029DFF4__c27;
 else if(target==192) alpha=D_0029DFF8__c27;
 if(alpha<target) { alpha+=D_0029DFFC__c27; if(alpha>target) alpha=target; }
 else if(alpha>target) { alpha-=D_0029DFFC__c27; if(alpha<0) alpha=0; }
 if((unsigned)(style-3)<2) {
  func_0020F518__c27(0,D_004C6560__c27[style].colors[group][color][0],D_004C6560__c27[style].colors[group][color][1],D_004C6560__c27[style].colors[group][color][2],target);
 } else {
  func_0020F518__c27(0,D_004C6560__c27[style].colors[group][color][0],D_004C6560__c27[style].colors[group][color][1],D_004C6560__c27[style].colors[group][color][2],alpha);
 }
 func_0020FB80__c27(0,D_004C6560__c27[style].x48,D_004C6560__c27[style].x4C,(int)D_004C6560__c27[style].x50,(int)D_004C6560__c27[style].x54);
 func_0020F9A8__c27(0,0,x,y,text);
 if(target==255) D_0029DFF4__c27=alpha;
 else if(target==192) D_0029DFF8__c27=alpha;
}
}
/* ---- merged from src/unsorted/001F5578.c ---- */
extern "C" {
extern int D_0029DFF4__c28 asm("D_0029DFF4");
extern int D_0029DFF8__c28 asm("D_0029DFF8");

extern void func_001F5578__c28(void) asm("func_001F5578");
void func_001F5578__c28(void)
{
    D_0029DFF4__c28 = 0;
    D_0029DFF8__c28 = 0;
}
}
/* ---- merged from src/unsorted/001F5588.c ---- */
extern "C" {
typedef struct Value__c29 { int x[4]; } Value__c29;
extern Value__c29 *D_00275B70__c29[] asm("D_00275B70"), *D_00275BB0__c29[] asm("D_00275BB0");
extern int D_00275C28__c29[] asm("D_00275C28"), D_00275C30__c29[] asm("D_00275C30"), D_00275CB0__c29[] asm("D_00275CB0");
extern int D_0029DF68__c29 asm("D_0029DF68"), D_004C6910__c29[] asm("D_004C6910");
extern Value__c29 D_004C6370__c29 asm("D_004C6370");
extern void func_00203E10__c29(Value__c29 *, float) asm("func_00203E10");
extern void func_002038E0__c29(Value__c29 *, int) asm("func_002038E0");
extern void func_001F5588__c29(void) asm("func_001F5588");
void func_001F5588__c29(void) {
    Value__c29 value;
    int type = D_004C6910__c29[D_0029DF68__c29];
    if (type == 14) {
        func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c29[14]), D_00275C28__c29[0], 0, D_0029D918);
        value = *D_00275B70__c29[D_004C6910__c29[D_0029DF68__c29]];
        func_00203E10__c29(&value, 1.0f);
        value = *D_00275B70__c29[D_004C6910__c29[D_0029DF68__c29]];
        func_002038E0__c29(&value, 1);
    } else {
        func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c29[type]), D_00275C30__c29[type], 0, D_0029D918);
        value = *D_00275BB0__c29[D_004C6910__c29[D_0029DF68__c29]];
        func_00203E10__c29(&value, 1.0f);
        value = *D_00275BB0__c29[D_004C6910__c29[D_0029DF68__c29]];
        func_002038E0__c29(&value, 1);
    }
    func_00203590((struct AnimationHandle *)(void *)(&D_004C6370__c29), 0x2330, 0, D_00275CB0__c29[D_004C6910__c29[D_0029DF68__c29]]);
    func_00203E10__c29(&D_004C6370__c29, 1.0f);
    func_002038E0__c29(&D_004C6370__c29, 1);
}
}
/* ---- merged from src/unsorted/001F57D0.c ---- */
extern "C" {
struct Global__c30 { char pad[0xa0c]; int xA0C[10][10]; };
extern struct Global__c30 D_004B25A0__c30 asm("D_004B25A0");
extern void func_00218498__c30(int, int) asm("func_00218498");
extern void func_001F57D0__c30(int arg0, int arg1) asm("func_001F57D0");
void func_001F57D0__c30(int arg0, int arg1) {
    int index = arg0 - 9000;
    func_00218498__c30(D_004B25A0__c30.xA0C[index / 10][index % 10], arg1);
}
}
/* ---- merged from src/unsorted/001F5828.c ---- */
extern "C" {
extern int *D_0029E0A0__c31 asm("D_0029E0A0");
extern int *D_0029E0A4__c31 asm("D_0029E0A4");
extern int *D_0029E0A8__c31 asm("D_0029E0A8");
extern int *D_0029E0AC__c31 asm("D_0029E0AC");
extern int *D_0029E0B0__c31 asm("D_0029E0B0");
extern int *D_0029E098__c31 asm("D_0029E098");
extern int *D_0029E09C__c31 asm("D_0029E09C");
extern int *D_0029E0B4__c31 asm("D_0029E0B4");
extern int *D_0029E0B8__c31 asm("D_0029E0B8");
extern int *D_0029E0BC__c31 asm("D_0029E0BC");
extern int *D_0029E0C0__c31 asm("D_0029E0C0");
extern int *D_0029E0C4__c31 asm("D_0029E0C4");
extern int *D_0029E0D4__c31 asm("D_0029E0D4");
extern int *D_0029E0C8__c31 asm("D_0029E0C8");
extern int *D_0029E0D0__c31 asm("D_0029E0D0");
extern int *D_0029E0CC__c31 asm("D_0029E0CC");
extern void func_001F5398__c31(int,int,int,int,int,int,int) asm("func_001F5398");
extern void func_001F5D08__c31(int,int,int,int,int,int,int) asm("func_001F5D08");
extern void func_001F5EE8__c31(int,int,int,int,int,int,int) asm("func_001F5EE8");
extern void func_001F5828__c31(int index) asm("func_001F5828");
void func_001F5828__c31(int index) {
 func_001F5398__c31(3,0,1,0x1E,0x3D,D_0029E0A0__c31[index],255);
 func_001F5398__c31(3,0,1,0x1E,0x58,D_0029E0A4__c31[index],255);
 func_001F5398__c31(3,0,1,0x1E,0x73,D_0029E0A8__c31[index],255);
 func_001F5398__c31(3,0,1,0x1E,0x86,D_0029E0AC__c31[index],255);
 func_001F5398__c31(3,0,1,0x1E,0xA4,D_0029E0B0__c31[index],255);
 func_001F5EE8__c31(3,0,1,0x14C,0x6C,D_0029E098__c31[index],255);
 func_001F5EE8__c31(3,0,1,0x262,0x41,D_0029E09C__c31[index],255);
 func_001F5EE8__c31(3,0,1,0x262,0x53,D_0029E0B4__c31[index],255);
 func_001F5EE8__c31(3,0,1,0x262,0x6C,D_0029E0B8__c31[index],255);
 func_001F5EE8__c31(3,0,1,0x262,0x78,D_0029E0BC__c31[index],255);
 func_001F5EE8__c31(3,0,1,0x262,0x82,D_0029E0C0__c31[index],255);
 func_001F5398__c31(3,0,1,0x1FE,0x97,D_0029E0C4__c31[index],255);
 func_001F5398__c31(3,0,1,0x1C2,0xAA,D_0029E0D4__c31[index],255);
 func_001F5D08__c31(3,0,1,0xF0,0xAC,D_0029E0C8__c31[index],255);
 func_001F5D08__c31(3,0,1,0x140,0xAC,D_0029E0D0__c31[index],255);
 func_001F5D08__c31(3,0,1,0x195,0xAC,D_0029E0CC__c31[index],255);
}
}
/* ---- merged from src/unsorted/001F5B08.c ---- */
extern "C" {
struct Actor__c32 { char pad[0xA8]; float xA8; };
struct Object__c32 { char pad[32]; struct Actor__c32 *actor; char rest[0x7080-36]; };
extern struct Object__c32 D_004B3D20__c32[] asm("D_004B3D20");
extern int D_004C6910__c32[] asm("D_004C6910");
extern int D_0029DF68__c32 asm("D_0029DF68");
extern void func_00221F50__c32(void *,void *) asm("func_00221F50");
extern void func_001F57D0__c32(int,int) asm("func_001F57D0");
extern void func_001F5B08__c32(int enabled) asm("func_001F5B08");
void func_001F5B08__c32(int enabled) {
 float first=-189.0f; float angles[15]={0.0f,first,135.6699981689453f,122.97000122070312f,80.69400024414062f,48.43000030517578f,14.979999542236328f,-3.4679999351501465f,-29.849000930786133f,-60.23899841308594f,-98.16300201416016f,-113.81900024414062f,-116.30000305175781f,191.0f,-171.16900634765625f};
 struct Object__c32 *p;
 if(enabled) {
 D_004B3D20__c32[0].actor->xA8=angles[D_004C6910__c32[D_0029DF68__c32]]*0.017453292f;
 func_00221F50__c32((char *)D_004B3D20__c32[0].actor+0x40,(char *)D_004B3D20__c32[0].actor+0xA0);
 p=(struct Object__c32 *)((int)D_004B3D20__c32+0x7080);

 p->actor->xA8=angles[D_004C6910__c32[D_0029DF68__c32]]*0.017453292f;
 func_00221F50__c32((char *)p->actor+0x40,(char *)p->actor+0xA0);
 func_001F57D0__c32(0x2335,1);
 } else {
 D_004B3D20__c32[0].actor->xA8=0;
 func_00221F50__c32((char *)D_004B3D20__c32[0].actor+0x40,(char *)D_004B3D20__c32[0].actor+0xA0);
 p=(struct Object__c32 *)((int)D_004B3D20__c32+0x7080);

 p->actor->xA8=0;
 func_00221F50__c32((char *)p->actor+0x40,(char *)p->actor+0xA0);
 func_001F57D0__c32(0x2335,0);
 }
}
}
/* ---- merged from src/unsorted/001F5D08.c ---- */
extern "C" {
struct Style__c33 { int colors[2][3][3]; int x48,x4C; float x50,x54; };
extern struct Style__c33 D_004C6560__c33[] asm("D_004C6560");
extern int D_0029DFF4__c33 asm("D_0029DFF4"),D_0029DFF8__c33 asm("D_0029DFF8"),D_0029DFFC__c33 asm("D_0029DFFC");
extern void func_0020F518__c33(int,int,int,int,int) asm("func_0020F518");
extern void func_0020FB80__c33(int,int,int,int,int) asm("func_0020FB80");
extern void func_0020F9F0__c33(int,int,int,int,void*) asm("func_0020F9F0");
extern void func_001F5D08__c33(int style,int group,int color,int x,int y,void *text,int target) asm("func_001F5D08");
void func_001F5D08__c33(int style,int group,int color,int x,int y,void *text,int target) {
 int alpha=255;
 if(target==255) alpha=D_0029DFF4__c33;
 else if(target==192) alpha=D_0029DFF8__c33;
 if(alpha<target) { alpha+=D_0029DFFC__c33; if(alpha>target) alpha=target; }
 else if(alpha>target) { alpha-=D_0029DFFC__c33; if(alpha<0) alpha=0; }
 if((unsigned)(style-3)<2) {
  func_0020F518__c33(0,D_004C6560__c33[style].colors[group][color][0],D_004C6560__c33[style].colors[group][color][1],D_004C6560__c33[style].colors[group][color][2],target);
 } else {
  func_0020F518__c33(0,D_004C6560__c33[style].colors[group][color][0],D_004C6560__c33[style].colors[group][color][1],D_004C6560__c33[style].colors[group][color][2],alpha);
 }
 func_0020FB80__c33(0,D_004C6560__c33[style].x48,D_004C6560__c33[style].x4C,(int)D_004C6560__c33[style].x50,(int)D_004C6560__c33[style].x54);
 func_0020F9F0__c33(0,0,x,y,text);
 if(target==255) D_0029DFF4__c33=alpha;
 else if(target==192) D_0029DFF8__c33=alpha;
}
}
/* ---- merged from src/unsorted/001F5EE8.c ---- */
extern "C" {
extern int D_0029DFF4__c34 asm("D_0029DFF4"), D_0029DFF8__c34 asm("D_0029DFF8");
int D_0029DFFC;
typedef struct { int rgb[2][3][3]; int x48,x4c; float x50,x54; } Entry__c34;
extern Entry__c34 D_004C6560__c34[] asm("D_004C6560");
extern void func_0020F518__c34(int,int,int,int,int) asm("func_0020F518");
extern void func_0020FB80__c34(int,int,int,int,int) asm("func_0020FB80");
extern void func_0020FA98__c34(int,int,int,int,int) asm("func_0020FA98");
extern void func_001F5EE8__c34(int index,int row,int col,int x,int y,int z,int target) asm("func_001F5EE8");
void func_001F5EE8__c34(int index,int row,int col,int x,int y,int z,int target) {
 int value=255;
 if(target==255) value=D_0029DFF4__c34;
 else if(target==192) value=D_0029DFF8__c34;
 if(value<target) { value+=D_0029DFFC; if(value>target) value=target; }
 else if(value>target) { value-=D_0029DFFC; if(value<0) value=0; }
 if((unsigned)(index-3)<2) func_0020F518__c34(0,D_004C6560__c34[index].rgb[row][col][0],D_004C6560__c34[index].rgb[row][col][1],D_004C6560__c34[index].rgb[row][col][2],target);
 else func_0020F518__c34(0,D_004C6560__c34[index].rgb[row][col][0],D_004C6560__c34[index].rgb[row][col][1],D_004C6560__c34[index].rgb[row][col][2],value);
 func_0020FB80__c34(0,D_004C6560__c34[index].x48,D_004C6560__c34[index].x4c,(int)D_004C6560__c34[index].x50,(int)D_004C6560__c34[index].x54);
 func_0020FA98__c34(0,0,x,y,z);
 if(target==255) D_0029DFF4__c34=value; else if(target==192) D_0029DFF8__c34=value;
}
}
/* ---- merged from src/unsorted/001F60C8.c ---- */
extern "C" {
typedef struct Obj__c35 { char pad[4]; unsigned char x4; } Obj__c35;
typedef struct Entry__c35 { Obj__c35 *obj; char pad[0x1DC]; } Entry__c35;
typedef struct ClearSlot__c35 { char pad[0x1CB0]; Obj__c35 *obj; } ClearSlot__c35;
typedef struct Slot__c35 { char pad[0x1710]; Obj__c35 *obj; } Slot__c35;
typedef struct Table__c35 { int values[15]; } Table__c35;
extern char D_004B25A0__c35[] asm("D_004B25A0");
extern Table__c35 D_00296D48__c35 asm("D_00296D48");
extern volatile unsigned char D_004B25EA__c35[] asm("D_004B25EA");
extern void *func_00254204__c35(void *, int, int) asm("func_00254204");
extern void func_001F60C8__c35(int index) asm("func_001F60C8");
void func_001F60C8__c35(int index) {
    int enabled[31];
    int *mapping;
    int i;
    func_00254204__c35(enabled, 0, 60);
    *(Table__c35 *)(enabled + 16) = D_00296D48__c35;
    switch (index) {
    case 0:
    case 1:
        enabled[1] = 1;
        enabled[2] = 1;
        enabled[13] = 1;
        enabled[14] = 1;
        break;
    case 2:
        enabled[1] = 1;
        enabled[2] = 1;
        enabled[3] = 1;
        break;
    case 3:
        enabled[3] = 1;
        enabled[4] = 1;
        enabled[2] = 1;
        break;
    case 4:
        enabled[5] = 1;
        enabled[4] = 1;
        enabled[3] = 1;
        break;
    case 5:
        enabled[5] = 1;
        enabled[4] = 1;
        enabled[6] = 1;
        break;
    case 6:
        enabled[5] = 1;
        enabled[6] = 1;
        enabled[7] = 1;
        break;
    case 7:
        enabled[6] = 1;
        enabled[8] = 1;
        enabled[7] = 1;
        break;
    case 8:
        enabled[7] = 1;
        enabled[8] = 1;
        enabled[9] = 1;
        break;
    case 9:
        enabled[8] = 1;
        enabled[9] = 1;
        enabled[10] = 1;
        break;
    case 10:
        enabled[9] = 1;
        enabled[10] = 1;
        enabled[11] = 1;
        break;
    case 11:
        enabled[11] = 1;
        enabled[10] = 1;
        enabled[12] = 1;
        break;
    case 12:
        enabled[12] = 1;
        enabled[11] = 1;
        enabled[13] = 1;
        break;
    case 13:
        if (D_004B25EA__c35[0] == 0) enabled[1] = 1;
        else enabled[14] = 1;
        enabled[12] = 1;
        enabled[13] = 1;
        break;
    case 14:
        enabled[1] = 1;
        enabled[14] = 1;
        enabled[13] = 1;
        break;
    }
    for (i = 1; i < 15; i++) {
        ((ClearSlot__c35 *)(D_004B25A0__c35 + i * 0x1E0))->obj->x4 = 0;
    }
    for (i = 17; i < 31; i++) {
        if (enabled[i - 16]) ((Slot__c35 *)(D_004B25A0__c35 + enabled[i] * 0x1E0))->obj->x4 = 1;
    }
}
}
/* ---- merged from src/unsorted/001F6338.c ---- */
extern "C" {
typedef struct Obj__c36 {
    char pad0[4];
    unsigned char x4;
} Obj__c36;

/* 0x1E0-byte records in D_004B25A0__c36; record i holds a pointer at 0x1710 */
typedef struct Slot__c36 {
    char pad0[0x1710];
    Obj__c36 *obj;
} Slot__c36;

typedef struct Table__c36 {
    int values[15];
} Table__c36;

extern char D_004B25A0__c36[] asm("D_004B25A0");
extern Table__c36 D_00296D48__c36 asm("D_00296D48");
extern void *func_00254204__c36(void *dst, int value, int size) asm("func_00254204");

#define SLOT(n) ((Slot__c36 *)(D_004B25A0__c36 + (n) * 0x1E0))

extern void func_001F6338__c36(int index) asm("func_001F6338");
void func_001F6338__c36(int index)
{
    /* [0..14]: enabled flag per entry, [16..30]: slot number per entry (copy of D_00296D48__c36) */
    int state[31];
    int i;

    func_00254204__c36(state, 0, 60);
    *(Table__c36 *)&state[16] = D_00296D48__c36;

    switch (index) {
    case 0:
        state[1] = 1;
        state[2] = 1;
        state[3] = 1;
        state[14] = 1;
        break;
    case 1:
        state[1] = 1;
        state[2] = 1;
        state[14] = 1;
        break;
    case 2:
        state[1] = 1;
        state[2] = 1;
        state[3] = 1;
        break;
    case 3:
        state[4] = 1;
        state[2] = 1;
        state[3] = 1;
        break;
    case 4:
        state[2] = 1;
        state[3] = 1;
        state[4] = 1;
        state[5] = 1;
        break;
    case 5:
        state[5] = 1;
        state[6] = 1;
        break;
    case 6:
        state[5] = 1;
        state[6] = 1;
        state[7] = 1;
        break;
    case 7:
        state[6] = 1;
        state[7] = 1;
        break;
    case 8:
        state[6] = 1;
        state[7] = 1;
        state[8] = 1;
        break;
    case 9:
        state[11] = 1;
        state[8] = 1;
        state[9] = 1;
        break;
    case 10:
        state[9] = 1;
        state[10] = 1;
        state[11] = 1;
        state[12] = 1;
        break;
    case 11:
        state[11] = 1;
        state[12] = 1;
        state[13] = 1;
        break;
    case 12:
        state[1] = 1;
        state[12] = 1;
        state[13] = 1;
        break;
    case 13:
        state[1] = 1;
        state[12] = 1;
        state[13] = 1;
        state[14] = 1;
        break;
    case 14:
        state[13] = 1;
        state[14] = 1;
        state[1] = 1;
        break;
    }

    for (i = 4; i < 18; i++) {
        SLOT(i)->obj->x4 = 0;
    }
    for (i = 16 + 1; i < 16 + 15; i++) {
        if (state[i - 16]) {
            SLOT(state[i])->obj->x4 = 1;
        }
    }
}
}
/* ---- merged from src/unsorted/001F65A8.c ---- */
extern "C" {
typedef struct Obj__c37 { char pad[4]; char x4; } Obj__c37;
typedef struct Entry__c37 { Obj__c37 *obj; char pad[0x1DC]; } Entry__c37;
typedef struct Global__c37 { char pad[0x1E90]; Entry__c37 entries[13]; } Global__c37;
extern Global__c37 D_004B25A0__c37 asm("D_004B25A0");
extern void func_001F65A8__c37(int value) asm("func_001F65A8");
void func_001F65A8__c37(int value) {
    int i;
    for (i = 0; i < 13; i++) {
        D_004B25A0__c37.entries[i].obj->x4 = value;
    }
}
}
/* ---- merged from src/unsorted/001F65E0.c ---- */
extern "C" {
typedef struct Obj__c38 { char pad[4]; char x4; } Obj__c38;
typedef struct Entry__c38 { Obj__c38 *obj; char pad[0x1DC]; } Entry__c38;
typedef struct Global__c38 { char pad[0x1EC0]; Entry__c38 entries[13]; } Global__c38;
extern Global__c38 D_004B25A0__c38 asm("D_004B25A0");
extern void func_001F65E0__c38(int value) asm("func_001F65E0");
void func_001F65E0__c38(int value) {
    int i;
    for (i = 0; i < 13; i++) {
        D_004B25A0__c38.entries[i].obj->x4 = value;
    }
}
}
/* ---- merged from src/unsorted/001F6618.c ---- */
extern "C" {
extern int D_004B25A0__c39[] asm("D_004B25A0");
extern char D_004225A0__c39[] asm("D_004225A0");
extern void func_00194290__c39(void *, int) asm("func_00194290");
extern void func_001F6618__c39(void) asm("func_001F6618");
void func_001F6618__c39(void) {
 func_00194290__c39(D_004225A0__c39, D_004B25A0__c39[7]);
 func_00194290__c39(D_004225A0__c39, D_004B25A0__c39[8]);
 func_00194290__c39(D_004225A0__c39, D_004B25A0__c39[9]);
 func_00194290__c39(D_004225A0__c39, D_004B25A0__c39[10]);
 func_00194290__c39(D_004225A0__c39, D_004B25A0__c39[11]);
 func_00194290__c39(D_004225A0__c39, D_004B25A0__c39[12]);
 func_00194290__c39(D_004225A0__c39, D_004B25A0__c39[13]);
 func_00194290__c39(D_004225A0__c39, D_004B25A0__c39[14]);
}
}
/* ---- merged from src/unsorted/001F66B0.c ---- */
extern "C" {
typedef struct Pads__c40 {
    int id[15];
    unsigned char state[15];
} Pads__c40;

extern Pads__c40 D_004B25A0__c40 asm("D_004B25A0");
extern int D_004C6780__c40[] asm("D_004C6780");
extern char D_004225A0__c40[] asm("D_004225A0");

extern unsigned char func_00193FE8__c40(void *p, int id) asm("func_00193FE8");

extern void func_001F66B0__c40(void) asm("func_001F66B0");
void func_001F66B0__c40(void)
{
    int i;

    for (i = 1; i < 15; i++) {
        if (func_00193FE8__c40(D_004225A0__c40, D_004B25A0__c40.id[i]) == 0)
            D_004C6780__c40[i] = 0;
        else if (D_004B25A0__c40.state[i] != func_00193FE8__c40(D_004225A0__c40, D_004B25A0__c40.id[i]))
            D_004C6780__c40[i] = 1;
        else if (D_004B25A0__c40.state[i] == func_00193FE8__c40(D_004225A0__c40, D_004B25A0__c40.id[i]))
            D_004C6780__c40[i] = 0;
        D_004B25A0__c40.state[i] = func_00193FE8__c40(D_004225A0__c40, D_004B25A0__c40.id[i]);
    }
}
}
/* ---- merged from src/unsorted/001F6798.c ---- */
extern "C" {
extern int D_0029DEF0__c41 asm("D_0029DEF0");
extern char D_004B25A0__c41[] asm("D_004B25A0");
extern void func_001BB040__c41(void *, int) asm("func_001BB040");
extern void func_001F6798__c41(int arg0) asm("func_001F6798");
void func_001F6798__c41(int arg0) {
    switch (arg0) {
    case 0: if (D_0029DEF0__c41 > 0) D_0029DEF0__c41 = 1; return;
    case 1: func_001BB040__c41(D_004B25A0__c41, 2); break;
    }
}
}
/* ---- merged from src/unsorted/001F67F0.c ---- */
extern "C" {
struct Obj__c42 { char pad[4]; signed char x4; };
struct Global__c42 { char pad[0x8760]; struct Obj__c42 *x8760; };
extern struct Global__c42 D_004B25A0__c42 asm("D_004B25A0");
extern char D_004C6370__c42[] asm("D_004C6370");
extern void func_001F57D0__c42(int,int) asm("func_001F57D0");
extern void func_002038E0__c42(void *,int) asm("func_002038E0");
extern void func_001F67F0__c42(int enabled) asm("func_001F67F0");
void func_001F67F0__c42(int enabled) {
    func_001F57D0__c42(0x232C,enabled);
    D_004B25A0__c42.x8760->x4 = enabled;
    if(enabled) {
        func_00203590((struct AnimationHandle *)(void *)(D_004C6370__c42),0x2330,0,0);
        func_002038E0__c42(D_004C6370__c42,1);
    }
    func_001F57D0__c42(0x232E,enabled ^ 1);
}
}
void func_001F6878(int a0, int index)
{
    union {
        u128 q;
        Fade fade;
    } vec;
    int offset;
    int handle;
    int last = 0xC;
    struct Record {
        char pad0[0x1710];
        int handle;
    } *record;

    vec.q = 0;
    vec.fade.x8 = 3.0f;

    offset = 3;
    if (index == 0xB) {
        offset = 5;
    } else if (index == last) {
        offset = 2;
    } else if (index == 0xD) {
        offset = 2;
    }
    func_001F6940();
    if (index == 0xE) {
        return;
    }
    record = (struct Record *)((char *)&D_004B25A0__target + (index + offset) * 0x1E0);
    handle = func_0021D638__target(record->handle, 1.0f);
    func_0021D6F8__target(handle, &vec.fade);
    func_0021D748__target(handle, 240.0f);
}
/* ---- merged from src/unsorted/001F6940.c ---- */
extern "C" {
typedef struct Slot__c43 {
    char pad0[0x1710];
    int x1710;
} Slot__c43;

extern char D_004B25A0__c43[] asm("D_004B25A0");
extern void func_0021D910__c43(int) asm("func_0021D910");

extern void func_001F6940__c43(void) asm("func_001F6940");
void func_001F6940__c43(void)
{
    int i;
    int offset;

    offset = 3;
    for (i = 1; i < 15; i++) {
        if (i == 11) {
            offset = 5;
        } else if (i == 12) {
            offset = 2;
        } else if (i == 13) {
            offset = 2;
        }
        if (i != 14) {
            func_0021D910__c43(((Slot__c43 *)(D_004B25A0__c43 + (i + offset) * 0x1E0))->x1710);
        }
    }
}
}
/* ---- merged from src/unsorted/001F6A08.c ---- */
extern "C" {
typedef unsigned int u128__c44 __attribute__((mode(TI)));

typedef union Vec4__c44 {
    u128__c44 q;
    float f[4];
} Vec4__c44;

/* table of 0x1E0-byte records whose handle sits at +0x1710 of D_004B25A0__c44 */
typedef struct Record__c44 {
    char pad0[0x1710];
    int handle;
} Record__c44;

extern char D_004B25A0__c44[] asm("D_004B25A0");

extern int func_0021D638__c44(int handle, float scale) asm("func_0021D638");
extern void func_0021D6F8__c44(int obj, float *vec) asm("func_0021D6F8");
extern void func_0021D748__c44(int obj, float value) asm("func_0021D748");

extern void func_001F6A08__c44(int index) asm("func_001F6A08");
void func_001F6A08__c44(int index)
{
    Vec4__c44 vec;
    int offset = 3;
    int obj;

    vec.q = 0;
    vec.f[2] = 3.0f;

    if (index == 11) {
        offset = 5;
    } else if (index == 12) {
        offset = 2;
    } else if (index == 13) {
        offset = 2;
    }
    if (index != 14) {
        obj = func_0021D638__c44(((Record__c44 *)((index + offset) * 0x1E0 + D_004B25A0__c44))->handle, 1.0f);
        func_0021D6F8__c44(obj, vec.f);
        func_0021D748__c44(obj, 240.0f);
    }
}
}
/* ---- func_001F6AB8: game/menu state init ---- */
extern "C" {
typedef struct Style__w1 {
    int rgb[2][3][3];
    int x48, x4C;
    float x50, x54;
} Style__w1;

extern Style__w1 D_004C6560__w1[] asm("D_004C6560");
extern char D_004C67C0__w1[] asm("D_004C67C0");
extern char D_004C6B40__w1[] asm("D_004C6B40");
extern char D_00275488__w1[] asm("D_00275488");
extern int D_004C6900__w1[] asm("D_004C6900");
extern int D_004C6910__w1[] asm("D_004C6910");
extern int D_004C6920__w1[] asm("D_004C6920");
extern int D_004C6960__w1[] asm("D_004C6960");
extern int D_004C69A0__w1[] asm("D_004C69A0");
extern int D_004C69E0__w1[13][5] asm("D_004C69E0");
extern unsigned char D_004B25DC__w1[] asm("D_004B25DC");
extern char D_00759780__w1[] asm("D_00759780");

extern int D_0029DE98__w1 asm("D_0029DE98");
extern float D_0029DEA0__w1 asm("D_0029DEA0"), D_0029DEA4__w1 asm("D_0029DEA4"), D_0029DEA8__w1 asm("D_0029DEA8"), D_0029DEAC__w1 asm("D_0029DEAC");
extern int D_0029DEB0__w1 asm("D_0029DEB0"), D_0029DEB4__w1 asm("D_0029DEB4");
extern float D_0029DEB8__w1 asm("D_0029DEB8"), D_0029DEBC__w1 asm("D_0029DEBC"), D_0029DEC0__w1 asm("D_0029DEC0"), D_0029DEC4__w1 asm("D_0029DEC4");
extern int D_0029DEC8__w1 asm("D_0029DEC8"), D_0029DECC__w1 asm("D_0029DECC"), D_0029DED0__w1 asm("D_0029DED0"), D_0029DED4__w1 asm("D_0029DED4");
extern int D_0029DEDC__w1 asm("D_0029DEDC"), D_0029DEE0__w1 asm("D_0029DEE0");
extern unsigned char D_0029DEE8__w1 asm("D_0029DEE8"), D_0029DEE9__w1 asm("D_0029DEE9");
extern int D_0029DEEC__w1 asm("D_0029DEEC"), D_0029DEF0__w1 asm("D_0029DEF0");
extern unsigned char D_0029DEF4__w1 asm("D_0029DEF4");
extern int D_0029DEF8__w1 asm("D_0029DEF8"), D_0029DEFC__w1 asm("D_0029DEFC"), D_0029DF00__w1 asm("D_0029DF00"), D_0029DF04__w1 asm("D_0029DF04");
extern int D_0029DF08__w1 asm("D_0029DF08"), D_0029DF0C__w1 asm("D_0029DF0C"), D_0029DF10__w1 asm("D_0029DF10"), D_0029DF14__w1 asm("D_0029DF14");
extern int D_0029DF18__w1 asm("D_0029DF18"), D_0029DF1C__w1 asm("D_0029DF1C"), D_0029DF24__w1 asm("D_0029DF24"), D_0029DF28__w1 asm("D_0029DF28");
extern int D_0029DF2C__w1 asm("D_0029DF2C"), D_0029DF30__w1 asm("D_0029DF30"), D_0029DF3C__w1 asm("D_0029DF3C"), D_0029DF40__w1 asm("D_0029DF40");
extern int D_0029DF44__w1 asm("D_0029DF44"), D_0029DF48__w1 asm("D_0029DF48");
extern unsigned char D_0029DF4C__w1 asm("D_0029DF4C");
extern int D_0029DF50__w1 asm("D_0029DF50");
extern float D_0029DF54__w1 asm("D_0029DF54");
extern int D_0029DF58__w1 asm("D_0029DF58"), D_0029DF5C__w1 asm("D_0029DF5C"), D_0029DF60__w1 asm("D_0029DF60"), D_0029DF64__w1 asm("D_0029DF64");
extern int D_0029DF68__w1 asm("D_0029DF68"), D_0029DF6C__w1 asm("D_0029DF6C"), D_0029DF70__w1 asm("D_0029DF70"), D_0029DF74__w1 asm("D_0029DF74");
extern float D_0029DF78__w1 asm("D_0029DF78");
extern int D_0029DF7C__w1 asm("D_0029DF7C"), D_0029DF80__w1 asm("D_0029DF80");
extern unsigned char D_0029DF84__w1 asm("D_0029DF84"), D_0029DF85__w1 asm("D_0029DF85"), D_0029DF86__w1 asm("D_0029DF86"), D_0029DF87__w1 asm("D_0029DF87");
extern unsigned char D_0029DF88__w1 asm("D_0029DF88"), D_0029DF89__w1 asm("D_0029DF89"), D_0029DF8A__w1 asm("D_0029DF8A"), D_0029DF8B__w1 asm("D_0029DF8B");
extern unsigned char D_0029DF8C__w1 asm("D_0029DF8C");
extern int D_0029DF90__w1 asm("D_0029DF90");
extern unsigned char D_0029DF94__w1 asm("D_0029DF94"), D_0029DF95__w1 asm("D_0029DF95"), D_0029DF96__w1 asm("D_0029DF96"), D_0029DF97__w1 asm("D_0029DF97");
extern int D_0029DF98__w1 asm("D_0029DF98");
extern unsigned char D_0029DFB4__w1 asm("D_0029DFB4");
extern int D_0029DFB8__w1 asm("D_0029DFB8"), D_0029DFBC__w1 asm("D_0029DFBC"), D_0029DFC0__w1 asm("D_0029DFC0");
extern float D_0029DFC4__w1 asm("D_0029DFC4"), D_0029DFC8__w1 asm("D_0029DFC8");
extern int D_0029DFCC__w1 asm("D_0029DFCC"), D_0029DFD0__w1 asm("D_0029DFD0"), D_0029DFD4__w1 asm("D_0029DFD4"), D_0029DFD8__w1 asm("D_0029DFD8");
extern int D_0029DFDC__w1 asm("D_0029DFDC"), D_0029DFE0__w1 asm("D_0029DFE0"), D_0029DFE4__w1 asm("D_0029DFE4");
extern float D_0029DFE8__w1 asm("D_0029DFE8"), D_0029DFEC__w1 asm("D_0029DFEC");
extern int D_0029DFF0__w1 asm("D_0029DFF0"), D_0029DFF4__w1 asm("D_0029DFF4"), D_0029DFF8__w1 asm("D_0029DFF8");
extern float D_0029E000__w1 asm("D_0029E000");
extern unsigned char D_0029E004__w1 asm("D_0029E004");
extern float D_0029E008__w1 asm("D_0029E008"), D_0029E00C__w1 asm("D_0029E00C"), D_0029E010__w1 asm("D_0029E010");
extern float D_0029E014__w1 asm("D_0029E014"), D_0029E018__w1 asm("D_0029E018");
extern int D_0029E01C__w1 asm("D_0029E01C"), D_0029E020__w1 asm("D_0029E020"), D_0029E024__w1 asm("D_0029E024");
extern int D_0029E028__w1 asm("D_0029E028"), D_0029E02C__w1 asm("D_0029E02C");
extern float D_0029E030__w1 asm("D_0029E030");
extern int D_0029E034__w1 asm("D_0029E034");
extern int D_0029E0E8__w1 asm("D_0029E0E8"), D_0029E0F0__w1 asm("D_0029E0F0"), D_0029E0F8__w1 asm("D_0029E0F8");
extern int D_0029E100__w1 asm("D_0029E100"), D_0029E108__w1 asm("D_0029E108");

extern void func_001F66B0__w1(void) asm("func_001F66B0");
extern void func_001F7DE0__w1(void) asm("func_001F7DE0");
extern int func_001F76D8__w1(char *, int) asm("func_001F76D8");
extern void func_001F1090__w1(void) asm("func_001F1090");

#define SET_RGB__w1(c, r, g, b) ((c)[0] = (r), (c)[1] = (g), (c)[2] = (b))

extern void func_001F6AB8__w1(void) asm("func_001F6AB8");
void func_001F6AB8__w1(void)
{
    Style__w1 *s;
    int n;
    int *p;
    int i;
    int j;

    D_0029DE98__w1 = 1;
    func_001F66B0__w1();
    func_001F7DE0__w1();

    s = D_004C6560__w1;
    SET_RGB__w1(s[0].rgb[0][0], 0x26, 0x26, 0x26);
    SET_RGB__w1(s[0].rgb[0][1], 0x46, 0x46, 0x46);
    SET_RGB__w1(s[0].rgb[0][2], 0x64, 0x64, 0x64);
    SET_RGB__w1(s[0].rgb[1][0], 0x3F, 0x40, 0x13);
    SET_RGB__w1(s[0].rgb[1][1], 0x59, 0x45, 0x1A);
    SET_RGB__w1(s[0].rgb[1][2], 0x78, 0x5F, 0x28);
    s[0].x48 = 0x1BD;
    s[0].x4C = 0x10A;
    s[0].x50 = -156.76f;
    s[0].x54 = -26.0f;
    SET_RGB__w1(s[1].rgb[0][0], 0x26, 0x26, 0x26);
    SET_RGB__w1(s[1].rgb[0][1], 0x46, 0x46, 0x46);
    SET_RGB__w1(s[1].rgb[0][2], 0x62, 0x62, 0x62);
    SET_RGB__w1(s[1].rgb[1][0], 0x51, 0x52, 0x25);
    SET_RGB__w1(s[1].rgb[1][1], 0x6B, 0x57, 0x2C);
    SET_RGB__w1(s[1].rgb[1][2], 0xAB, 0x89, 0x40);
    s[1].x48 = 0x181;
    s[1].x4C = 0xB1;
    s[1].x50 = -132.59999f;
    s[1].x54 = -33.6f;
    SET_RGB__w1(s[2].rgb[0][0], 0x34, 0x34, 0x34);
    SET_RGB__w1(s[2].rgb[0][1], 0x4C, 0x4C, 0x4C);
    SET_RGB__w1(s[2].rgb[0][2], 0x60, 0x60, 0x60);
    SET_RGB__w1(s[2].rgb[1][0], 0x49, 0x4A, 0x1D);
    SET_RGB__w1(s[2].rgb[1][1], 0x63, 0x4F, 0x24);
    SET_RGB__w1(s[2].rgb[1][2], 0xA3, 0x81, 0x38);
    s[2].x48 = 0x156;
    s[2].x4C = 0xA7;
    s[2].x50 = -137.5984f;
    s[2].x54 = -30.6f;
    SET_RGB__w1(s[3].rgb[0][0], 0x38, 0x38, 0x38);
    SET_RGB__w1(s[3].rgb[0][1], 0x4C, 0x4C, 0x4C);
    SET_RGB__w1(s[3].rgb[0][2], 0x60, 0x60, 0x60);
    SET_RGB__w1(s[3].rgb[1][0], 0x39, 0x3A, 0x23);
    SET_RGB__w1(s[3].rgb[1][1], 0x63, 0x4F, 0x24);
    SET_RGB__w1(s[3].rgb[1][2], 0xA3, 0x81, 0x38);
    s[3].x48 = 0x100;
    s[3].x4C = 0xB1;
    s[3].x50 = -88.0f;
    s[3].x54 = -44.12f;
    SET_RGB__w1(s[4].rgb[0][0], 0x38, 0x38, 0x38);
    SET_RGB__w1(s[4].rgb[1][0], 0x18, 0x39, 0x59);
    s[4].x48 = 0x15D;
    s[4].x4C = 0xA6;
    s[4].x50 = -134.0f;
    s[4].x54 = -26.6f;
    SET_RGB__w1(s[5].rgb[0][0], 0x29, 0x29, 0x29);
    s[5].x48 = 0x116;
    s[5].x4C = 0xA0;
    s[5].x50 = -89.5984f;
    s[5].x54 = -26.6f;

    D_0029DEAC__w1 = 0.0f;
    D_0029DEA0__w1 = D_0029DEAC__w1;
    D_0029DEA4__w1 = D_0029DEAC__w1;
    D_0029DEA8__w1 = D_0029DEAC__w1;
    D_0029DEB0__w1 = 0;
    D_0029DEB8__w1 = 370.0f;
    D_0029DEBC__w1 = 15210.0f;
    D_0029DEC0__w1 = 255.0f;
    D_0029DEC4__w1 = 16380.0f;
    D_0029DEC8__w1 = 0x25;
    D_0029DECC__w1 = 0x10;
    D_0029DED0__w1 = 0xF;
    D_0029DEB4__w1 = 1;
    D_0029DED4__w1 = func_001F76D8__w1(D_004C67C0__w1, 0);
    D_0029DEDC__w1 = -1;
    D_0029DEEC__w1 = 0;
    D_0029DEE0__w1 = 1;
    D_0029DEE8__w1 = 0;
    D_0029DEF4__w1 = 1;
    D_0029DEE9__w1 = 1;
    D_0029DEF0__w1 = 0;

    {
        char *d = D_004C6B40__w1;
        char *s = D_00275488__w1;
        int k = 20;
        while (k--) {
            *d++ = *s++;
        }
    }

    D_0029DF54__w1 = 1.0f;
    D_0029DF78__w1 = 0.85f;
    D_0029DEF8__w1 = 0;
    D_0029DEFC__w1 = 0;
    D_0029DF00__w1 = 0;
    D_0029DF04__w1 = 2;
    D_0029DF08__w1 = 0;
    D_0029DF0C__w1 = 0;
    D_0029DF10__w1 = 0;
    D_0029DF14__w1 = 0;
    D_0029DF18__w1 = 0;
    D_0029DF1C__w1 = 0;
    D_0029DF24__w1 = 0;
    D_0029DF28__w1 = 0;
    D_0029DF2C__w1 = 0;
    D_0029DF30__w1 = 0;
    D_0029DF3C__w1 = 0;
    D_0029DF40__w1 = 0;
    D_0029DF44__w1 = 0;
    D_0029DF48__w1 = 0;
    D_0029DF4C__w1 = 0;
    D_0029DF50__w1 = 0;
    D_0029DF58__w1 = 0;
    D_0029DF5C__w1 = -1;
    D_0029DF60__w1 = 1;
    D_0029DF64__w1 = 0;
    D_0029DF68__w1 = 0;
    D_0029DF6C__w1 = 1;
    D_0029DF70__w1 = 0;
    D_0029DF74__w1 = 0;
    D_0029E0E8__w1 = 0;
    D_0029E0F8__w1 = 0;
    D_0029E108__w1 = 2;
    D_0029E0F0__w1 = 0;
    D_0029DFC4__w1 = 100.0f;
    D_0029DFC8__w1 = 688.0f;
    D_0029DF80__w1 = -1;
    D_0029DFBC__w1 = 0xFE;
    D_0029E100__w1 = 0;
    D_0029DF7C__w1 = 0;
    D_0029DF84__w1 = 0;
    D_0029DF85__w1 = 0;
    D_0029DF86__w1 = 0;
    D_0029DF87__w1 = 0;
    D_0029DF88__w1 = 0;
    D_0029DF89__w1 = 0;
    D_0029DF8A__w1 = 0;
    D_0029DF8B__w1 = 0;
    D_0029DF8C__w1 = 0;
    D_0029DF90__w1 = 0;
    D_0029D800 = 0;
    D_0029DFB4__w1 = 0;
    D_0029DFB8__w1 = 0;
    D_0029DFC0__w1 = 0;
    D_0029DF94__w1 = 0;
    D_0029DF95__w1 = 0;
    D_0029DF96__w1 = 0;
    D_0029DF97__w1 = 0;
    D_0029DF98__w1 = 0;
    D_0029DFCC__w1 = 0x97;
    D_0029DFD0__w1 = 0x97;
    D_0029DFD4__w1 = 0;
    D_0029DFD8__w1 = 5;
    D_0029DFDC__w1 = 5;
    D_004C6900__w1[0] = 0x3C;
    D_004C6900__w1[1] = 0x3C;
    D_004C6900__w1[2] = 0x3C;
    D_0029DFE0__w1 = 0x120;
    D_0029DFE4__w1 = 0xB0;
    D_0029DFE8__w1 = -20.8f;
    D_0029DFEC__w1 = 24.0f;
    D_0029DFF0__w1 = 3;
    D_0029DFFC = 2;
    D_0029E000__w1 = 640.0f;
    D_0029E008__w1 = 401.6f;
    D_0029E00C__w1 = 177.0f;
    D_0029E010__w1 = -164.59999f;
    D_0029E014__w1 = -33.6f;
    D_0029E018__w1 = 2.4f;
    D_004C6910__w1[0] = 1;
    D_0029DFF4__w1 = 0;
    D_0029DFF8__w1 = 0;
    D_0029E004__w1 = 0;
    D_0029E01C__w1 = 0;
    D_0029E020__w1 = 1;
    D_0029E024__w1 = 0;
    D_004C6910__w1[1] = 1;
    D_004C6910__w1[2] = 1;
    D_004C6910__w1[3] = 1;
    D_004C6920__w1[0] = 10;
    D_004C6920__w1[1] = 0;
    p = &D_004C6920__w1[2];
    for (i = 0; i < 13; i++) {
        *p++ = 10;
    }
    D_004C6960__w1[0] = 10;
    D_004C6960__w1[1] = 0;
    {
        int *q = &D_004C6960__w1[2];
        for (i = 12; i >= 0; i--) {
            *q++ = 10;
        }
    }

    for (i = 0; i < 13; i = n) {
        n = i + 1;
        for (j = 0; j < 5; j++) {
            if (i == 0) {
                D_004C69E0__w1[0][j] = 0;
            } else {
                D_004C69E0__w1[i][j] = 0x2347 + i * 10 + j;
            }
        }
    }
    func_001F1090__w1();

    for (j = 0; j < 15; j++) {
        D_004C69A0__w1[j] = 1;
        if (D_004B25DC__w1[j] == 0) {
            D_004C69A0__w1[j] = D_0029D91C;
        }
    }
    D_0029E028__w1 = -1;
    D_0029E030__w1 = 1.0f;
    D_0029E02C__w1 = -1;
    D_0029E034__w1 = 0;
    D_0029D7F4 = 0;
    D_0029D7F8 = 0;
    D_0029D7FC = 0;
    func_00236A30((TimerPool *)D_00759780__w1, 100);
}
}
/* ---- merged from src/unsorted/001F7420.c ---- */
extern "C" {
typedef struct Msg__c45 {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    unsigned char data[1];
} Msg__c45;

extern void func_001F67F0__c45(int) asm("func_001F67F0");
extern int func_00238F48__c45(void) asm("func_00238F48");
extern void func_001F5578__c45(void) asm("func_001F5578");

extern void func_001F7420__c45(Msg__c45 *msg, int x18, int x14, int x8, int xC, int x10, unsigned char *data, int size) asm("func_001F7420");
void func_001F7420__c45(Msg__c45 *msg, int x18, int x14, int x8, int xC, int x10, unsigned char *data, int size)
{
    int i;

    if (msg->x4 == 0) {
        func_001F67F0__c45(0);
        msg->x18 = x18;
        msg->x14 = x14;
        msg->x8 = x8;
        msg->xC = xC;
        msg->x10 = x10;
        for (i = 0; i < size; i++) {
            msg->data[i] = data[i];
        }
        msg->x0 = func_00238F48__c45();
        func_001F5578__c45();
    }
}
}
/* ---- merged from src/unsorted/001F74F0.c ---- */
extern "C" {
typedef struct Obj__c46 { int x0, count, x8, xC, index, x14, x18; unsigned char flags[1]; } Obj__c46;
extern int func_001F5320__c46(int, int, int, int, unsigned char *) asm("func_001F5320");
extern void func_001F74F0__c46(Obj__c46 *p) asm("func_001F74F0");
void func_001F74F0__c46(Obj__c46 *p) {
    int index;
    unsigned char *flags;
    if (p->count < 1000) p->count++;
    index = p->index;
    flags = p->flags;
    if (index >= 0 && flags[index]) p->index = func_001F5320__c46(1, p->x8, p->xC, index, flags);
}
}
/* ---- merged from src/unsorted/001F7550.c ---- */
extern "C" {
typedef unsigned int u128__c47 __attribute__((mode(TI)));

typedef struct Obj__c47 {
    char pad0[0x8];
    int x8;
    int xC;
    int x10;
    char pad14[0x8];
    char x1C[1];
} Obj__c47;

extern char D_004B2A90__c47[] asm("D_004B2A90");
extern int func_001F5320__c47(int a, int b, int c, int d, void *e) asm("func_001F5320");
extern void func_001C57A8__c47(void *p, u128__c47 name) asm("func_001C57A8");

/* "S_NEXT" packed little-endian into a 128-bit value */
#define NAME_S_NEXT ((u128__c47)0x5458454E5F53LL)

extern void func_001F7550__c47(Obj__c47 *self) asm("func_001F7550");
void func_001F7550__c47(Obj__c47 *self)
{
    int old = self->x10;
    int cur;

    cur = func_001F5320__c47(1, self->x8, self->xC, old, self->x1C);
    self->x10 = cur;
    if (old != cur) {
        func_001C57A8__c47(D_004B2A90__c47, NAME_S_NEXT);
    }
}
}
/* ---- merged from src/unsorted/001F75C0.c ---- */
extern "C" {
typedef unsigned int u128__c48 __attribute__((mode(TI)));

typedef struct Obj__c48 {
    char pad0[0x8];
    int x8;
    int xC;
    int x10;
    char pad14[0x8];
    char x1C[1];
} Obj__c48;

extern char D_004B2A90__c48[] asm("D_004B2A90");

extern int func_001F5320__c48(int, int, int, int, char *) asm("func_001F5320");
extern void func_001C57A8__c48(char *, u128__c48) asm("func_001C57A8");

extern void func_001F75C0__c48(Obj__c48 *p) asm("func_001F75C0");
void func_001F75C0__c48(Obj__c48 *p)
{
    int prev = p->x10;

    p->x10 = func_001F5320__c48(-1, p->x8, p->xC, prev, p->x1C);
    if (prev != p->x10) {
        /* packed name "S_PREVIOUS" */
        func_001C57A8__c48(D_004B2A90__c48, ((u128__c48)0x5355LL << 64) | (u128__c48)0x4F49564552505F53LL);
    }
}
}
/* ---- merged from src/unsorted/001F7640.c ---- */
extern "C" {
typedef unsigned int u128__c49 __attribute__((mode(TI)));
extern char D_004B2A90__c49[] asm("D_004B2A90");
extern void func_001C57A8__c49(void *, u128__c49) asm("func_001C57A8");
extern void PlayMenuSelectLegacy9(void) asm("func_001F7640");
void PlayMenuSelectLegacy9(void) {
    func_001C57A8__c49(D_004B2A90__c49, (u128__c49)0x5443454C45535F53UL);
}
}
/* ---- merged from src/unsorted/001F7680.c ---- */
extern "C" {
typedef unsigned int u128__c50 __attribute__((mode(TI)));
extern char D_004B2A90__c50[] asm("D_004B2A90");
extern void func_001C57A8__c50(void *, u128__c50) asm("func_001C57A8");
extern void PlayMenuBackLegacy0(void) asm("func_001F7680");
void PlayMenuBackLegacy0(void) {
    func_001C57A8__c50(D_004B2A90__c50, (u128__c50)0x4B4341425F53UL);
}
}
/* ---- merged from src/unsorted/001F76B8.c ---- */
extern "C" {
struct Obj__c51 {
    int x0;
    int x4;
};

extern void func_001F5578__c51(void) asm("func_001F5578");

extern void func_001F76B8__c51(struct Obj__c51 *obj) asm("func_001F76B8");
void func_001F76B8__c51(struct Obj__c51 *obj)
{
    obj->x4 = 0;
    func_001F5578__c51();
}
}
/* ---- merged from src/unsorted/001F76D8.c ---- */
extern "C" {
typedef unsigned int u128__c52 __attribute__((mode(TI)));
struct Packet__c52 { volatile unsigned long lo; unsigned long hi; };
extern char D_00513B00__c52[] asm("D_00513B00");
extern void func_0023D618__c52(int,int *,int *) asm("func_0023D618");
extern void func_0023D650__c52(int,int *,int *) asm("func_0023D650");
extern void func_002105A0__c52(void *,struct Packet__c52 *,int,int,int,int) asm("func_002105A0");
extern void func_00210578__c52(void *,struct Packet__c52 *,int,int) asm("func_00210578");
extern void *func_001F76D8__c52(struct Packet__c52 *p,int object) asm("func_001F76D8");
void *func_001F76D8__c52(struct Packet__c52 *p,int object) {
 int x,y,width,height,left,top; struct Packet__c52 *base; void *result;
 base=p; p++;
 p->lo=0x1000000000008001UL; p->hi=14; p=(struct Packet__c52 *)((int)p+16);
 p->lo=0x4000000044UL; p->hi=0x42; p=(struct Packet__c52 *)((int)p+16);
 p->lo=0x3023400000008001UL; p->hi=0x551;
 func_0023D618__c52(object,&x,&y);
 p=(struct Packet__c52 *)((int)p+16); result=(char *)p+12;
 func_0023D650__c52(object,&width,&height);
 func_002105A0__c52(D_00513B00__c52,p,0,0,0,0); p=(struct Packet__c52 *)((int)p+16);
 left=x-(width>>1); top=y-(height>>1);
 func_00210578__c52(D_00513B00__c52,p,left,top); p=(struct Packet__c52 *)((int)p+16);
 func_00210578__c52(D_00513B00__c52,p,left+width,top+height);
 *(u128__c52 *)base=0;
 *(int *)base=(((char *)p-(char *)base)>>4)|0x60000000;
 return result;
}
}
/* ---- merged from src/unsorted/001F7840.cpp ---- */
extern unsigned int *D_0029DED4__c53 asm("D_0029DED4");
extern char D_004C67C0__c53[] asm("D_004C67C0"), D_00513B00__c53[] asm("D_00513B00"), D_00759780__c53[] asm("D_00759780");

extern "C" unsigned int func_001FD238__c53(float) asm("func_001FD238");
extern "C" void func_002103D8__c53(void *, int, void *) asm("func_002103D8");

extern "C" int func_001F7840__c53(void)
{
    float delta;
    float value;
    float sum;
    unsigned int *dest;

    if (!D_0029D7F4) {
        return 0;
    }
    delta = (float)D_0029D7F8 - (float)*D_0029DED4__c53;
    if (D_0029D7FC) {
        delta /= (float)D_0029D7FC;
        D_0029D7FC--;
    }

    value = (float)*D_0029DED4__c53;
    sum = value + delta;
    value = sum >? 0.0f;
    dest = D_0029DED4__c53;
    *dest = func_001FD238__c53(value);
    if (*D_0029DED4__c53) {
        func_002103D8__c53(D_00513B00__c53, 0, D_004C67C0__c53);
    }
    D_0029D7F4 = (*D_0029DED4__c53 != 0);
    return D_0029D7F4;
}

extern "C" void func_001F79D8__c53(int value, int mode)
{
    int current = *D_0029DED4__c53;
    if (current == 0 || current != value) {
        D_0029D7F8 = value;
        D_0029D7FC = mode;
        if (D_0029D7F4 == 0)
            func_00236B98((TimerPool *)D_00759780__c53, (TimerCallbackNoArg)func_001F7840__c53, 1);
        if (*D_0029DED4__c53 != 0)
            func_002103D8__c53(D_00513B00__c53, 0, D_004C67C0__c53);
        D_0029D7F4 = 1;
    }
}
/* ---- merged from src/unsorted/001F7A60.c ---- */
extern "C" {
extern int *D_0029DED4__c54 asm("D_0029DED4");

extern void func_001F7A60__c54(int value) asm("func_001F7A60");
void func_001F7A60__c54(int value)
{
    if (D_0029DED4__c54 != 0) {
        *D_0029DED4__c54 = value;
    }
}
}
/* ---- merged from src/unsorted/001F7A78.c ---- */
extern "C" {
extern int *D_0029DED4__c55 asm("D_0029DED4");

extern int func_001F7A78__c55(void) asm("func_001F7A78");
int func_001F7A78__c55(void)
{
    if (D_0029DED4__c55 != 0) {
        return *D_0029DED4__c55;
    }
    return 0;
}
}
/* ---- merged from src/unsorted/001F7A98.c ---- */
extern "C" {
extern int D_00296E10__c56[] asm("D_00296E10");
extern int func_001F7A98__c56(int index) asm("func_001F7A98");
int func_001F7A98__c56(int index) { return D_00296E10__c56[index / 32 - 1]; }
}
/* ---- merged from src/unsorted/001F7AC8.c ---- */
extern "C" {
extern int D_00296E50__c57[] asm("D_00296E50");
extern int func_001F7AC8__c57(int index) asm("func_001F7AC8");
int func_001F7AC8__c57(int index) { return D_00296E50__c57[index / 32 - 1]; }
}
/* ---- merged from src/unsorted/001F7AF8.c ---- */
extern "C" {
extern int D_00296E90__c58[] asm("D_00296E90");

extern int func_001F7AF8__c58(int index) asm("func_001F7AF8");
int func_001F7AF8__c58(int index)
{
    return D_00296E90__c58[index];
}
}
/* ---- merged from src/unsorted/001F7B10.c ---- */
extern "C" {
extern int D_0029DF98__c59 asm("D_0029DF98");
extern int func_0018DF28__c59(int,int) asm("func_0018DF28");
extern void func_001F7B10__c59(int count) asm("func_001F7B10");
void func_001F7B10__c59(int count) {
    int i=0;
    D_0029DF98__c59=0;
    for(;i<=count-1;i++) {
        if(func_0018DF28__c59(i,0)) D_0029DF98__c59=count;
    }
}
}
/* ---- merged from src/unsorted/001F7B80.c ---- */
extern "C" {
typedef struct { long x0, x8; } __attribute__((packed)) Value__c61;
typedef struct { char pad0[0x3C]; unsigned char selectable[32]; } RiderFlag__c61;

extern Value__c61 *D_00275B70__c61[] asm("D_00275B70");
extern int D_00275BF0__c61[] asm("D_00275BF0");
extern int D_004C6780__c61[] asm("D_004C6780");
extern int D_004C6910__c61[] asm("D_004C6910");
extern int D_004C69A0__c61[] asm("D_004C69A0");
extern int D_0029DF68__c61 asm("D_0029DF68");
extern char D_004B25A0__c61[] asm("D_004B25A0");
extern Value__c61 D_004C6370__c61 asm("D_004C6370");

extern void func_00203E10__c61(Value__c61 *, float) asm("func_00203E10");
extern int func_00204230__c61(Value__c61 *) asm("func_00204230");
extern int func_00222BD8__c61(int, int) asm("func_00222BD8");
extern void func_001F57D0__c61(int, int) asm("func_001F57D0");

extern void func_001F7B80__c61(void) asm("func_001F7B80");
void func_001F7B80__c61(void)
{
    int i;
    Value__c61 a, b;

    for (i = 1; i < 15; i++) {
        func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c61[i]), D_00275BF0__c61[i], 0, D_004C69A0__c61[i]);
        a = *D_00275B70__c61[i];
        if (!func_00204230__c61(&a)) {
            D_004C69A0__c61[i] = func_00222BD8__c61(1, 2);
            if (i == 12 && D_004C6910__c61[D_0029DF68__c61] != i)
                D_004C69A0__c61[12] = 1;
            /* selectable[0] is D_004B25DC; the struct form orders the prologue like retail */
            if (((RiderFlag__c61 *)D_004B25A0__c61)->selectable[i] == 1 && !D_004C6780__c61[i]) {
                func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c61[i]), D_00275BF0__c61[i], 0, D_004C69A0__c61[i]);
                b = *D_00275B70__c61[i];
                func_00203E10__c61(&b, 1.0f);
                func_00202E38((struct AnimationHandle *)(void *)(D_00275B70__c61[i]), 1, 1, 1000.2f);
                if (i == 12 && D_004C6910__c61[D_0029DF68__c61] == i && D_004C69A0__c61[12] == 2) {
                    func_00203590((struct AnimationHandle *)(void *)(&D_004C6370__c61), 0x2330, 0, 18);
                    func_00202E38((struct AnimationHandle *)(void *)(&D_004C6370__c61), 1, 1, 1000.2f);
                }
                if (i == 2)
                    func_001F57D0__c61(0x235F, 0);
            }
        }
    }
}
}
/* ---- merged from src/unsorted/001F7DE0.c ---- */
extern "C" {
typedef struct Value__c60 { int x[4]; } Value__c60;
typedef struct Obj__c60 { char pad[4]; unsigned char x4; } Obj__c60;
typedef struct Slot__c60 { char pad[0x1710]; Obj__c60 *obj; } Slot__c60;
typedef struct Table__c60 { int values[15]; } Table__c60;

typedef struct Game__c60 {
    char pad0[0x3C];
    unsigned char active[15];   /* active[14] (0x4A) is read again after the loop */
    char pad4B[0x3720 - 0x4B];
    Obj__c60 *x3720;
} Game__c60;

extern Game__c60 D_004B25A0__c60 asm("D_004B25A0");
extern int D_004C6780__c60[] asm("D_004C6780");
extern int D_004C67B8__c60[] asm("D_004C67B8");
extern Value__c60 *D_00275B70__c60[] asm("D_00275B70");
extern int D_00275BF0__c60[] asm("D_00275BF0");
extern Value__c60 *D_00275BB0__c60[] asm("D_00275BB0");
extern int D_00275C68__c60[] asm("D_00275C68");
extern Table__c60 D_00296D48__c60 asm("D_00296D48");

extern void func_00203FE0__c60(Value__c60 *, int) asm("func_00203FE0");
extern void func_002038E0__c60(Value__c60 *, int) asm("func_002038E0");
extern void func_00203CB8__c60(Value__c60 *) asm("func_00203CB8");
extern void func_00203F40__c60(Value__c60 *, int) asm("func_00203F40");
extern void func_00203E10__c60(Value__c60 *, float) asm("func_00203E10");
extern void func_00203B68__c60(Value__c60 *, int, float) asm("func_00203B68");
extern void func_002185C0__c60(Obj__c60 *, int) asm("func_002185C0");

extern void func_001F7DE0__c60(void) asm("func_001F7DE0");
void func_001F7DE0__c60(void)
{
    int i;
    Value__c60 v;
    Table__c60 table;
    Game__c60 *game;

    for (i = 7; i < 15; i++) {
        if (D_004B25A0__c60.active[i] != 0 && D_004C6780__c60[i] == 0) {
            func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c60[i]), D_00275BF0__c60[i], 0, D_0029D91C);
            v = *D_00275B70__c60[i];
            func_00203FE0__c60(&v, 1);
            v = *D_00275B70__c60[i];
            func_002038E0__c60(&v, 0);
        } else {
            func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c60[i]), D_00275BF0__c60[i], 0, 1);
            v = *D_00275B70__c60[i];
            func_00203CB8__c60(&v);
            func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c60[i]), D_00275BF0__c60[i], 0, 2);
            v = *D_00275B70__c60[i];
            func_00203CB8__c60(&v);
            func_00203590((struct AnimationHandle *)(void *)(D_00275B70__c60[i]), D_00275BF0__c60[i], 0, D_0029D91C);
            v = *D_00275B70__c60[i];
            func_00203F40__c60(&v, 1);
        }
        table = D_00296D48__c60;
        func_002185C0__c60(((Slot__c60 *)((char *)&D_004B25A0__c60 + table.values[i] * 0x1E0))->obj, 0);
    }
    game = &D_004B25A0__c60;
    if (game->active[14]) {
        if (D_004C67B8__c60[0] != 0) {
            return;
        }
        func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c60[14]), D_00275C68__c60[0], 0, 5);
        v = *D_00275BB0__c60[14];
        func_00203E10__c60(&v, 1.0f);
        v = *D_00275BB0__c60[14];
        func_00203B68__c60(&v, 1, 0.995f);
        v = *D_00275BB0__c60[14];
        func_002038E0__c60(&v, 0);
        game->x3720->x4 = 1;
    } else {
        func_00203590((struct AnimationHandle *)(void *)(D_00275BB0__c60[14]), D_00275C68__c60[0], 0, 0);
        v = *D_00275BB0__c60[14];
        func_00203E10__c60(&v, 1.0f);
        v = *D_00275BB0__c60[14];
        func_002038E0__c60(&v, 1);
        game->x3720->x4 = 1;
    }
}
}
}
