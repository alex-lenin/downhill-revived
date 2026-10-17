#ifndef DHD_EVENT_QUEUE_H
#define DHD_EVENT_QUEUE_H

typedef struct EventQueueItem {
    int index;
    unsigned int time;
    int link;
    int data[1];
} EventQueueItem;

typedef struct EventQueueInfo {
    unsigned char priority, x1, x2, x3;
    int x4;
    char pad8[2];
    unsigned short dataWords;
    char padC[8];
    int x14;
} EventQueueInfo;

typedef struct EventQueueTable {
    int count;
    char pad4[8];
    EventQueueInfo **infos;
} EventQueueTable;

#ifdef __cplusplus
/* Keep the original method form and virtual order for matching. */
class EventQueue {
    void *heap;
    EventQueueItem **lists[32];
    int counts[65];
    char pad188[0x1D8 - 0x188];
    EventQueueTable *table;
    char pad1DC[0x1F4 - 0x1DC];
    int x1F4;
    char pad1F8[0x5FC - 0x1F8];
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual unsigned int Random();
    virtual unsigned int Value();
    int func_00233BA8(int, int, int *, int, int);
    int func_002337C0(int, int);
    int func_00233F00(int, int, int, EventQueueInfo *, int *);
};
extern "C" {
#else
typedef struct EventQueue EventQueue;
#endif
int func_00233BA8(EventQueue *, int, int, int *, int, int);
int func_002337C0(EventQueue *, int, int);
#ifdef __cplusplus
}
#endif

typedef char EventQueueItem_size_check[sizeof(EventQueueItem) == 0x10 ? 1 : -1];
typedef char EventQueueInfo_size_check[sizeof(EventQueueInfo) == 0x18 ? 1 : -1];
typedef char EventQueueTable_size_check[sizeof(EventQueueTable) == 0x10 ? 1 : -1];

#endif
