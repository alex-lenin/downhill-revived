/* UNIT 0x00192118 0x001921C0 */
typedef struct Vec4 {
    float v[4];
} Vec4 __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec4 pos;
} Body;

typedef struct Rider {
    char pad0[0x792C];
    Body *body;
} Rider;

typedef struct World {
    char pad0[0x501A0];
    Rider *rider;
} World;

typedef struct Node {
    char pad0[0x10];
    Vec4 pos;
    char pad20[0x40 - 0x20];
    char x40[0xA0 - 0x40];
    int xA0;
    int xA4;
    float distance;
} Node;

typedef struct Marker {
    char pad0[0x8];
    Node *node;
} Marker;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

extern World D_002AF2C0;
extern float func_00222610(Vec4 *a, Vec4 *b);
extern void func_00221F50(void *a, int *b);
extern void func_002182A8(Node *node, int arg);
extern void func_00191BC0(Marker *marker, int arg);

void func_00192118(Marker *marker, Vec4 *pos) {
    Node *node;
    Rider *rider;

    if (pos != 0) {
        node = marker->node;
        if (node == 0) {
            return;
        }
        rider = D_002AF2C0.rider;
        COPY_VECTOR(node->pos, pos);
        if (rider != 0) {
            marker->node->xA0 = 0;
            marker->node->xA4 = 0;
            marker->node->distance = func_00222610(&marker->node->pos, &rider->body->pos);
            func_00221F50(marker->node->x40, &marker->node->xA0);
        }
        func_002182A8(marker->node, 1);
        func_00191BC0(marker, 1);
    }
}
