/* UNIT 0x0023CB00 0x0023CE00 */
#include "matrix_packets.h"
typedef struct Matrix { float m[4][4]; } Matrix;
typedef struct Camera { char pad0[0x10]; float pos[4]; char pad20[0x20]; Matrix x40; } Camera;
typedef struct View { float x0; char pad4[0x1C]; float x20; char pad24[12]; Matrix x30; Matrix x70; } View;
typedef struct Entry { Camera *camera; View *view; char pad8[12]; } Entry;
typedef struct Transform { char pad0[0x20]; float negPos[4]; Matrix x30,x70,xB0; } Transform;
extern Entry D_0075A9D0[];
extern Transform D_00528360[];
extern Matrix D_0075AA70[],D_0075ABF0[],D_0075AD70[],D_0075AEF0[],D_0075B070[],D_0075B1F0[];
extern Matrix *D_00279AF0[];
extern int D_0075A980[];
extern float D_002694A8[];
extern int D_0029E454;
extern void func_0023D460(Matrix *, Matrix *);


extern void func_00221D58(Matrix *, Matrix *);
extern float *func_0023CF78(int);
extern void func_0023D2C8(Matrix *, Matrix *, Matrix *, int);
void func_0023CB00(int idx)
{
    int offset = idx << 6;
    Entry *entry = (Entry *)((char *)D_0075A9D0 + idx * 20);
    Transform *transform = (Transform *)((char *)D_00528360 + idx * 240);
    Matrix *rotation = &transform->x30;
    Camera *camera = entry->camera;
    Matrix *orientation;
    Matrix *result;
    D_0029E454 = idx;
    orientation = &camera->x40;
    func_0023D460(rotation, orientation);
    transform->negPos[0] = -camera->pos[0];
    transform->negPos[1] = -camera->pos[1];
    transform->negPos[2] = -camera->pos[2];
    func_002239B0((Matrix4 *)(&transform->x70),(const Matrix4 *)(&entry->view->x30),(const Matrix4 *)(orientation));
    func_002239B0((Matrix4 *)(&transform->xB0),(const Matrix4 *)(&entry->view->x70),(const Matrix4 *)(orientation));
    result = (Matrix *)((char *)D_0075B1F0 + offset);
    func_00221930((Matrix4 *)(result),(const Matrix4 *)((Matrix *)((char *)D_0075AA70 + offset)),(const Matrix4 *)(rotation));
    func_00221930((Matrix4 *)((Matrix *)((char *)D_0075AEF0 + offset)),(const Matrix4 *)((Matrix *)((char *)D_0075ABF0 + offset)),(const Matrix4 *)(rotation));
    func_00221930((Matrix4 *)((Matrix *)((char *)D_0075B070 + offset)),(const Matrix4 *)((Matrix *)((char *)D_0075AD70 + offset)),(const Matrix4 *)(rotation));
    if (idx < 4) {
        Matrix **slot = &D_00279AF0[idx];
        Matrix *matrix;
        if (*slot) {
            float *pos;
            func_00221D58(*slot, result);
            pos = func_0023CF78(idx);
            matrix = *slot;
            matrix->m[3][0] -= matrix->m[0][0]*pos[0] + matrix->m[1][0]*pos[1] + matrix->m[2][0]*pos[2];
            matrix->m[3][1] -= matrix->m[0][1]*pos[0] + matrix->m[1][1]*pos[1] + matrix->m[2][1]*pos[2];
            matrix->m[3][2] -= matrix->m[0][2]*pos[0] + matrix->m[1][2]*pos[1] + matrix->m[2][2]*pos[2];
            matrix->m[3][3] -= matrix->m[0][3]*pos[0] + matrix->m[1][3]*pos[1] + matrix->m[2][3]*pos[2];
        }
        matrix = (Matrix *)D_002694A8;
        matrix->m[0][0] = entry->view->x0 * 0.5f;
        matrix->m[0][1] = entry->view->x0 * (entry->view->x20 * 0.5f);
    }
    func_0023D2C8(&D_0075B1F0[idx], &D_0075AEF0[idx], &D_0075B070[idx], idx);
    D_0075A980[idx] = 0;
}
