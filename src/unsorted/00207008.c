/* UNIT 0x00207008 0x002070B8 */
typedef struct Key {
    float time;
    char pad4[0x10];
} Key;

typedef struct Track {
    char pad0[0x4];
    short count;
    char pad6[0x2];
    Key keys[1];
} Track;

/* Binary search for the key interval containing time; starts from a guess. */
int func_00207008(Track *track, short *out, float time)
{
    Key *keys = track->keys;
    int hi = track->count - 1;
    int mid = hi >> 1;
    int lo;

    if (keys[mid + 1].time <= time) {
        lo = mid + 1;
        mid = mid + 2;
    } else if (time < keys[mid].time) {
        hi = mid;
        lo = 0;
        mid = 1;
    } else {
        return mid;
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
