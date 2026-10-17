/* UNIT 0x00235878 0x00235A20 .rodata=0x299290:0x2992C0 */
#include "scalar_api.h"
#include "skat_queue.h"
#include "sound.h"
typedef SkatQueueData Queue;
typedef SkatQueueOwner Obj;
extern char D_00299220[], D_00299248[], D_00299258[];
extern void func_00254E58(char *, int, char *);
extern void func_00235BC8(Obj *, Queue *);
extern void func_00202708(int);

#if defined(__mips__)
extern void func_00235E70(void);
#else

#endif


void func_00235878(Obj *obj, unsigned int index) {
 Queue *q;
 if (!obj->queueData) { func_00254E58(D_00299220,0x13F,D_00299248); if (!obj->queueData) return; }
 if (index >= obj->numQueues) { func_00254E58(D_00299220,0x143,D_00299258); if (index >= obj->numQueues) return; }
 q = &obj->queueData[index];
 switch(q->state - 1) {
 case 0:
  if (q->readIndex != q->writeIndex) { q->state=2; func_00235BC8(obj,q); if (++q->readIndex >= 50) q->readIndex=0; }
  break;
 case 2:
  while (q->readIndex != q->writeIndex) { func_00235BC8(obj,q); if (++q->readIndex >= 50) q->readIndex=0; }
  func_00202708(q->soundHandle); q->state=4; break;
 case 3:
  if (!q->callbackPending) { q->callbackPending=1; func_00201D80(q->soundHandle, (SoundCompletionCallback)(func_00235E70), (SoundCallbackData)(q)); } break;
 case 4:
  if (!q->callbackPending && func_00235798((SkatQueueOwner *)(obj),index)) { q->soundHandle=0; q->state=1; } break;
 case 1: break; default: break;
 }
}
const char D_002992A8[] __attribute__((aligned(8))) = "pThis->m_pQueueData";
