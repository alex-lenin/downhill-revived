/* UNIT 0x00117A98 0x00117CA0 */
#include "track_bins.h"


extern int func_00117A10(PathBins *bins, float value);
extern short func_00117CA0(PathBins *bins, float *pos, int index, float *distance);
extern void func_00254D08(const char *fmt, ...);
extern char D_002889A0[];
extern float D_0029C110[];

void func_00117A98(PathBins *bins, float *pos, short *outIndex, short *outPoint, float *outDistance) {
    int bin;
    short *p;
    short index;
    short point;
    float best;
    float start;
    float distance;
    float d;
    int loops;
    TrackBinNode *node;

    bin = func_00117A10(bins, pos[2] + 50.0f);
    index = *outIndex = bins->bins[bin];
    if (index < 0) {
        do {
            if (bin == 0) {
                index = *outIndex = bins->bins[0];
                if (index >= 0) break;
                p = bins->bins;
                while (++bin < 1024) {
                    index = *outIndex = *++p;
                    if (index >= 0) break;
                }
            } else {
                bin--;
                if (bin < 0) {
                    *outIndex = 0;
                    *outPoint = 1;
                    return;
                }
                index = *outIndex = bins->bins[bin];
            }
        } while (index < 0);
    }
    best = D_0029C110[0];
    *outPoint = -1;
    if (index >= 0) {
        start = best;
        loops = 0;
        do {
            distance = start;
            point = func_00117CA0(bins, pos, index, &distance);
            d = distance;
            if (point >= 0 && d < best) {
                *outIndex = index;
                best = d;
                *outPoint = point;
                if (outDistance) *outDistance = d;
            }
            index++;
            node = &bins->nodes[index];
            if (loops >= 10000) {
                func_00254D08(D_002889A0);
                return;
            }
            if (index >= bins->count || node->pos[2] < pos[2] - 100.0f) index = -1;
        } while (index >= 0);
    }
}
