/* UNIT 0x00207230 0x00207308 */
typedef struct Key {
    unsigned char time;
    char pad1[0x5];
} Key;

typedef struct Track {
    char pad0[0x4];
    short count;
    Key keys[1];
} Track;

typedef struct Header {
    float start;
    float scale;
    char pad8[0x8];
    Track track;
} Header;

typedef struct Pos {
    int idx;
    int x4;
    int x8;
} Pos;

int func_00207230(Track *track, short *out, float time)
{
    Header *h;
    Key *keys;
    Pos pos;
    int lo;
    int hi;
    int mid;

    h = track ? (Header *)((char *)track - 0x10) : 0;
    keys = track->keys;
    hi = track->count - 1;
    mid = hi >> 1;
    pos.idx = (time - h->start) / h->scale;
    if (pos.idx >= keys[mid + 1].time) {
        lo = mid + 1;
        mid = mid + 2;
    } else if (pos.idx >= keys[mid].time) {
        return mid;
    } else {
        lo = 0;
        hi = mid;
        mid = 1;
    }
    while (hi - lo >= 2) {
        if (pos.idx < keys[mid].time) {
            hi = mid;
        } else {
            lo = mid;
        }
        mid = (lo + hi) >> 1;
    }
    *out = lo;
    return lo;
}
