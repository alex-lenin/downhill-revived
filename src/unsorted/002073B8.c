/* UNIT 0x002073B8 0x00207490 */
typedef struct {
    unsigned short key;
    unsigned short x2;
    unsigned short x4;
} TrackKey;

typedef struct {
    int x0;
    short count;
    TrackKey keys[1];
} Track;

int func_002073B8(Track *h, short *out, float t)
{
    float *f;
    TrackKey *keys;
    struct {
        int key;
        int pad;
    } ki;
    int lo;
    int hi;
    int mid;

    f = h ? (float *)h - 4 : (float *)0;
    keys = h->keys;
    hi = h->count - 1;
    mid = hi >> 1;
    ki.key = (int)((t - f[0]) / f[1]);

    if (keys[mid + 1].key <= ki.key) {
        lo = mid + 1;
        mid = mid + 2;
    } else {
        if (ki.key >= keys[mid].key) {
            return mid;
        }
        hi = mid;
        lo = 0;
        mid = 1;
    }

    while (hi - lo >= 2) {
        if (ki.key < keys[mid].key) {
            hi = mid;
        } else {
            lo = mid;
        }
        mid = (lo + hi) >> 1;
    }

    *out = lo;
    return lo;
}
