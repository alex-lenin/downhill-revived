/* UNIT 0x00206E90 0x00206F38 */
typedef struct Key {
    float time;
    float value;
} Key;

typedef struct Track {
    char pad0[0x4];
    short count;
    char pad6[0x2];
    Key keys[1];
} Track;

int func_00206E90(Track *track, short *out, float time) {
    Key *keys = track->keys;
    int hi = track->count - 1;
    int mid = hi >> 1;
    int lo;

    if (keys[mid + 1].time <= time) {
        lo = mid + 1;
        mid = mid + 2;
    } else {
        if (!(time < keys[mid].time)) {
            return mid;
        }
        hi = mid;
        lo = 0;
        mid = 1;
    }
    while (hi - lo >= 2) {
        if (time < keys[mid].time) {
            hi = mid;
        } else {
            lo = mid;
        }
        mid = (lo + hi) >> 1;
    }
    *out = lo;
    return lo;
}
