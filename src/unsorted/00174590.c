/* UNIT 0x00174590 0x001746A8 */
typedef struct Marker {
    char pad0[0x8];
    float x8;
    char padC[0x8];
    unsigned short x14;
} Marker;

typedef struct Track {
    char pad0[0x5C];
    int count;
} Track;

extern float D_0029C8F4[];
extern Marker *func_00163D40(Track *, int);
extern Marker *func_00163F98(Track *, int, float);

Marker *func_00174590(Track *track, int arg1, float pos)
{
    Marker *best = 0;
    float bestDist = D_0029C8F4[0];
    int i;

    for (i = 0; i < track->count; i++) {
        Marker *m = func_00163D40(track, i);
        if ((m->x14 >> 1) & 1) {
            float d = pos - m->x8;
            if (pos < m->x8) {
                if (d < bestDist || 0.0f < bestDist) {
                    best = m;
                    bestDist = d;
                }
            } else if (fabsf(d) < bestDist) {
                best = m;
                bestDist = d;
            }
        }
    }
    if (best == 0) {
        best = func_00163F98(track, arg1, pos);
    }
    return best;
}
