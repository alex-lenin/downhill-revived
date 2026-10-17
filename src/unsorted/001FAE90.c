/* UNIT 0x001FAE90 0x001FAFF8 */
#include "player_slot_event.h"
#include "timer.h"
typedef struct Obj {
 char pad0[0x11c8]; int x11C8; char pad11cc[4]; int x11D0;
 char pad11d4[0xbc]; volatile int x1290; char pad1294[4]; int x1298;
 char pad129c[0x149]; unsigned char x13E5; char pad13e6; volatile unsigned char x13E7;
 char pad13e8[0x3928]; char x4D10[0x40]; unsigned char x4D50;
 char pad4d51[0x18f]; char x4EE0[0x1730]; char x6610[0x1378]; unsigned int x7988;
} Obj;
typedef struct Arg { Obj *x0; char pad[0x20]; int x24; int x28; } Arg;
typedef struct Stats { char pad[0x546]; short x546; short x548; } Stats;
extern Stats D_003985B0;
typedef struct Slot { char pad[0x150]; char x150[0x1230]; } Slot;
extern Slot D_003B2270[];
extern char D_00759780[];
extern int D_004B26AC[];
extern void func_00262780(void);
extern void func_001B0870(void *, int);
extern void func_001E6420(void *, float);
extern void func_001E5B20(void *, int);

void func_001FAE90(Arg *arg0, void *arg1) {
 Obj *obj = arg0->x0;
 void *p;
 unsigned int index;
 Slot *slot;
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00262780), arg0, 1);
 func_001B0870(obj->x4D10, 0);
 obj->x4D50 = 1;
 if (!obj->x13E5) obj->x11C8 = 0;
 obj->x1290 = 0;
 if (!obj->x13E7) obj->x11D0 = 0;
 obj->x1298 = 0;
 if (arg1 != obj->x4EE0) {
  if (D_004B26AC[0] == 5) {
   if (arg0->x28 == 0) {
    int value = arg0->x24;
    if (D_003985B0.x548 < value) D_003985B0.x548 = value;
   } else {
    int value = arg0->x24;
    if (D_003985B0.x546 < value) D_003985B0.x546 = value;
   }
  }
  p = obj->x6610;
  func_001E6420(p, (float)arg0->x24);
  if (arg0->x28 == 0) { func_001E5B20(p, 5); return; }
  func_001E5B20(p, 6);
  return;
 }
 index = obj->x7988;
 slot = D_003B2270;
 if (index < 4U) SetPlayerSlotEventLegacy((char *)slot + (index * 0x1380 + 0x150), 0, 0, 0, 0, 0, 0);
}
