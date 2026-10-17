/* UNIT 0x0023BE10 0x0023BF20 */
typedef struct Event {
    long x0, x8;
    int x10, x14;
} Event;

/* Ring buffer of events guarded by a lock (x40). */
typedef struct Queue {
    char pad0[0x40];
    int lock;
    char pad44[0xC];
    Event *items;
    int capacity;
    int count;
    int tail;
} Queue;

extern int func_0025A720(int);
extern void func_0025A700(int);
extern void func_0023BCB8(Queue *, Event *);

int func_0023BE10(Queue *queue, Event *event)
{
    int result = 0;

    func_0025A720(queue->lock);
    if (queue->count < queue->capacity) {
        func_0023BCB8(queue, event);
        if (event->x0 >= 0 || event->x8 >= 0) {
            queue->items[queue->tail].x0 = event->x0;
            queue->items[queue->tail].x8 = event->x8;
            queue->items[queue->tail].x10 = event->x10;
            queue->items[queue->tail].x14 = event->x14;
            queue->count++;
            queue->tail = (queue->tail + 1) % queue->capacity;
        }
        result = 1;
    }
    func_0025A700(queue->lock);
    return result;
}
