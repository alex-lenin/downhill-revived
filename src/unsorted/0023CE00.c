/* UNIT 0x0023CE00 0x0023CEF0 */
#include "matrix_packets.h"
typedef struct Matrix {
    float m[4][4];
} Matrix;

struct Entry {
    Matrix *x0;
    char *x4;
    char pad8[0x14 - 0x8];
};

struct Object {
    char pad0[0x30];
    Matrix x30;
    Matrix x70;
    Matrix xB0;
};

extern struct Entry D_0075A9D0[];
extern struct Object D_00528360[];
extern Matrix D_0075B1F0[];
extern Matrix D_0075AA70[];
extern Matrix D_0075B3B0;
extern Matrix D_0075B370;

extern void func_0023D460(Matrix *dst, Matrix *src);

extern void func_0023CEF0(void);
extern void func_0023CF10(void);


void func_0023CE00(int index)
{
    struct Entry *entry;
    struct Object *obj;
    Matrix *base;

    entry = &D_0075A9D0[index];
    obj = &D_00528360[index];
    base = entry->x0 + 1;
    func_0023D460(&obj->x30, base);
    func_002239B0((Matrix4 *)(&obj->x70),(const Matrix4 *)(entry->x4 + 0x30),(const Matrix4 *)(base));
    func_0023CEF0();
    func_002239B0((Matrix4 *)(&obj->xB0),(const Matrix4 *)(entry->x4 + 0x70),(const Matrix4 *)(base));
    func_0023CF10();
    func_00221930((Matrix4 *)(&D_0075B1F0[index]),(const Matrix4 *)(&D_0075AA70[index]),(const Matrix4 *)(&obj->x30));
    func_00221930((Matrix4 *)(&D_0075B3B0),(const Matrix4 *)(&D_0075B370),(const Matrix4 *)(&obj->x30));
}
