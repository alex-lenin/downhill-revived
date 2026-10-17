#ifndef DHD_PATH_MOTION_H
#define DHD_PATH_MOTION_H
#include "vector.h"

typedef struct PathNode {
    Vector4 pos;
    short x10;
    char pad12[4];
    short x16;
} PathNode;

typedef struct PathTrack {
    int x0;
    PathNode *prev, *cur, *next;
    Vector4 pos, x20, x30, x40, x50, x60, offset;
    float x80;
    unsigned char x84;
} PathTrack;

#ifdef __cplusplus
extern "C" {
#endif
float func_001DDA50(void *, PathTrack *, Vector4 *);
#ifdef __cplusplus
}
#endif
#endif
