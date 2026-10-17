/* UNIT 0x001678E0 0x00167A00 */
typedef struct Frame {
    char pad0[0x20];
    float x20;
    float x24;
    char pad28[8];
} Frame;

typedef struct Track {
    Frame frames[5];
    int count;
    int indices[1];
} Track;

int func_001678E0(Track *track, int which, float *fraction, float time)
{
    int index;
    int last;
    int fresh;
    float start;
    float end;

    fresh = 1;
    *fraction = 1.0f;
    index = track->indices[which];
    last = track->count - 1;
    while (time < track->frames[index].x24) {
        if (index == last) {
            track->indices[which] = index;
            return index;
        }
        index++;
        fresh = 0;
    }
    if (fresh) {
        do {
            if (index == 0) {
                track->indices[which] = 0;
                return 0;
            }
            index--;
        } while (track->frames[index].x24 < time);
        index++;
    }
    start = track->frames[index].x20;
    if (start < time) {
        end = track->frames[index - 1].x24;
        *fraction = (end - time) / (end - start);
    }
    track->indices[which] = index;
    return index;
}
