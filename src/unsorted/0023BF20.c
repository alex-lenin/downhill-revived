/* UNIT 0x0023BF20 0x0023C0C8 */
static inline int elapsed(int n, int d) { return n % d; }
typedef struct Event {
    long x0;
    long x8;
    int time;
    int duration;
} Event;

typedef struct Stream {
    int x0;
    int x4;
    int x8;
    char padC[0x2C];
    int x38;
    int x3C;
    int lock;
    char pad44[0xC];
    Event *events;
    int size;
    int pending;
    int head;
} Stream;

extern int func_0025A720(int);
extern int func_0025A700(int);

int func_0023BF20(Stream *stream, Event *out)
{
    int found;
    int cursor;
    unsigned int status;
    int fraction;
    int period;
    int count;
    int start;
    int i;
    int index;
    int n;
    int head;
    Event *event;

    found = 0;
    cursor = *(volatile int *)0x1000B410;
    status = *(volatile unsigned int *)0x10002020;
    fraction = stream->x38 & 0x7F;
    period = stream->x8 << 11;
    cursor -= (((status >> 16) & 3) + ((status >> 8) & 0xF)) * 16;
    func_0025A720(stream->lock);
    out->x0 = -1;
    out->x8 = -1;
    cursor += fraction >> 3;
    cursor += period;
    cursor -= stream->x0;
    cursor = (unsigned int)cursor % period;
    count = stream->pending;
    i = 0;
    head = stream->head;
    if (count > 0) {
        start = head - count;
        for (; i < count && !found; ) {
            index = (start + stream->size + i) % stream->size;
            event = (Event *)(index * sizeof(Event) + (unsigned int)stream->events);
            i++;
            if (elapsed(cursor + period - event->time, period) < event->duration) {
                found = 1;
                out->x0 = event->x0;
                out->x8 = stream->events[index].x8;
                stream->events[index].x0 = -1;
                stream->events[index].x8 = -1;
                n = stream->pending;
                stream->pending = n - (found < n ? found : n);
            }
        }
    }
    func_0025A700(stream->lock);
    return 1;
}
