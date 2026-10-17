/* UNIT 0x00235488 0x00235588 */
extern "C" {
extern char D_00299220[]; /* "SKAT989.cpp" */
extern char D_00299230[]; /* "m_pQueueData == NULL" */
extern void func_00254E58(char *file, int line, char *msg);
extern void *func_001FD430(unsigned int size);
extern void *func_002353E0(void *item);
extern void func_00235A20(void);
}

struct QueueItem {
    char data[0x3928];

    QueueItem() { func_002353E0(this); }
    ~QueueItem() {}
    static void *operator new[](unsigned int size) { return func_001FD430(size); }
};

struct Queue {
    char pad0[0x604];
    QueueItem *m_pQueueData;
    int x608;
};

extern int func_00233118(Queue *queue, int count, int a, int b);
extern int func_00233660(Queue *queue, void (*cb)(void), void *arg);

int func_00235488(Queue *queue, int count, int a, int b)
{
    int result;

    if (queue->m_pQueueData != 0) {
        func_00254E58(D_00299220, 0x3E, D_00299230);
    }
    queue->m_pQueueData = new QueueItem[count];
    queue->x608 = count;
    result = func_00233118(queue, count, a, b);
    func_00233660(queue, func_00235A20, queue);
    return result;
}
