/* UNIT 0x00202C48 0x00202D78 */
typedef struct Peak {
    signed char maxIdx;
    signed char minIdx;
    short amp;
} Peak;

typedef struct Levels {
    short v[21];
} Levels;

int func_00202C48(Levels *data, Peak *out) {
    int found = 0;
    int i, j, start, end, lo, hi, last;
    short hiVal, loVal;
    Peak *p;

    i = 0;
    do {
        start = (i + 1) * i / 2;
        end = (i + 2) * (i + 1) / 2;
        p = &out[i];
        lo = start;
        hi = start;
        for (j = start + 1; j < end; j++) {
            if (data->v[j] < data->v[lo]) {
                lo = j;
            } else if (data->v[j] > data->v[hi]) {
                hi = j;
            }
        }
        hiVal = data->v[hi];
        if (hiVal < 2 || (loVal = data->v[lo]) * 2 >= hiVal) {
            p->minIdx = -1;
            p->maxIdx = -1;
            p->amp = 0;
        } else {
            p->amp = (hiVal - loVal) >> 1;
            p->maxIdx = hi - start;
            p->minIdx = lo - start;
            found = 1;
        }
        last = 5;
        i++;
    } while (i <= last);
    return found;
}

