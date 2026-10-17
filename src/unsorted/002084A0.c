/* UNIT 0x002084A0 0x00208500 */
typedef struct Entry {
    int x0;
    int x4;
    int x8;
    int xC;
} Entry;

typedef struct Queue {
    Entry entries[12];
    int count;
} Queue;

void func_002084A0(Queue *q) {
    int i;

    for (i = 0; i < q->count; i++) {
        q->entries[i].x0 = q->entries[i + 1].x0;
        q->entries[i].x4 = q->entries[i + 1].x4;
        q->entries[i].x8 = q->entries[i + 1].x8;
        q->entries[i].xC = q->entries[i + 1].xC;
    }
    q->count--;
}
