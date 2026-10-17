/* UNIT 0x00234608 0x00234660 */
typedef struct Entry { int key; int value; } Entry;
Entry *func_00234608(Entry *first, int count, int key) {
    Entry *end = first + count;
    Entry *mid;
    while (first != end) {
        mid = first + (end-first)/2;
        if (key == mid->key) return mid;
        if (key < mid->key) end = mid;
        else first = mid+1;
    }
    return 0;
}
