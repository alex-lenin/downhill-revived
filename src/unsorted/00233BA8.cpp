/* UNIT 0x00233BA8 0x00233EF8 */
#include "event_queue.h"
extern "C" int func_00234D08(void *, int, int);
extern "C" void *func_00235218(void *, int);

int EventQueue::func_00233BA8(int kind, int index, int *buf, int size, int link)
{
    int equalCount = 0;
    unsigned int base = Value();
    EventQueueItem **list = lists[kind];
    EventQueueItem *item;
    EventQueueInfo *info;
    int position;
    int total;
    int prio;
    int cur;
    EventQueueItem **pp;
    EventQueueItem *curItem;
    EventQueueInfo *curInfo;
    int *cnt;
    int *data;

    if (list == 0)
        return index;
    if (table == 0)
        return index;
    if (index >= table->count)
        return index;
    if (index >= 0) {
        size >>= 2;
        info = table->infos[index];
        if (info->dataWords != size)
            return index;
        if (counts[kind] >= counts[64]) {
            do {
                if (!func_00234D08(this, kind, index))
                    return index;
            } while (counts[kind] >= counts[64]);
        }
        item = (EventQueueItem *)func_00235218(heap, size + 3);
        while (counts[kind] != 0 && item == 0) {
            if (!func_00234D08(this, kind, index))
                return index;
            item = (EventQueueItem *)func_00235218(heap, size + 3);
        }
        if (item == 0)
            return index;
        data = item->data;
        prio = table->infos[index]->priority;
        item->index = index;
        item->time = base + table->infos[index]->x4;
        item->link = link;
        for (position = 0; position < size; position++)
            data[position] = buf[position];
        cnt = counts;
        cnt += kind;
        pp = list;
        position = 0;
        for (; position < *cnt; pp++) {
            curItem = *pp;
            /* retail reloads table on every iteration */
            curInfo = ((volatile EventQueue *)this)->table->infos[curItem->index];
            cur = curInfo->priority;
            if (prio < cur)
                break;
            position++;
            if (cur == prio) {
                if (curItem->time - curInfo->x4 == base)
                    equalCount++;
            }
        }
        if (equalCount != 0) {
            unsigned int r = Random();
            position -= r % (equalCount + 1);
        }
        total = *cnt;
        if (position < total) {
            int j = total;
            if (j != position) {
                /* integer form gives retail's "addu offset, list" operand order */
                EventQueueItem **p = (EventQueueItem **)(j * 4 + (int)list);
                do {
                    p[0] = p[-1];
                    p--;
                } while (--j != position);
            }
            list[position] = item;
        } else {
            list[total] = item;
        }
        counts[kind]++;
    }
    return index;
}
