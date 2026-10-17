/* UNIT 0x001728A8 0x00172A00 */
#include "object_registry.h"
#include "scene.h"
typedef struct Mat2x4 {
    float m[2][4];
} __attribute__((aligned(16))) Mat2x4;

typedef struct Header {
    unsigned int pad : 7;
    unsigned int id : 11;
    unsigned int rest : 14;
} Header;

typedef SceneNode Model;


typedef struct Group {
    char pad0[0x86C];
    float x86C;
} Group;

typedef struct Entry {
    char pad0[0x4];
    int type;
    int x8;
    Model *xC;
    Group *group;
    char pad14[0x20 - 0x14];
    Mat2x4 x20;
    int x40;
    Mat2x4 *x44;
    char pad48[0x4C - 0x48];
    int x4C;
    char pad50[0xA0 - 0x50];
    float xA0[4];
    char padB0[0xB8 - 0xB0];
    float xB8;
    char padBC[0xD0 - 0xBC];
    struct { char data[0x124 - 0xD0]; } xD0;
    int x124;
} Entry;

extern int D_004B26AC[];
extern void func_00172A30(Entry *, Mat2x4 *);

extern void func_001D1888(void *);

void func_001728A8(Entry *self, Group *group, Model *model, Mat2x4 *mat) {
    self->xC = model;
    if (model != 0) {
        model->flags |= ((SceneNodeFlags)1 << 57);
        model->xC = 2000.0f;
    }
    self->group = group;
    self->x44 = mat;
    self->x4C = 0;
    self->x40 = 0;
    self->x20 = *mat;
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(self->xA0));
    self->xB8 = self->group->x86C;
    func_00172A30(self, self->x44);

    if (D_004B26AC[0] == 15) {
        self->x8 = 5;
    } else {
        switch (self->type) {
        case 1:
            self->x8 = 5;
            break;
        case 2:
            self->x8 = 1;
            break;
        case 3:
            self->x8 = 7;
            break;
        default:
            self->x8 = 0;
            break;
        }
    }

    if (self->xC->xB4 == 0) {
        self->xC->xB4 = RegisterObjectShortRetail((void *)(self), (unsigned int)(0));
    }
    {
        Model *res = self->xC;
        self->x124 = ((Header *)res->resource)->id;
        ((Header *)res->resource)->id = res->xB4;
        func_001D1888(&self->xD0);
    }
}
