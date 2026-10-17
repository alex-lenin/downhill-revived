/* UNIT 0x00235798 0x00235878 */
#include "skat_queue.h"
extern char D_00299220[]; /* "SKAT989.cpp" */
extern char D_00299248[]; /* "m_pQueueData" */
extern char D_00299258[]; /* "queue < m_numQueues" */

extern void func_00254E58(char *file, int line, char *expr);
extern void func_002025A0(int soundHandle);

typedef SkatQueueData QueueData;
typedef SkatQueueOwner Skat;

int func_00235798(Skat *skat, unsigned int queue)
{
    QueueData *data;
    unsigned int voice;

    if (skat->queueData == 0) {
        func_00254E58(D_00299220, 0x110, D_00299248);
        if (skat->queueData == 0) {
            return 0;
        }
    }
    if (queue >= skat->numQueues) {
        func_00254E58(D_00299220, 0x114, D_00299258);
        if (queue >= skat->numQueues) {
            return 0;
        }
    }
    data = &skat->queueData[queue];
    data->x3920 = -1;
    voice = data->soundHandle;
    if (voice == 0) {
        return 1;
    }
    if (voice != 0xFFFFFFFFU) {
        func_002025A0(voice);
        return 1;
    }
    data->state = 5;
    return 0;
}
