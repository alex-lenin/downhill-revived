/* UNIT 0x00235A20 0x00235BC8 */
extern char D_00299220[]; /* "SKAT989.cpp" */
extern char D_002992A8[]; /* "pThis->m_pQueueData" */
extern char D_002992C0[]; /* "pData->queue < pThis->m_numQueues" */
extern char D_002992E8[]; /* "pQueueData->m_playRequestPutIndex != pQueueData->m_playRequestTakeIndex" */

extern void func_00254E58(char *file, int line, char *expr);

typedef struct PlayRequest {
    unsigned int queue;
    char pad4[0x124 - 4];
} PlayRequest;

typedef struct QueueData {
    int state;
    PlayRequest requests[50];
    int putIndex;
    int takeIndex;
    char pad3914[0x3928 - 0x3914];
} QueueData;

typedef struct Skat {
    char pad0[0x604];
    QueueData *queueData;
    unsigned int numQueues;
} Skat;

void func_00235A20(PlayRequest *pData, Skat *pThis)
{
    QueueData *pQueueData;

    if (pThis->queueData == 0) {
        func_00254E58(D_00299220, 0x194, D_002992A8);
        if (pThis->queueData == 0) {
            return;
        }
    }
    if (pData->queue >= pThis->numQueues) {
        func_00254E58(D_00299220, 0x198, D_002992C0);
        if (pData->queue >= pThis->numQueues) {
            return;
        }
    }
    pQueueData = &pThis->queueData[pData->queue];
    pQueueData->requests[pQueueData->putIndex++] = *pData;
    if (pQueueData->putIndex >= 50) {
        pQueueData->putIndex = 0;
    }
    if (pQueueData->putIndex == pQueueData->takeIndex) {
        func_00254E58(D_00299220, 0x1A3, D_002992E8);
    }
}
