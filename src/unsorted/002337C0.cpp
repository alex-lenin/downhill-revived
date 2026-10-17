/* UNIT 0x002337C0 0x00233BA8 */
#include "event_queue.h"
extern "C" int func_002352E8(void *, EventQueueItem *);
extern "C" int *func_00233630(void *, int);

int EventQueue::func_002337C0(int kind, int arg)
{
    unsigned int time = Value();
    int found = -1;
    EventQueueItem **list = lists[kind];
    int i, j, n;
    int done, match;
    EventQueueInfo *info;

    if (!func_00233630(this, kind)) {
        return 0;
    }
    for (i = counts[kind] - 1; i >= 0; i--) {
        info = table->infos[list[i]->index];
        if (time > list[i]->time) {
            func_002352E8(heap, list[i]);
            list[i] = 0;
        } else if (info->x14 >= 0 && x1F4 - info->x14 < info->x1) {
            func_002352E8(heap, list[i]);
            list[i] = 0;
        } else if (Random() % 100 < info->x2) {
            if (list[i]->link >= 0 && (list[i]->link & 0x40000000)) {
                continue;
            }
            if (func_00233F00(kind, arg, list[i]->index, info, &list[i]->data[0])) {
                found = list[i]->index;
                func_002352E8(heap, list[i]);
                list[i] = 0;
                info->x14 = ++x1F4;
                break;
            }
            func_002352E8(heap, list[i]);
            list[i] = 0;
        } else {
            func_002352E8(heap, list[i]);
            list[i] = 0;
        }
    }
    for (i = 0; i < counts[kind]; i++) {
        if (list[i] && table->infos[list[i]->index]->x3) {
            if (found >= 0 && (list[i]->link & 0xBFFFFFFF) == found) {
                list[i]->link = -1;
            } else if (list[i]->link == -1) {
                func_002352E8(heap, list[i]);
                list[i] = 0;
            }
        }
    }
    do {
        done = 1;
        for (i = 0; i < counts[kind]; i++) {
            if (list[i] && list[i]->link >= 0 && table->infos[list[i]->index]->x3) {
                match = 0;
                for (j = 0; j < counts[kind]; j++) {
                    if (i == j) {
                        continue;
                    }
                    if (list[j] && list[j]->index == (list[i]->link & 0xBFFFFFFF)) {
                        match = 1;
                        break;
                    }
                }
                if (!match) {
                    func_002352E8(heap, list[i]);
                    list[i] = 0;
                    done = 0;
                }
            }
        }
    } while (!done);
    n = 0;
    for (i = 0; i < counts[kind]; i++) {
        if (list[i]) {
            list[n++] = list[i];
        }
    }
    counts[kind] = n;
    return 0;
}
