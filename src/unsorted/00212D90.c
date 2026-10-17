/* UNIT 0x00212D90 0x00212E80 */
typedef struct {
    char pad0[0x28];
    float end;
    float timeA;
    float timeB;
    float stepA;
    float stepB;
    float now;
    int posA;
    int posB;
    int strideA;
    int strideB;
} Stream;

extern int func_00212BB0(Stream *s);

void func_00212D90(Stream *s)
{
    while (s->now < s->end) {
        if (func_00212BB0(s) != 0) {
            return;
        }
        if (s->timeA < s->timeB) {
            s->now = s->timeA;
            s->timeA += s->stepA;
            s->posA += s->strideA;
        } else if (s->timeB < s->timeA) {
            s->now = s->timeB;
            s->timeB += s->stepB;
            s->posB += s->strideB;
        } else {
            s->now = s->timeB;
            s->timeB = s->timeB + s->stepB;
            s->timeA = s->timeB;
            s->posA += s->strideA;
            s->posB += s->strideB;
        }
    }
}
