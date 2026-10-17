#ifndef DHD_SKAT_QUEUE_H
#define DHD_SKAT_QUEUE_H
/* Queue state and 0x3928 stride used by the stop/update pair. */
typedef struct SkatQueueData {
    int state;
    char pad4[0x3908];
    int writeIndex, readIndex;
    unsigned char callbackPending;
    char pad3915[3];
    int soundHandle;
    int x391C, x3920, x3924;
} SkatQueueData;
typedef struct SkatQueueOwner { char pad0[0x604]; SkatQueueData *queueData; unsigned int numQueues; } SkatQueueOwner;
typedef char SkatQueueData_size_check[sizeof(SkatQueueData) == 0x3928 ? 1 : -1];
typedef char SkatQueueOwner_size_check[sizeof(SkatQueueOwner) == 0x60C ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
int func_00235798(SkatQueueOwner *, unsigned int queue);
#ifdef __cplusplus
}
#endif
#endif
