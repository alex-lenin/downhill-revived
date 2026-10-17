/* UNIT 0x001A8EF0 0x001A8FE0 */
typedef struct Sample {
    short x0;
    unsigned char x2;
    unsigned char x3;
    unsigned char x4;
    unsigned char x5;
    unsigned char x6;
    char pad7;
} Sample;

typedef struct Graph {
    char pad0[0x140];
    float x140[1];
} Graph;

typedef struct Recorder {
    char pad0[0xC];
    Sample *samples;
    Graph *x10;
    char pad14[0x24 - 0x14];
    int count;
    char pad28[0x9C8 - 0x28];
    int x9C8;
} Recorder;

#include "rider.h"
typedef struct World {
    char pad0[0x50130];
    Rider *x50130;
} World;

extern World D_002AF2C0;

extern short func_00220268(int arg);
extern void func_00220280(int arg, unsigned char *a, unsigned char *b, unsigned char *c, unsigned char *d);
extern int func_00238F60(void);

void func_001A8EF0(Recorder *rec)
{
    Sample *sample;

    sample = &rec->samples[rec->count];
    sample->x0 = func_00220268(0);
    func_00220280(0, &sample->x2, &sample->x3, &sample->x4, &sample->x5);
    sample->x6 = 1;
    if (func_00238F60() >= 2) {
        sample = &rec->samples[rec->count - 1];
        sample->x6 = func_00238F60();
    }
    if (rec->count - rec->x9C8 > 60) {
        rec->x10->x140[rec->count / 60] = D_002AF2C0.x50130->bodyNode->position.z;
        rec->x9C8 = rec->count;
    }
}
