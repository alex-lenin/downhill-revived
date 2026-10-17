/* UNIT 0x00139940 0x00139A48 */
#include "scalar_api.h"
typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct Batch {
    int x0;
    Vec4 points[10];
    int xA4;
    int values[10];
} Batch;

typedef struct Model {
    char pad0[0x20];
    Batch x20;
} Model;

typedef struct Node {
    long x0;
} Node;

typedef struct NodeList {
    char pad0[0x20];
    Node *x20[10];
} NodeList;

typedef struct Scalar00139940Obj {
    char pad0[0x31714];
    NodeList *x31714;
    char pad31718[0x3171C - 0x31718];
    int x3171C;
    int x31720;
} Obj;

extern Model *func_00215B58(Node *);

void func_00139940(Obj *obj, float *pos, float w, int value) {
    Node *node;
    Batch *batch;
    Vec4 *points;

    if (obj->x3171C < 10) {
        node = obj->x31714->x20[obj->x3171C];
        node->x0 |= 0x100000000;
        batch = (Batch *)func_00215B58(node);
        batch = (Batch *)((char *)batch + 0x20);
        points = batch->points;
        points[obj->x31720].x = pos[0];
        points[obj->x31720].y = pos[1];
        points[obj->x31720].z = pos[2];
        points[obj->x31720].w = w;
        batch->values[obj->x31720] = value;
        obj->x31720++;
        if (obj->x31720 >= 10) {
            obj->x31720 = 0;
            obj->x3171C++;
        }
    }
}
